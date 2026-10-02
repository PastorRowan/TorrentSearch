
#include "Aria2c/Aria2c.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <atomic>

std::atomic<quint64> getNextRequestId{ 1 };

Aria2c::Aria2c(
    QObject* parent
):
    QObject(parent),
    aria2Process(this),
    networkAccessManager(this) {

};

void Aria2c::request(
    const RpcMethod method,
    const QJsonArray& params,
    std::function<void(std::variant<QJsonValue, Aria2Error>)> callback
) {

    // Build the params array with the auth token prepended.
    // aria2 requires "token:<secret>" as the first element of `params`
    // when it was started with --rpc-secret.
    QJsonArray actualParams;

    if (!secret.isEmpty()) {
        actualParams.append(QStringLiteral("token:") + secret);
    };

    for (const QJsonValue& param : params) {
        actualParams.append(param);
    };

    const quint64 id = getNextRequestId.fetch_add(1, std::memory_order_relaxed);

    QJsonObject requestObj {
        { QStringLiteral("jsonrpc"), QStringLiteral("2.0") },
        { QStringLiteral("id"),      QString::number(id) },
        { QStringLiteral("method"),  rpcMethodEnumToRpcMethodName(method) },
        { QStringLiteral("params"),  actualParams }
    };

    QNetworkRequest networkRequest {
        QUrl(rpcUrl)
    };
    networkRequest.setHeader(
        QNetworkRequest::ContentTypeHeader,
        QStringLiteral("application/json")
    );

    QNetworkReply* reply =
        networkAccessManager.post(
            networkRequest,
            QJsonDocument(requestObj).toJson(QJsonDocument::Compact)
        );

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        [ reply, callback = std::move(callback) ]() mutable {

            reply->deleteLater();

            const QByteArray body = reply->readAll();

            // Try to parse the body regardless of transport status.
            // Some HTTP-level errors (401, 400) still carry a
            // structured JSON-RPC error object, and we don't want to
            // lose that message by treating them as pure transport failures.
            QJsonParseError parseError{};
            const QJsonDocument doc = QJsonDocument::fromJson(body, &parseError);
            const bool parsed =
                (parseError.error == QJsonParseError::NoError) && doc.isObject();

            // ---- Transport-level failure (connection refused, timeout, ...) ----
            if (reply->error() != QNetworkReply::NoError) {

                // If the body happens to be a JSON-RPC error, prefer that —
                // it carries a proper code and message from aria2.
                if (parsed) {
                    const QJsonObject obj = doc.object();
                    if (obj.contains(QStringLiteral("error"))) {
                        const QJsonObject err =
                            obj.value(QStringLiteral("error")).toObject();
                        callback(Aria2Error{
                            err.value(QStringLiteral("code")).toInt(),
                            err.value(QStringLiteral("message")).toString(),
                            Aria2Error::Source::Rpc
                        });
                        return;
                    }
                }

                qWarning(
                    "Aria2c::request: transport error: %s",
                    qPrintable(reply->errorString())
                );
                callback(Aria2Error{
                    -1,
                    reply->errorString(),
                    Aria2Error::Source::Transport
                });
                return;
            };

            // ---- Parse failure (body wasn't valid JSON, or wasn't an object) ----
            if (!parsed) {
                qWarning(
                    "Aria2c::request: invalid JSON response: %s",
                    qPrintable(parseError.errorString())
                );
                callback(Aria2Error{
                    -2,
                    parseError.errorString(),
                    Aria2Error::Source::Parse
                });
                return;
            };

            const QJsonObject responseObj = doc.object();

            // ---- JSON-RPC error object: { "code": int, "message": string } ----
            if (responseObj.contains(QStringLiteral("error"))) {
                const QJsonObject err =
                    responseObj.value(QStringLiteral("error")).toObject();
                const int     code    = err.value(QStringLiteral("code")).toInt();
                const QString message = err.value(QStringLiteral("message")).toString();

                qWarning(
                    "Aria2c::request: RPC error %d: %s",
                    code,
                    qPrintable(message)
                );

                callback(Aria2Error{
                    code,
                    message,
                    Aria2Error::Source::Rpc
                });
                return;
            };

            // ---- Success: hand the `result` value to the caller ----
            callback(responseObj.value(QStringLiteral("result")));
        }
    );

};

#define DEFINE_RPC_METHOD_Y(func, name) \
    void Aria2c::func( \
        const name##Params& params, \
        std::function<void(std::variant<name##Response, Aria2Error>)> cb \
    ) { \
        request( \
            RpcMethod::name, \
            params.toQJsonArray(), \
            [ cb = std::move(cb) ](std::variant<QJsonValue, Aria2Error> outcome) { \
                if (auto* err = std::get_if<Aria2Error>(&outcome)) { \
                    cb(*err); \
                } else { \
                    cb(name##Response::fromQJsonValue(std::get<QJsonValue>(outcome))); \
                } \
            } \
        ); \
    }


#define DEFINE_RPC_METHOD_N(func, name) \
    void Aria2c::func( \
        std::function<void(std::variant<name##Response, Aria2Error>)> cb \
    ) { \
        request( \
            RpcMethod::name, \
            QJsonArray{}, \
            [ cb = std::move(cb) ](std::variant<QJsonValue, Aria2Error> outcome) { \
                if (auto* err = std::get_if<Aria2Error>(&outcome)) { \
                    cb(*err); \
                } else { \
                    cb(name##Response::fromQJsonValue(std::get<QJsonValue>(outcome))); \
                } \
            } \
        ); \
    }


#define DEFINE_RPC_METHOD(function, name, hasParams) \
    DEFINE_RPC_METHOD_##hasParams(function, name)

#define X(function, method, name, hasParams) \
    DEFINE_RPC_METHOD(function, name, hasParams)

RPC_METHODS

#undef X
#undef DEFINE_RPC_METHOD
#undef DEFINE_RPC_METHOD_N
#undef DEFINE_RPC_METHOD_Y
