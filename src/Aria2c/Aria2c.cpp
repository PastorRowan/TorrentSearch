
#include "Aria2c/Aria2c.h"

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
    aria2Process(parent),
    networkAccessManager(parent) {

};

void Aria2c::request(
    const RpcMethod method,
    const QJsonArray& params,
    std::function<void(const QJsonValue&)> callback
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
        { "jsonrpc", "2.0" },
        { "id", QString::number(id) },
        { "method", rpcMethodEnumToRpcMethodName(method) },
        { "params", actualParams }
    };

    QNetworkRequest networkRequest {
        QUrl(rpcUrl)
    };
    networkRequest.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json"
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

            // Transport-level failure (connection refused, timeout, ...).
            if (reply->error() != QNetworkReply::NoError) {
                qWarning(
                    "Aria2c::request: transport error: %s",
                    qPrintable(reply->errorString())
                );
                callback(QJsonValue(QJsonValue::Null));
                return;
            };

            const QByteArray body = reply->readAll();

            QJsonParseError parseError{};
            const QJsonDocument doc = QJsonDocument::fromJson(body, &parseError);

            if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
                qWarning(
                    "Aria2c::request: invalid JSON response: %s",
                    qPrintable(parseError.errorString())
                );
                callback(QJsonValue(QJsonValue::Null));
                return;
            };

            const QJsonObject responseObj = doc.object();

            // JSON-RPC error object: { "code": int, "message": string }
            if (responseObj.contains(QStringLiteral("error"))) {
                const QJsonObject err = responseObj.value(QStringLiteral("error")).toObject();
                qWarning(
                    "Aria2c::request: RPC error %lld: %s",
                    static_cast<long long>(err.value(QStringLiteral("code")).toInt()),
                    qPrintable(err.value(QStringLiteral("message")).toString())
                );
                callback(QJsonValue(QJsonValue::Null));
                return;
            };

            // Success: hand the `result` value to the caller.
            callback(responseObj.value(QStringLiteral("result")));
        }
    );

};

struct AddUriParams {

    QStringList uris;

    struct Tail1 {
        QJsonObject options;
        std::optional<int> position;
        void appendTo(QJsonArray& array) const {
            array.append(options);
            if (position) {
                array.append(*position);
            };
        };
    };

    std::optional<Tail1> tail1;

    QJsonArray toQJsonArray() const {

        QJsonArray array;
        QJsonArray urisArray;

        for (const QString& uri : uris) {
            urisArray.append(uri);
        };

        array.append(urisArray);

        if (tail1) {
            tail1->appendTo(array);
        };

        return array;

    };

};

struct AddUriResponse {
    static AddUriResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return AddUriResponse{};
    };
};

void Aria2c::aria2AddUri(
    const AddUriParams& params,
    std::function<void (const AddUriResponse&)> cb
) {

};


struct AddTorrentParams {

    // raw torrent bytes; base64-encoded on the wire
    QByteArray torrent;

    // Trailing optional args, in the order aria2 expects them:
    //     [, uris[, options[, position]]]
    //
    // Each level of nesting mirrors one level of `[...]` in the spec.
    struct Tail1 {

        QStringList uris;

        struct Tail2 {
            QJsonObject options;
            std::optional<int> position;

            void appendTo(QJsonArray& array) const {
                array.append(options);
                if (position) {
                    array.append(*position);
                };
            };
        };

        std::optional<Tail2> tail2;

        void appendTo(QJsonArray& array) const {
            QJsonArray urisArray;
            for (const QString& uri : uris) {
                urisArray.append(uri);
            };
            array.append(urisArray);
            if (tail2) {
                tail2->appendTo(array);
            };
        };

    };

    std::optional<Tail1> tail1;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(QString::fromLatin1(torrent.toBase64()));
        if (tail1) {
            tail1->appendTo(array);
        };
        return array;
    };

};

struct AddTorrentResponse {
    static AddTorrentResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return AddTorrentResponse{};
    };
};

void Aria2c::aria2AddTorrent(
    const AddTorrentParams& params,
    std::function<void (const AddTorrentResponse&)> cb
) {

};


struct AddMetalinkParams {

    // raw metalink bytes; base64-encoded on the wire
    QByteArray metalink;

    // Trailing optional args, in the order aria2 expects them:
    //     [, options[, position]]
    struct Tail1 {
        QJsonObject options;
        std::optional<int> position;

        void appendTo(QJsonArray& array) const {
            array.append(options);
            if (position) {
                array.append(*position);
            };
        };
    };

    std::optional<Tail1> tail1;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(QString::fromLatin1(metalink.toBase64()));
        if (tail1) {
            tail1->appendTo(array);
        };
        return array;
    };

};

struct AddMetalinkResponse {
    static AddMetalinkResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return AddMetalinkResponse{};
    };
};

void Aria2c::aria2cAddMetalink() {
    const AddMetalinkParams& params,
    std::function<void (const AddMetalinkResponse&)> cb
};


struct RemoveParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    }
};

struct RemoveResponse {
    static RemoveResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return RemoveResponse{};
    };
};

void Aria2c::aria2cRemove(

) {

};


struct ForceRemoveParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    };
};

struct ForceRemoveResponse {
    static ForceRemoveResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ForceRemoveResponse{};
    };
};

void Aria2c::aria2ForceRemove(
    const ForceRemoveParams& params,
    std::function<void (const ForceRemoveResponse&)> cb
) {

};


struct PauseParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    };
};

struct PauseResponse {
    static PauseResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return PauseResponse{};
    };
};

void Aria2c::aria2Pause(
    const PauseParams& params,
    std::function<void (const PauseResponse&)> cb
) {

};


struct PauseAllResponse {
    static PauseAllResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return PauseAllResponse{};
    };
};

void Aria2c::aria2PauseAll(
    const NoParams& params,
    std::function<void (const PauseAllResponse&)> cb
) {

};


struct ForcePauseParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    };
};

struct ForcePauseResponse {
    static ForcePauseResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ForcePauseResponse{};
    };
};

void Aria2c::aria2ForcePause(
    const ForcePauseParams& params,
    std::function<void (const ForcePauseResponse&)> cb
) {

};


struct ForcePauseAllResponse {
    static ForcePauseAllResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ForcePauseAllResponse{};
    };
};

void Aria2c::aria2ForcePauseAll(
    const NoParams& params,
    std::function<void (const ForcePauseAllResponse&)> cb
) {

};


struct UnpauseParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    }
};

struct UnpauseResponse {
    static UnpauseResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return UnpauseResponse{};
    };
};

void Aria2c::aria2Unpause(
    const UnpauseParams& params,
    std::function<void (const UnpauseResponse&)> cb
) {

};


struct UnpauseAllResponse {
    static UnpauseAllResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return UnpauseAllResponse{};
    };
};

void Aria2c::aria2UnpauseAll(
    const NoParams& params,
    std::function<void (const UnpauseAllResponse&)> cb
) {

};


struct TellStatusParams {

    QString gid;

    // Single trailing optional with no positional coupling —
    // a plain std::optional is sufficient here.
    std::optional<QStringList> keys;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        if (keys) {
            QJsonArray keysArray;
            for (const QString& key : *keys) {
                keysArray.append(key);
            };
            array.append(keysArray);
        };
        return array;
    };
};

struct TellStatusResponse {
    static TellStatusResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return TellStatusResponse{};
    };
};

void Aria2c::aria2TellStatus(
    const TellStatusParams& params,
    std::function<void (const TellStatusResponse&)> cb
) {

};
