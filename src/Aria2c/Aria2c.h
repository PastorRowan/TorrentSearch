
#pragma once

#include <concepts>
#include <functional>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QString>
#include <concepts>
#include <functional>

struct NoParams {
    QJsonArray toQJsonArray() const {
        return {};
    };
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

struct PauseAllResponse {
    static PauseAllResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return PauseAllResponse{};
    };
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

struct ForcePauseAllResponse {
    static ForcePauseAllResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ForcePauseAllResponse{};
    };
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

struct UnpauseAllResponse {
    static UnpauseAllResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return UnpauseAllResponse{};
    };
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

struct GetUrisParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    };
};

struct GetUrisResponse {
    static GetUrisResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return GetUrisResponse{};
    };
};

struct GetFilesParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    };
};

struct GetFilesResponse {
    static GetFilesResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return GetFilesResponse{};
    };
};

struct GetPeersParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    };
};

struct GetPeersResponse {
    static GetPeersResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return GetPeersResponse{};
    };
};

struct GetServersParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    };
};

struct GetServersResponse {
    static GetServersResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return GetServersResponse{};
    };
};

struct TellActiveResponse {
    static TellActiveResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return TellActiveResponse{};
    };
};

struct TellWaitingParams {

    int offset;
    int num;

    // Single trailing optional; no positional coupling.
    std::optional<QStringList> keys;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(offset);
        array.append(num);
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

struct TellWaitingResponse {
    static TellWaitingResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return TellWaitingResponse{};
    };
};

struct TellStoppedParams {

    int offset;
    int num;

    // Single trailing optional; no positional coupling.
    std::optional<QStringList> keys;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(offset);
        array.append(num);
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

struct TellStoppedResponse {
    static TellStoppedResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return TellStoppedResponse{};
    };
};

struct ChangePositionParams {

    enum class How {
        Set,
        Cur,
        End,
        Count
    };

    static constexpr const unsigned int howToUnsignedInt(const How howP) {
        return static_cast<unsigned int>(howP);
    };

    static constexpr unsigned int HOW_COUNT = static_cast<unsigned int>(How::Count);

    QString gid;
    int pos;
    How how;

    static inline const QString HOW_ENUM_TO_QSTRING_MAP[HOW_COUNT] = {
        "POS_SET",
        "POS_CUR",
        "POS_END"
    };

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        array.append(pos);
        array.append(HOW_ENUM_TO_QSTRING_MAP[howToUnsignedInt(how)]);
        return array;
    };

};

struct ChangePositionResponse {
    static ChangePositionResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ChangePositionResponse{};
    };
};

struct ChangeUriParams {

    QString gid;
    int fileIndex;
    QStringList delUris;
    QStringList addUris;

    // Single trailing optional; no positional coupling.
    std::optional<int> position;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        array.append(fileIndex);

        QJsonArray delUrisArray;
        for (const QString& uri : delUris) {
            delUrisArray.append(uri);
        };
        array.append(delUrisArray);

        QJsonArray addUrisArray;
        for (const QString& uri : addUris) {
            addUrisArray.append(uri);
        };
        array.append(addUrisArray);

        if (position) {
            array.append(*position);
        };
        return array;
    };

};

struct ChangeUriResponse {
    static ChangeUriResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ChangeUriResponse{};
    };
};

struct GetOptionParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    };
};

struct GetOptionResponse {
    static GetOptionResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return GetOptionResponse{};
    };
};

struct ChangeOptionParams {
    QString gid;
    QJsonObject options;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        array.append(options);
        return array;
    };
};

struct ChangeOptionResponse {
    static ChangeOptionResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ChangeOptionResponse{};
    };
};

struct GetGlobalOptionResponse {
    static GetGlobalOptionResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return GetGlobalOptionResponse{};
    };
};

struct ChangeGlobalOptionParams {
    QJsonObject options;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(options);
        return array;
    };
};

struct ChangeGlobalOptionResponse {
    static ChangeGlobalOptionResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ChangeGlobalOptionResponse{};
    };
};

struct GetGlobalStatResponse {
    static GetGlobalStatResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return GetGlobalStatResponse{};
    };
};

struct PurgeDownloadResultResponse {
    static PurgeDownloadResultResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return PurgeDownloadResultResponse{};
    };
};

struct RemoveDownloadResultParams {
    QString gid;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        array.append(gid);
        return array;
    };
};

struct RemoveDownloadResultResponse {
    static RemoveDownloadResultResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return RemoveDownloadResultResponse{};
    };
};

struct GetVersionResponse {
    static GetVersionResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return GetVersionResponse{};
    };
};

struct GetSessionInfoResponse {
    static GetSessionInfoResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return GetSessionInfoResponse{};
    };
};

struct ShutdownResponse {
    static ShutdownResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ShutdownResponse{};
    };
};

struct ForceShutdownResponse {
    static ForceShutdownResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ForceShutdownResponse{};
    };
};

struct SaveSessionResponse {
    static SaveSessionResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return SaveSessionResponse{};
    };
};

// One entry in a system.multicall batch. `methodName` is the RPC method
// name (e.g. "aria2.getVersion"); `params` is the already-serialized
// QJsonArray from some Params::toQJsonArray().
struct MulticallMethod {
    QString methodName;
    QJsonArray params;
};

struct MulticallParams {

    QVector<MulticallMethod> methods;

    QJsonArray toQJsonArray() const {
        QJsonArray array;
        for (const MulticallMethod& method : methods) {
            QJsonObject entry;
            entry["methodName"] = method.methodName;
            entry["params"] = method.params;
            array.append(entry);
        };
        return array;
    };
};

struct MulticallResponse {
    static MulticallResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return MulticallResponse{};
    };
};

struct ListMethodsResponse {
    static ListMethodsResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ListMethodsResponse{};
    };
};

struct ListNotificationsResponse {
    static ListNotificationsResponse fromQJsonValue(const QJsonValue& qJsonValue) {
        return ListNotificationsResponse{};
    };
};

// RPC methods macro used to create stuff
#define RPC_METHODS \
    X(aria2AddUri,              "aria2.addUri",              AddUriParams,               AddUriResponse) \
    X(aria2AddTorrent,          "aria2.addTorrent",          AddTorrentParams,           AddTorrentResponse) \
    X(aria2AddMetalink,         "aria2.addMetalink",         AddMetalinkParams,          AddMetalinkResponse) \
    X(aria2Remove,              "aria2.remove",              RemoveParams,               RemoveResponse) \
    X(aria2ForceRemove,         "aria2.forceRemove",         ForceRemoveParams,          ForceRemoveResponse) \
    X(aria2Pause,               "aria2.pause",               PauseParams,                PauseResponse) \
    X(aria2PauseAll,            "aria2.pauseAll",            NoParams,                   PauseAllResponse) \
    X(aria2ForcePause,          "aria2.forcePause",          ForcePauseParams,           ForcePauseResponse) \
    X(aria2ForcePauseAll,       "aria2.forcePauseAll",       NoParams,                   ForcePauseAllResponse) \
    X(aria2Unpause,             "aria2.unpause",             UnpauseParams,              UnpauseResponse) \
    X(aria2UnpauseAll,          "aria2.unpauseAll",          NoParams,                   UnpauseAllResponse) \
    X(aria2TellStatus,          "aria2.tellStatus",          TellStatusParams,           TellStatusResponse) \
    X(aria2GetUris,             "aria2.getUris",             GetUrisParams,              GetUrisResponse) \
    X(aria2GetFiles,            "aria2.getFiles",            GetFilesParams,             GetFilesResponse) \
    X(aria2GetPeers,            "aria2.getPeers",            GetPeersParams,             GetPeersResponse) \
    X(aria2GetServers,          "aria2.getServers",          GetServersParams,           GetServersResponse) \
    X(aria2TellActive,          "aria2.tellActive",          NoParams,                   TellActiveResponse) \
    X(aria2TellWaiting,         "aria2.tellWaiting",         TellWaitingParams,          TellWaitingResponse) \
    X(aria2TellStopped,         "aria2.tellStopped",         TellStoppedParams,          TellStoppedResponse) \
    X(aria2ChangePosition,      "aria2.changePosition",      ChangePositionParams,       ChangePositionResponse) \
    X(aria2ChangeUri,           "aria2.changeUri",           ChangeUriParams,            ChangeUriResponse) \
    X(aria2GetOption,           "aria2.getOption",           GetOptionParams,            GetOptionResponse) \
    X(aria2ChangeOption,        "aria2.changeOption",        ChangeOptionParams,         ChangeOptionResponse) \
    X(aria2GetGlobalOption,     "aria2.getGlobalOption",     NoParams,                   GetGlobalOptionResponse) \
    X(aria2ChangeGlobalOption,  "aria2.changeGlobalOption",  ChangeGlobalOptionParams,   ChangeGlobalOptionResponse) \
    X(aria2GetGlobalStat,       "aria2.getGlobalStat",       NoParams,                   GetGlobalStatResponse) \
    X(aria2PurgeDownloadResult, "aria2.purgeDownloadResult", NoParams,                   PurgeDownloadResultResponse) \
    X(aria2RemoveDownloadResult,"aria2.removeDownloadResult",RemoveDownloadResultParams, RemoveDownloadResultResponse) \
    X(aria2GetVersion,          "aria2.getVersion",           NoParams,                  GetVersionResponse) \
    X(aria2GetSessionInfo,      "aria2.getSessionInfo",       NoParams,                  GetSessionInfoResponse) \
    X(aria2Shutdown,            "aria2.shutdown",             NoParams,                  ShutdownResponse) \
    X(aria2ForceShutdown,       "aria2.forceShutdown",        NoParams,                  ForceShutdownResponse) \
    X(aria2SaveSession,         "aria2.saveSession",          NoParams,                  SaveSessionResponse) \
    X(systemMulticall,          "system.multicall",           MulticallParams,           MulticallResponse) \
    X(systemListMethods,        "system.listMethods",         NoParams,                  ListMethodsResponse) \
    X(systemListNotifications,  "system.listNotifications",   NoParams,                  ListNotificationsResponse)

// RPC method enum
#define X(method, name, params, response) method,

enum class RpcMethod {
    RPC_METHODS
    Count
};

#undef X
//

// RPC method count
const unsigned int RPC_METHOD_COUNT = static_cast<unsigned int>(RpcMethod::Count);

unsigned int RpcMethodToUnsignedInt(
    const RpcMethod method
) {
    return static_cast<unsigned int>(method);
};

// RPC method names
#define X(method, name, params, response) name,

const QString rpcMethodNames[RPC_METHOD_COUNT] = {
    RPC_METHODS
};

#undef X


QString rpcMethodEnumToRpcMethodName(
    const RpcMethod method
) {
    return rpcMethodNames[RpcMethodToUnsignedInt(method)];
};


// RPC method parameters concept
// Checks whether each RPC method parameter has toQJsonArray
template<typename T>
concept RpcParams = requires(const T& params) {
    { params.toQJsonArray() } -> std::same_as<QJsonArray>;
};


// Ensures all RPC method parameters have toQJsonArray method
#define X(method, name, params, response) \
    static_assert(RpcParams<params>, #params " must have toQJsonArray() returning QJsonArray");

RPC_METHODS

#undef X

// RPC method response concept
// Checks whether each RPC method response has fromQJsonValue
template<typename T>
concept RpcResponses = requires(const QJsonValue& qJsonValue) {
    { T::fromQJsonValue(qJsonValue) } -> std::same_as<T>;
};

// Ensures all RPC method responses have fromQJsonValue method
#define X(method, name, params, response) \
    static_assert(RpcResponses<response>, #response " must have fromQJsonValue(...) returning itself");

RPC_METHODS

#undef X

// RPC method binding parameters to response
template<RpcMethod Method>
struct RpcMethodTraits;

#define X(method, name, params, response) \
    template<> \
    struct RpcMethodTraits<RpcMethod::method> { \
        using Params = params; \
        using Response = response; \
    };

RPC_METHODS

#undef X

class Aria2c : QObject {

    Q_OBJECT

    private:

        QProcess aria2Process;

        QNetworkAccessManager networkAccessManager;

        QString rpcUrl = "http://127.0.0.1:6800/jsonrpc";

        // empty = no token auth (aria2c --rpc-secret not set)
        QString secret;

    protected:

    public:

        explicit Aria2c(
            QObject* parent = nullptr
        );

        void setSecret(const QString& s) {
            secret = s;
        };

        void setRpcUrl(const QString& u) {
            rpcUrl = u;
        };

        void request(
            const RpcMethod method,
            const QJsonArray& params,
            std::function<void(const QJsonValue&)> callback
        );

        // Named public API — generated from the table.
        #define X(method, name, params, response) \
            void method( \
                const params& p, \
                std::function<void(const response&)> cb \
            ) { \
                request( \
                    RpcMethod::method, \
                    p.toQJsonArray(), \
                    [ cb ](const QJsonValue& v) { \
                        cb(response::fromQJsonValue(v)); \
                    }); \
            }

        RPC_METHODS

        #undef X

};
