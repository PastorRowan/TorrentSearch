
#include "Aria2c/Aria2c.h"
#include "helpers/helpers.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <atomic>
#include <QDebug>

std::atomic<quint64> getNextRequestId{ 1 };

Aria2c::Aria2c(
    QObject* parent
):
    QObject(parent),
    aria2cProcess(new QProcess(this)),
    networkAccessManager(new QNetworkAccessManager(this)) {

    connect(
        aria2cProcess,
        &QProcess::started,
        this,
        [ this ]() {
            qDebug() << "Started aria2c process";
            emit started();
        }
    );

    connect(
        aria2cProcess,
        &QProcess::errorOccurred,
        this,
        [ this ](QProcess::ProcessError error) {
            qDebug()
                << "Aria2c process error:\n"
                << "error =" << error << "\n"
                << "errorString =" << aria2cProcess->errorString() << "\n"
                << "program =" << aria2cProcess->program() << "\n"
                << "arguments =" << aria2cProcess->arguments()
            ;
            emit stopped();
        }
    );

};

bool Aria2c::isNotRunning() const {
    return aria2cProcess->state() == QProcess::NotRunning;
};

bool Aria2c::isStarting() const {
    return aria2cProcess->state() == QProcess::Starting;
};

bool Aria2c::isRunning() const {
    return aria2cProcess->state() == QProcess::Running;
};

void Aria2c::start() {

    qDebug()
        << "Aria2c::start() called\n"
        << "isRunning(): " << isRunning()
    ;

    if (isRunning() || isStarting()) {
        qDebug() << "Aria2c is already running or starting";
        return;
    };

    aria2cProcess->start(
        aria2cBinFileLocation,
        aria2Arguments
    );

};

void Aria2c::stop() {

    qDebug() << "Aria2c::stop() called";

    if (aria2cProcess->state() == QProcess::NotRunning) {
        return;
    };

    aria2Shutdown(
        this,
        [ this ](std::variant<Aria2ShutdownResponse, Aria2Error> outcome) {

            if (auto* err = std::get_if<Aria2Error>(&outcome)) {
                qDebug() << "Failed to shutdown aria2c RPC server" << err->message;
                aria2ForceShutdown(
                    this,
                    [ this ](std::variant<Aria2ForceShutdownResponse, Aria2Error> outcome) {
                        if (auto* err = std::get_if<Aria2Error>(&outcome)) {
                            qDebug() << "Failed to force shutdown aria2c RPC server" << err->message;
                        };
                        aria2cProcess->terminate();
                    }
                );
                return;
            };

            aria2cProcess->terminate();

        }
    );

    if (!aria2cProcess->waitForFinished(4000)) {
        qWarning() << "aria2c did not terminate gracefully; killing process.";
        aria2cProcess->kill();
        aria2cProcess->waitForFinished();
    };

};

void Aria2c::request(
    const RpcMethod method,
    const QJsonArray& params,
    QObject* context,
    std::function<void(std::variant<QJsonValue, Aria2Error>)> callback
) {

    #define LOG_REQUEST 0

    #if LOG_REQUEST
        qDebug().noquote()
            << "method: " << rpcMethodEnumToRpcMethodName(method) << "\n"
            << "params: " << helpers::prettyQJson(params)
        ;
    #endif
    #undef LOG_REQUEST

    if (!isRunning()) {
        callback(Aria2Error{
            .code = -1,
            .message = "Aria2c QProcess is not running",
            .source = Aria2Error::Source::Rpc
        });
        return;
    };

    // Build the params array with the auth token prepended.
    // aria2 requires "token:<secret>" as the first element of `params`
    // when it was started with --rpc-secret.
    QJsonArray actualParams;

    if (!secret.isEmpty()) {
        QJsonArray methods = params;
        if (method == RpcMethod::SystemMulticall) {
            for (unsigned int i = 0; i < methods.size(); ++i) {
                QJsonObject entry = methods[i].toObject();
                QJsonArray entryParams = entry["params"].toArray();
                entryParams.prepend("token:" + secret);
                entry["params"] = entryParams;
                methods[i] = entry;
            };
        actualParams.append(methods);
        } else {
            actualParams.append("token:" + secret);
            for (const QJsonValue& param : params) {
                actualParams.append(param);
            };
        };
    };

    const quint64 id = getNextRequestId.fetch_add(1, std::memory_order_relaxed);

    QJsonObject requestObj {
        { "jsonrpc", "2.0" },
        { "id", QString::number(id) },
        { "method",  rpcMethodEnumToRpcMethodName(method) },
        { "params",  actualParams }
    };

    QNetworkRequest networkRequest {
        QUrl(rpcUrl)
    };
    networkRequest.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json"
    );

    QNetworkReply* reply =
        networkAccessManager->post(
            networkRequest,
            QJsonDocument(requestObj).toJson(QJsonDocument::Compact)
        );

    connect(
        reply,
        &QNetworkReply::finished,
        context,
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
                    if (obj.contains("error")) {
                        const QJsonObject err =
                            obj.value("error").toObject();
                        callback(Aria2Error{
                            .code = err.value("code").toInt(),
                            .message = err.value("message").toString(),
                            .source = Aria2Error::Source::Rpc
                        });
                        return;
                    }
                }

                qWarning(
                    "Aria2c::request: transport error: %s",
                    qPrintable(reply->errorString())
                );
                callback(Aria2Error{
                    .code = -1,
                    .message = reply->errorString(),
                    .source = Aria2Error::Source::Transport
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
                    .code = -2,
                    .message = parseError.errorString(),
                    .source = Aria2Error::Source::Parse
                });
                return;
            };

            const QJsonObject responseObj = doc.object();

            // ---- JSON-RPC error object: { "code": int, "message": string } ----
            if (responseObj.contains("error")) {
                const QJsonObject err =
                    responseObj.value("error").toObject();
                const int     code    = err.value("code").toInt();
                const QString message = err.value("message").toString();

                qWarning(
                    "Aria2c::request: RPC error %d: %s",
                    code,
                    qPrintable(message)
                );

                callback(Aria2Error{
                    .code = code,
                    .message = message,
                    .source = Aria2Error::Source::Rpc
                });
                return;
            };

            // ---- Success: hand the `result` value to the caller ----
            callback(responseObj.value("result"));

        }
    );

};

#define DEFINE_RPC_METHOD_Y(func, name) \
    void Aria2c::func( \
        const name##Params& params, \
        QObject* context, \
        std::function<void(std::variant<name##Response, Aria2Error>)> cb \
    ) { \
        request( \
            RpcMethod::name, \
            params.toQJsonArray(), \
            context, \
            [ cb = std::move(cb) ](std::variant<QJsonValue, Aria2Error> outcome) { \
                if (auto* err = std::get_if<Aria2Error>(&outcome)) { \
                    cb(*err); \
                } else if (auto* qJsonValueResponse = std::get_if<QJsonValue>(&outcome)) { \
                    cb(name##Response::fromQJsonValue(*qJsonValueResponse)); \
                } else { \
                    qFatal( \
                        "Error: '%s' received an unknown type", \
                        #name \
                    ); \
                }; \
            } \
        ); \
    }


#define DEFINE_RPC_METHOD_N(func, name) \
    void Aria2c::func( \
        QObject* context, \
        std::function<void(std::variant<name##Response, Aria2Error>)> cb \
    ) { \
        request( \
            RpcMethod::name, \
            QJsonArray{}, \
            context, \
            [ cb = std::move(cb) ](std::variant<QJsonValue, Aria2Error> outcome) { \
                if (auto* err = std::get_if<Aria2Error>(&outcome)) { \
                    cb(*err); \
                } else if (auto* qJsonValueResponse = std::get_if<QJsonValue>(&outcome)) { \
                    cb(name##Response::fromQJsonValue(*qJsonValueResponse)); \
                } else { \
                    qFatal( \
                        "Error: '%s' received an unknown type", \
                        #name \
                    ); \
                }; \
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
