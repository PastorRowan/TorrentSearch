
#pragma once

#include <QString>
#include <QJsonArray>
#include <QJsonValue>

// ============================================================
// RPC method table
// ============================================================

#define RPC_METHODS \
    X(aria2AddUri,               "aria2.addUri",                Aria2AddUri,                Y) \
    X(aria2AddTorrent,           "aria2.addTorrent",            Aria2AddTorrent,            Y) \
    X(aria2AddMetalink,          "aria2.addMetalink",           Aria2AddMetalink,           Y) \
    X(aria2Remove,               "aria2.remove",                Aria2Remove,                Y) \
    X(aria2ForceRemove,          "aria2.forceRemove",           Aria2ForceRemove,           Y) \
    X(aria2Pause,                "aria2.pause",                 Aria2Pause,                 Y) \
    X(aria2PauseAll,             "aria2.pauseAll",              Aria2PauseAll,              N) \
    X(aria2ForcePause,           "aria2.forcePause",            Aria2ForcePause,            Y) \
    X(aria2ForcePauseAll,        "aria2.forcePauseAll",         Aria2ForcePauseAll,         N) \
    X(aria2Unpause,              "aria2.unpause",               Aria2Unpause,               Y) \
    X(aria2UnpauseAll,           "aria2.unpauseAll",            Aria2UnpauseAll,            N) \
    X(aria2TellStatus,           "aria2.tellStatus",            Aria2TellStatus,            Y) \
    X(aria2GetUris,              "aria2.getUris",               Aria2GetUris,               Y) \
    X(aria2GetFiles,             "aria2.getFiles",              Aria2GetFiles,              Y) \
    X(aria2GetPeers,             "aria2.getPeers",              Aria2GetPeers,              Y) \
    X(aria2GetServers,           "aria2.getServers",            Aria2GetServers,            Y) \
    X(aria2TellActive,           "aria2.tellActive",            Aria2TellActive,            N) \
    X(aria2TellWaiting,          "aria2.tellWaiting",           Aria2TellWaiting,           Y) \
    X(aria2TellStopped,          "aria2.tellStopped",           Aria2TellStopped,           Y) \
    X(aria2ChangePosition,       "aria2.changePosition",        Aria2ChangePosition,        Y) \
    X(aria2ChangeUri,            "aria2.changeUri",             Aria2ChangeUri,             Y) \
    X(aria2GetOption,            "aria2.getOption",             Aria2GetOption,             Y) \
    X(aria2ChangeOption,         "aria2.changeOption",          Aria2ChangeOption,          Y) \
    X(aria2GetGlobalOption,      "aria2.getGlobalOption",       Aria2GetGlobalOption,       N) \
    X(aria2ChangeGlobalOption,   "aria2.changeGlobalOption",    Aria2ChangeGlobalOption,    Y) \
    X(aria2GetGlobalStat,        "aria2.getGlobalStat",         Aria2GetGlobalStat,         N) \
    X(aria2PurgeDownloadResult,  "aria2.purgeDownloadResult",   Aria2PurgeDownloadResult,   N) \
    X(aria2RemoveDownloadResult, "aria2.removeDownloadResult",  Aria2RemoveDownloadResult,  Y) \
    X(aria2GetVersion,           "aria2.getVersion",            Aria2GetVersion,            N) \
    X(aria2GetSessionInfo,       "aria2.getSessionInfo",        Aria2GetSessionInfo,        N) \
    X(aria2Shutdown,             "aria2.shutdown",              Aria2Shutdown,              N) \
    X(aria2ForceShutdown,        "aria2.forceShutdown",         Aria2ForceShutdown,         N) \
    X(aria2SaveSession,          "aria2.saveSession",           Aria2SaveSession,           N) \
    X(systemMulticall,           "system.multicall",            SystemMulticall,            Y) \
    X(systemListMethods,         "system.listMethods",          SystemListMethods,          N) \
    X(systemListNotifications,   "system.listNotifications",    SystemListNotifications,    N)

// ============================================================
// RPC method enum
// ============================================================

#define X(function, method, name, hasParams) name,

enum class RpcMethod {
    RPC_METHODS
    Count
};

#undef X

// RPC method count
const unsigned int RPC_METHOD_COUNT = static_cast<unsigned int>(RpcMethod::Count);

unsigned int rpcMethodToUnsignedInt(
    const RpcMethod method
);


// ============================================================
// RPC method names
// ============================================================

#define X(function, method, name, hasParams) method,

const QString rpcMethodNames[RPC_METHOD_COUNT] = {
    RPC_METHODS
};

#undef X

QString rpcMethodEnumToRpcMethodName(
    const RpcMethod method
);
