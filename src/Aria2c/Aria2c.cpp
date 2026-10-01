
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

struct Aria2AddUriParams {
    QStringList uris;
    struct Tail1 {
        QJsonObject options;
        std::optional<int> position;
    };
    std::optional<Tail1> tail1;
};

struct Aria2AddUriResponse {};

void Aria2c::aria2AddUri(
    const Aria2AddUriParams& params,
    std::function<void (const Aria2AddUriResponse&)> cb
) {

    QJsonArray qJsonArray;

    QJsonArray urisArray;
    for (const QString& uri : params.uris) {
        urisArray.append(uri);
    };
    qJsonArray.append(urisArray);

    if (params.tail1) {
        qJsonArray.append(params.tail1->options);
        if (params.tail1->position) {
            qJsonArray.append(*params.tail1->position);
        };
    };

    request(
        RpcMethod::Aria2AddUri,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            // TODO: parse `value` into Aria2AddUriResponse fields
            (void)value;
            cb(Aria2AddUriResponse{});
        }
    );

};


struct Aria2AddTorrentParams {
    QByteArray torrent;
    struct Tail1 {
        QStringList uris;
        struct Tail2 {
            QJsonObject options;
            std::optional<int> position;
        };
        std::optional<Tail2> tail2;
    };
    std::optional<Tail1> tail1;
};

struct Aria2AddTorrentResponse {};

void Aria2c::aria2AddTorrent(
    const Aria2AddTorrentParams& params,
    std::function<void (const Aria2AddTorrentResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(QString::fromLatin1(params.torrent.toBase64()));

    if (params.tail1) {
        QJsonArray urisArray;
        for (const QString& uri : params.tail1->uris) {
            urisArray.append(uri);
        };
        qJsonArray.append(urisArray);

        if (params.tail1->tail2) {
            qJsonArray.append(params.tail1->tail2->options);
            if (params.tail1->tail2->position) {
                qJsonArray.append(*params.tail1->tail2->position);
            };
        };
    };

    request(
        RpcMethod::Aria2AddTorrent,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2AddTorrentResponse{});
        }
    );

};


struct Aria2AddMetalinkParams {
    QByteArray metalink;
    struct Tail1 {
        QJsonObject options;
        std::optional<int> position;
    };
    std::optional<Tail1> tail1;
};

struct Aria2AddMetalinkResponse {};

void Aria2c::aria2AddMetalink(
    const Aria2AddMetalinkParams& params,
    std::function<void (const Aria2AddMetalinkResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(QString::fromLatin1(params.metalink.toBase64()));

    if (params.tail1) {
        qJsonArray.append(params.tail1->options);
        if (params.tail1->position) {
            qJsonArray.append(*params.tail1->position);
        };
    };

    request(
        RpcMethod::Aria2AddMetalink,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2AddMetalinkResponse{});
        }
    );

};


struct Aria2RemoveParams {
    QString gid;
};

struct Aria2RemoveResponse {};

void Aria2c::aria2Remove(
    const Aria2RemoveParams& params,
    std::function<void (const Aria2RemoveResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(params.gid);

    request(
        RpcMethod::Aria2Remove,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2RemoveResponse{});
        }
    );

};


struct Aria2ForceRemoveParams {
    QString gid;
};

struct Aria2ForceRemoveResponse {};

void Aria2c::aria2ForceRemove(
    const Aria2ForceRemoveParams& params,
    std::function<void (const Aria2ForceRemoveResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(params.gid);

    request(
        RpcMethod::Aria2ForceRemove,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2ForceRemoveResponse{});
        }
    );

};


struct Aria2PauseParams {
    QString gid;
};

struct Aria2PauseResponse {};

void Aria2c::aria2Pause(
    const Aria2PauseParams& params,
    std::function<void (const Aria2PauseResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(params.gid);

    request(
        RpcMethod::Aria2Pause,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2PauseResponse{});
        }
    );

};


struct Aria2PauseAllResponse {};

void Aria2c::aria2PauseAll(
    std::function<void (const Aria2PauseAllResponse&)> cb
) {

    request(
        RpcMethod::Aria2PauseAll,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2PauseAllResponse{});
        }
    );

};


struct Aria2ForcePauseParams {
    QString gid;
};

struct Aria2ForcePauseResponse {};

void Aria2c::aria2ForcePause(
    const Aria2ForcePauseParams& params,
    std::function<void (const Aria2ForcePauseResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(params.gid);

    request(
        RpcMethod::Aria2ForcePause,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2ForcePauseResponse{});
        }
    );

};


struct Aria2ForcePauseAllResponse {};

void Aria2c::aria2ForcePauseAll(
    std::function<void (const Aria2ForcePauseAllResponse&)> cb
) {

    request(
        RpcMethod::Aria2ForcePauseAll,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2ForcePauseAllResponse{});
        }
    );

};


struct Aria2UnpauseParams {
    QString gid;
};

struct Aria2UnpauseResponse {};

void Aria2c::aria2Unpause(
    const Aria2UnpauseParams& params,
    std::function<void (const Aria2UnpauseResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(params.gid);

    request(
        RpcMethod::Aria2Unpause,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2UnpauseResponse{});
        }
    );

};


struct Aria2UnpauseAllResponse {};

void Aria2c::aria2UnpauseAll(
    std::function<void (const Aria2UnpauseAllResponse&)> cb
) {

    request(
        RpcMethod::Aria2UnpauseAll,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2UnpauseAllResponse{});
        }
    );

};


struct Aria2TellStatusParams {
    QString gid;
    std::optional<QStringList> keys;
};

struct Aria2TellStatusResponse {};

void Aria2c::aria2TellStatus(
    const Aria2TellStatusParams& params,
    std::function<void (const Aria2TellStatusResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(params.gid);

    if (params.keys) {
        QJsonArray keysArray;
        for (const QString& key : *params.keys) {
            keysArray.append(key);
        };
        qJsonArray.append(keysArray);
    };

    request(
        RpcMethod::Aria2TellStatus,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2TellStatusResponse{});
        }
    );

};


struct Aria2GetUrisParams {
    QString gid;
};

struct Aria2GetUrisResponse {};

void Aria2c::aria2GetUris(
    const Aria2GetUrisParams& params,
    std::function<void (const Aria2GetUrisResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(params.gid);

    request(
        RpcMethod::Aria2GetUris,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2GetUrisResponse{});
        }
    );

};


struct Aria2GetFilesParams {
    QString gid;
};

struct Aria2GetFilesResponse {};

void Aria2c::aria2GetFiles(
    const Aria2GetFilesParams& params,
    std::function<void (const Aria2GetFilesResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(params.gid);

    request(
        RpcMethod::Aria2GetFiles,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2GetFilesResponse{});
        }
    );

};


struct Aria2GetPeersParams {
    QString gid;
};

struct Aria2GetPeersResponse {};

void Aria2c::aria2GetPeers(
    const Aria2GetPeersParams& params,
    std::function<void (const Aria2GetPeersResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(params.gid);

    request(
        RpcMethod::Aria2GetPeers,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2GetPeersResponse{});
        }
    );

};


struct Aria2GetServersParams {
    QString gid;
};

struct Aria2GetServersResponse {};

void Aria2c::aria2GetServers(
    const Aria2GetServersParams& params,
    std::function<void (const Aria2GetServersResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(params.gid);

    request(
        RpcMethod::Aria2GetServers,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2GetServersResponse{});
        }
    );

};


struct Aria2TellActiveResponse {};

void Aria2c::aria2TellActive(
    std::function<void (const Aria2TellActiveResponse&)> cb
) {

    request(
        RpcMethod::Aria2TellActive,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2TellActiveResponse{});
        }
    );

};


struct Aria2TellWaitingParams {
    int offset;
    int num;
    std::optional<QStringList> keys;
};

struct Aria2TellWaitingResponse {};

void Aria2c::aria2TellWaiting(
    const Aria2TellWaitingParams& params,
    std::function<void (const Aria2TellWaitingResponse&)> cb
) {

    QJsonArray qJsonArray;
    qJsonArray.append(params.offset);
    qJsonArray.append(params.num);

    if (params.keys) {
        QJsonArray keysArray;
        for (const QString& key : *params.keys) {
            keysArray.append(key);
        };
        qJsonArray.append(keysArray);
    };

    request(
        RpcMethod::Aria2TellWaiting,
        qJsonArray,
        [ cb = std::move(cb) ](const QJsonValue& value) {
            (void)value;
            cb(Aria2TellWaitingResponse{});
        }
    );

};


struct Aria2TellStoppedParams {
    int offset;
    int num;
    std::optional<QStringList> keys;
};

struct Aria2TellStoppedResponse {};

void Aria2c::aria2TellStopped(
    const Aria2TellStoppedParams& params,
    std::function<void (const Aria2TellStoppedResponse&)> cb
) {

};


struct Aria2ChangePositionParams {

    enum class How {
        Set,
        Cur,
        End,
        Count
    };

    static constexpr unsigned int howToUnsignedInt(const How h) {
        return static_cast<unsigned int>(h);
    };

    static constexpr unsigned int HOW_COUNT =
        static_cast<unsigned int>(How::Count);

    QString gid;
    int pos;
    How how;

    static inline const QString HOW_ENUM_TO_QSTRING_MAP[HOW_COUNT] = {
        "POS_SET",
        "POS_CUR",
        "POS_END"
    };

};

struct Aria2ChangePositionResponse {};

void Aria2c::aria2ChangePosition(
    const Aria2ChangePositionParams& params,
    std::function<void (const Aria2ChangePositionResponse&)> cb
) {

};


struct Aria2ChangeUriParams {
    QString gid;
    int fileIndex;
    QStringList delUris;
    QStringList addUris;
    std::optional<int> position;
};

struct Aria2ChangeUriResponse {};

void Aria2c::aria2ChangeUri(
    const Aria2ChangeUriParams& params,
    std::function<void (const Aria2ChangeUriResponse&)> cb
) {

};


struct Aria2GetOptionParams {
    QString gid;
};

struct Aria2GetOptionResponse {};

void Aria2c::aria2GetOption(
    const Aria2GetOptionParams& params,
    std::function<void (const Aria2GetOptionResponse&)> cb
) {

};


struct Aria2ChangeOptionParams {
    QString gid;
    QJsonObject options;
};

struct Aria2ChangeOptionResponse {};

void Aria2c::aria2ChangeOption(
    const Aria2ChangeOptionParams& params,
    std::function<void (const Aria2ChangeOptionResponse&)> cb
) {

};


struct Aria2GetGlobalOptionResponse {};

void Aria2c::aria2GetGlobalOption(
    std::function<void (const Aria2GetGlobalOptionResponse&)> cb
) {

};


struct Aria2ChangeGlobalOptionParams {
    QJsonObject options;
};

struct Aria2ChangeGlobalOptionResponse {};

void Aria2c::aria2ChangeGlobalOption(
    const Aria2ChangeGlobalOptionParams& params,
    std::function<void (const Aria2ChangeGlobalOptionResponse&)> cb
) {

};


struct Aria2GetGlobalStatResponse {};

void Aria2c::aria2GetGlobalStat(
    std::function<void (const Aria2GetGlobalStatResponse&)> cb
) {

};


struct Aria2PurgeDownloadResultResponse {};


void Aria2c::aria2PurgeDownloadResult(
    std::function<void (const Aria2PurgeDownloadResultResponse&)> cb
) {

};


struct Aria2RemoveDownloadResultParams {
    QString gid;
};

struct Aria2RemoveDownloadResultResponse {};

void Aria2c::aria2RemoveDownloadResult(
    const Aria2RemoveDownloadResultParams& params,
    std::function<void (const Aria2RemoveDownloadResultResponse&)> cb
) {

};


struct Aria2GetSessionInfoResponse {};

void Aria2c::aria2GetSessionInfo(
    std::function<void (const Aria2GetSessionInfoResponse&)> cb
) {

};


struct Aria2ShutdownResponse {};

void Aria2c::aria2Shutdown(
    std::function<void (const Aria2ShutdownResponse&)> cb
) {

};


struct Aria2ForceShutdownResponse {};

void Aria2c::aria2ForceShutdown(
    std::function<void (const Aria2ForceShutdownResponse&)> cb
) {

};


struct Aria2SaveSessionResponse {};

void Aria2c::aria2SaveSession(
    std::function<void (const Aria2SaveSessionResponse&)> cb
) {

};

// One entry in a system.multicall batch. `methodName` is the RPC method
// name (e.g. "aria2.getVersion"); `params` is the already-serialized
// QJsonArray from some Params::toQJsonArray().
struct SystemMulticallMethod {
    QString methodName;
    QJsonArray params;
};

struct SystemMulticallParams {
    QVector<SystemMulticallMethod> methods;
};

struct SystemMulticallResponse {};

void Aria2c::systemMulticall(
    const SystemMulticallParams& params,
    std::function<void (const SystemMulticallResponse&)> cb
) {

};


struct SystemListMethodsResponse {};

void Aria2c::systemListMethods(
    std::function<void (const SystemListMethodsResponse&)> cb
) {

};


struct SystemListNotificationsResponse {};

void Aria2c::systemListNotifications(
    std::function<void (const SystemListNotificationsResponse&)> cb
) {

};
