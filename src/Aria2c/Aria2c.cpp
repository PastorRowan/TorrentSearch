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

#define DEFINE_RPC_METHOD_Y(function, name) \
    void Aria2c::function( \
        const name##Params& params, \
        std::function<void(const name##Response&)> cb \
    ) { \
        request( \
            RpcMethod::name, \
            params.toQJsonArray(), \
            [ cb = std::move(cb) ](const QJsonValue& value) { \
                cb(name##Response::fromQJsonValue(value)); \
            } \
        ); \
    }


#define DEFINE_RPC_METHOD_N(function, name) \
    void Aria2c::function( \
        std::function<void(const name##Response&)> cb \
    ) { \
        request( \
            RpcMethod::name, \
            QJsonArray{}, \
            [ cb = std::move(cb) ](const QJsonValue& value) { \
                cb(name##Response::fromQJsonValue(value)); \
            } \
        ); \
    }


#define DEFINE_RPC_METHOD(function, name, hasParams) \
    DEFINE_RPC_METHOD_##hasParams(function, name)

#define X(function, method, name, hasParams) \
    DEFINE_RPC_METHOD(function, name, hasParams)

void Aria2c::aria2AddUri(
    const Aria2AddUriParams& params,
    std::function<void (const Aria2AddUriResponse&)> cb
) {
    request(
        RpcMethod::Aria2AddUri,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2AddUriResponse::fromQJsonValue(value));
        }
    );
};


void Aria2c::aria2AddTorrent(
    const Aria2AddTorrentParams& params,
    std::function<void (const Aria2AddTorrentResponse&)> cb
) {
    request(
        RpcMethod::Aria2AddTorrent,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2AddTorrentResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2AddMetalink(
    const Aria2AddMetalinkParams& params,
    std::function<void (const Aria2AddMetalinkResponse&)> cb
) {
    request(
        RpcMethod::Aria2AddMetalink,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2AddMetalinkResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2Remove(
    const Aria2RemoveParams& params,
    std::function<void (const Aria2RemoveResponse&)> cb
) {
    request(
        RpcMethod::Aria2Remove,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2RemoveResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2ForceRemove(
    const Aria2ForceRemoveParams& params,
    std::function<void (const Aria2ForceRemoveResponse&)> cb
) {
    request(
        RpcMethod::Aria2ForceRemove,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2ForceRemoveResponse::fromQJsonValue(value));
        }
    );
};


// ================================================================
// Aria2Pause
// ================================================================

struct Aria2PauseParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    };
};

struct Aria2PauseResponse {
    QString gid;    // GID of the paused download

    static Aria2PauseResponse fromQJsonValue(const QJsonValue& value) {
        Aria2PauseResponse result;
        result.gid = value.toString();
        return result;
    };
};

void Aria2c::aria2Pause(
    const Aria2PauseParams& params,
    std::function<void (const Aria2PauseResponse&)> cb
) {
    request(
        RpcMethod::Aria2Pause,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2PauseResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2PauseAll(
    std::function<void (const Aria2PauseAllResponse&)> cb
) {
    request(
        RpcMethod::Aria2PauseAll,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2PauseAllResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2ForcePause(
    const Aria2ForcePauseParams& params,
    std::function<void (const Aria2ForcePauseResponse&)> cb
) {
    request(
        RpcMethod::Aria2ForcePause,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2ForcePauseResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2ForcePauseAll(
    std::function<void (const Aria2ForcePauseAllResponse&)> cb
) {
    request(
        RpcMethod::Aria2ForcePauseAll,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2ForcePauseAllResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2Unpause(
    const Aria2UnpauseParams& params,
    std::function<void (const Aria2UnpauseResponse&)> cb
) {
    request(
        RpcMethod::Aria2Unpause,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2UnpauseResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2UnpauseAll(
    std::function<void (const Aria2UnpauseAllResponse&)> cb
) {
    request(
        RpcMethod::Aria2UnpauseAll,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2UnpauseAllResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2TellStatus(
    const Aria2TellStatusParams& params,
    std::function<void (const Aria2TellStatusResponse&)> cb
) {
    request(
        RpcMethod::Aria2TellStatus,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2TellStatusResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2GetUris(
    const Aria2GetUrisParams& params,
    std::function<void (const Aria2GetUrisResponse&)> cb
) {
    request(
        RpcMethod::Aria2GetUris,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2GetUrisResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2GetFiles(
    const Aria2GetFilesParams& params,
    std::function<void (const Aria2GetFilesResponse&)> cb
) {
    request(
        RpcMethod::Aria2GetFiles,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2GetFilesResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2GetPeers(
    const Aria2GetPeersParams& params,
    std::function<void (const Aria2GetPeersResponse&)> cb
) {
    request(
        RpcMethod::Aria2GetPeers,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2GetPeersResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2GetServers(
    const Aria2GetServersParams& params,
    std::function<void (const Aria2GetServersResponse&)> cb
) {
    request(
        RpcMethod::Aria2GetServers,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2GetServersResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2TellActive(
    std::function<void (const Aria2TellActiveResponse&)> cb
) {
    request(
        RpcMethod::Aria2TellActive,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2TellActiveResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2TellWaiting(
    const Aria2TellWaitingParams& params,
    std::function<void (const Aria2TellWaitingResponse&)> cb
) {
    request(
        RpcMethod::Aria2TellWaiting,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2TellWaitingResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2TellStopped(
    const Aria2TellStoppedParams& params,
    std::function<void (const Aria2TellStoppedResponse&)> cb
) {
    request(
        RpcMethod::Aria2TellStopped,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2TellStoppedResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2ChangePosition(
    const Aria2ChangePositionParams& params,
    std::function<void (const Aria2ChangePositionResponse&)> cb
) {
    request(
        RpcMethod::Aria2ChangePosition,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2ChangePositionResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2ChangeUri(
    const Aria2ChangeUriParams& params,
    std::function<void (const Aria2ChangeUriResponse&)> cb
) {
    request(
        RpcMethod::Aria2ChangeUri,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2ChangeUriResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2GetOption(
    const Aria2GetOptionParams& params,
    std::function<void (const Aria2GetOptionResponse&)> cb
) {
    request(
        RpcMethod::Aria2GetOption,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2GetOptionResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2ChangeOption(
    const Aria2ChangeOptionParams& params,
    std::function<void (const Aria2ChangeOptionResponse&)> cb
) {
    request(
        RpcMethod::Aria2ChangeOption,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2ChangeOptionResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2GetGlobalOption(
    std::function<void (const Aria2GetGlobalOptionResponse&)> cb
) {
    request(
        RpcMethod::Aria2GetGlobalOption,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2GetGlobalOptionResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2ChangeGlobalOption(
    const Aria2ChangeGlobalOptionParams& params,
    std::function<void (const Aria2ChangeGlobalOptionResponse&)> cb
) {
    request(
        RpcMethod::Aria2ChangeGlobalOption,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2ChangeGlobalOptionResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2GetGlobalStat(
    std::function<void (const Aria2GetGlobalStatResponse&)> cb
) {
    request(
        RpcMethod::Aria2GetGlobalStat,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2GetGlobalStatResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2PurgeDownloadResult(
    std::function<void (const Aria2PurgeDownloadResultResponse&)> cb
) {
    request(
        RpcMethod::Aria2PurgeDownloadResult,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2PurgeDownloadResultResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2RemoveDownloadResult(
    const Aria2RemoveDownloadResultParams& params,
    std::function<void (const Aria2RemoveDownloadResultResponse&)> cb
) {
    request(
        RpcMethod::Aria2RemoveDownloadResult,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2RemoveDownloadResultResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2GetVersion(
    std::function<void(const Aria2GetVersionResponse&)> cb
) {
    request(
        RpcMethod::Aria2GetVersion,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2GetVersionResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2GetSessionInfo(
    std::function<void (const Aria2GetSessionInfoResponse&)> cb
) {
    request(
        RpcMethod::Aria2GetSessionInfo,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2GetSessionInfoResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2Shutdown(
    std::function<void (const Aria2ShutdownResponse&)> cb
) {
    request(
        RpcMethod::Aria2Shutdown,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2ShutdownResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2ForceShutdown(
    std::function<void (const Aria2ForceShutdownResponse&)> cb
) {
    request(
        RpcMethod::Aria2ForceShutdown,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2ForceShutdownResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::aria2SaveSession(
    std::function<void (const Aria2SaveSessionResponse&)> cb
) {
    request(
        RpcMethod::Aria2SaveSession,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(Aria2SaveSessionResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::systemMulticall(
    const SystemMulticallParams& params,
    std::function<void (const SystemMulticallResponse&)> cb
) {
    request(
        RpcMethod::SystemMulticall,
        params.toQJsonArray(),
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(SystemMulticallResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::systemListMethods(
    std::function<void (const SystemListMethodsResponse&)> cb
) {
    request(
        RpcMethod::SystemListMethods,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(SystemListMethodsResponse::fromQJsonValue(value));
        }
    );
};

void Aria2c::systemListNotifications(
    std::function<void (const SystemListNotificationsResponse&)> cb
) {
    request(
        RpcMethod::SystemListNotifications,
        QJsonArray{},
        [ cb = std::move(cb) ](const QJsonValue& value) {
            cb(SystemListNotificationsResponse::fromQJsonValue(value));
        }
    );
};
