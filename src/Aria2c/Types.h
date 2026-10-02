#pragma once

#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QStringList>
#include <QVector>
#include <optional>

// ================================================================
// Aria2Error
// ================================================================

struct Aria2Error {
    int code = 0;
    QString message;
    enum class Source { Rpc, Transport, Parse } source = Source::Rpc;
};


// ================================================================
// Shared helper structs for nested response objects
// ================================================================

// A single URI as returned by aria2.getUris() and nested inside
// file objects in aria2.getFiles() and aria2.tellStatus().
struct Aria2Uri {
    QString uri;       // URI
    QString status;    // "used" or "waiting"

    static Aria2Uri fromQJsonObject(const QJsonObject& obj);
};

// A single file as returned by aria2.getFiles() and nested inside
// aria2.tellStatus().
struct Aria2File {
    QString index;                 // Index of the file, starting at 1
    QString path;                  // File path
    QString length;                // File size in bytes
    QString completedLength;       // Completed length of this file in bytes
    QString selected;              // "true" if selected by --select-file
    QVector<Aria2Uri> uris;        // List of URIs for this file

    static Aria2File fromQJsonObject(const QJsonObject& obj);
};

// A single peer as returned by aria2.getPeers().
struct Aria2Peer {
    QString peerId;           // Percent-encoded peer ID
    QString ip;               // IP address of the peer
    QString port;             // Port number of the peer
    QString bitfield;         // Hexadecimal representation of the download progress
    QString amChoking;        // "true" if aria2 is choking the peer
    QString peerChoking;      // "true" if the peer is choking aria2
    QString downloadSpeed;    // Download speed (byte/sec) that this client obtains from the peer
    QString uploadSpeed;      // Upload speed (byte/sec) that this client uploads to the peer
    QString seeder;           // "true" if this is a seeder

    static Aria2Peer fromQJsonObject(const QJsonObject& obj);
};

// A single server entry nested inside aria2.getServers().
struct Aria2Server {
    QString uri;               // URI originally added
    QString currentUri;        // URI currently used for downloading
    QString downloadSpeed;     // Download speed (byte/sec)

    static Aria2Server fromQJsonObject(const QJsonObject& obj);
};

// A single file's server list as returned by aria2.getServers().
struct Aria2FileServers {
    QString index;                    // Index of the file, starting at 1
    QVector<Aria2Server> servers;     // List of servers

    static Aria2FileServers fromQJsonObject(const QJsonObject& obj);
};

// BitTorrent metadata nested inside aria2.tellStatus().
struct Aria2BittorrentInfo {
    QString name;                     // Name in info dictionary
    QString comment;                  // Comment of the torrent
    QString creationDate;             // Creation time (integer since epoch)
    QString mode;                     // "single" or "multi"
    QVector<QVector<QString>> announceList;  // List of lists of announce URIs

    static Aria2BittorrentInfo fromQJsonObject(const QJsonObject& obj);
};


// ================================================================
// Aria2AddUri
// ================================================================

struct Aria2AddUriParams {
    QStringList uris;
    struct Tail1 {
        QJsonObject options;
        std::optional<int> position;
    };
    std::optional<Tail1> tail1;

    QJsonArray toQJsonArray() const;
};

struct Aria2AddUriResponse {
    QString gid;    // GID of the newly registered download

    static Aria2AddUriResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2AddTorrent
// ================================================================

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

    QJsonArray toQJsonArray() const;
};

struct Aria2AddTorrentResponse {
    QString gid;    // GID of the newly registered download

    static Aria2AddTorrentResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2AddMetalink
// ================================================================

struct Aria2AddMetalinkParams {
    QByteArray metalink;
    struct Tail1 {
        QJsonObject options;
        std::optional<int> position;
    };
    std::optional<Tail1> tail1;

    QJsonArray toQJsonArray() const;
};

struct Aria2AddMetalinkResponse {
    // Returns an array of GIDs for each download described in the metalink.
    QStringList gids;

    static Aria2AddMetalinkResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2Remove
// ================================================================

struct Aria2RemoveParams {
    QString gid;

    QJsonArray toQJsonArray() const;
};

struct Aria2RemoveResponse {
    QString gid;    // GID of the removed download

    static Aria2RemoveResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2ForceRemove
// ================================================================

struct Aria2ForceRemoveParams {
    QString gid;

    QJsonArray toQJsonArray() const;
};

struct Aria2ForceRemoveResponse {
    QString gid;    // GID of the removed download

    static Aria2ForceRemoveResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2Pause
// ================================================================

struct Aria2PauseParams {
    QString gid;

    QJsonArray toQJsonArray() const;
};

struct Aria2PauseResponse {
    QString gid;    // GID of the paused download

    static Aria2PauseResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2PauseAll
// ================================================================

struct Aria2PauseAllResponse {
    // Returns "OK" on success. We model it as a bool.
    bool ok = false;

    static Aria2PauseAllResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2ForcePause
// ================================================================

struct Aria2ForcePauseParams {
    QString gid;

    QJsonArray toQJsonArray() const;
};

struct Aria2ForcePauseResponse {
    QString gid;    // GID of the paused download

    static Aria2ForcePauseResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2ForcePauseAll
// ================================================================

struct Aria2ForcePauseAllResponse {
    bool ok = false;

    static Aria2ForcePauseAllResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2Unpause
// ================================================================

struct Aria2UnpauseParams {
    QString gid;

    QJsonArray toQJsonArray() const;
};

struct Aria2UnpauseResponse {
    QString gid;    // GID of the unpaused download

    static Aria2UnpauseResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2UnpauseAll
// ================================================================

struct Aria2UnpauseAllResponse {
    bool ok = false;

    static Aria2UnpauseAllResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2TellStatus
// ================================================================

struct Aria2TellStatusParams {
    QString gid;
    std::optional<QStringList> keys;

    QJsonArray toQJsonArray() const;
};

struct Aria2TellStatusResponse {
    QString gid;                    // GID of the download
    QString status;                 // active, waiting, paused, error, complete, removed
    QString totalLength;            // Total length in bytes
    QString completedLength;        // Completed length in bytes
    QString uploadLength;           // Uploaded length in bytes
    QString bitfield;               // Hex representation of progress (absent if not started)
    QString downloadSpeed;          // Download speed (byte/sec)
    QString uploadSpeed;            // Upload speed (byte/sec)
    QString infoHash;               // InfoHash. BitTorrent only.
    QString numSeeders;             // Number of seeders connected. BitTorrent only.
    QString seeder;                 // "true" if local endpoint is a seeder. BitTorrent only.
    QString pieceLength;            // Piece length in bytes
    QString numPieces;              // Number of pieces
    QString connections;            // Number of peers/servers connected
    QString errorCode;              // Last error code (stopped/completed only)
    QString errorMessage;           // Human readable error message
    QStringList followedBy;         // GIDs generated as result of this download
    QString following;              // Reverse link for followedBy
    QString belongsTo;              // GID of a parent download
    QString dir;                    // Directory to save files
    QVector<Aria2File> files;       // List of files
    std::optional<Aria2BittorrentInfo> bittorrent;  // BitTorrent metadata
    QString verifiedLength;         // Verified bytes (only during hash check)
    QString verifyIntegrityPending; // "true" if waiting for hash check in queue

    static Aria2TellStatusResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2GetUris
// ================================================================

struct Aria2GetUrisParams {
    QString gid;

    QJsonArray toQJsonArray() const;
};

struct Aria2GetUrisResponse {
    QVector<Aria2Uri> uris;    // List of URI objects

    static Aria2GetUrisResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2GetFiles
// ================================================================

struct Aria2GetFilesParams {
    QString gid;

    QJsonArray toQJsonArray() const;
};

struct Aria2GetFilesResponse {
    QVector<Aria2File> files;    // List of file objects

    static Aria2GetFilesResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2GetPeers
// ================================================================

struct Aria2GetPeersParams {
    QString gid;

    QJsonArray toQJsonArray() const;
};

struct Aria2GetPeersResponse {
    QVector<Aria2Peer> peers;    // List of peer objects

    static Aria2GetPeersResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2GetServers
// ================================================================

struct Aria2GetServersParams {
    QString gid;

    QJsonArray toQJsonArray() const;
};

struct Aria2GetServersResponse {
    QVector<Aria2FileServers> fileServers;    // List of file-server groups

    static Aria2GetServersResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2TellActive
// ================================================================

struct Aria2TellActiveResponse {
    QVector<Aria2TellStatusResponse> downloads;    // Same struct as tellStatus

    static Aria2TellActiveResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2TellWaiting
// ================================================================

struct Aria2TellWaitingParams {
    int offset;
    int num;
    std::optional<QStringList> keys;

    QJsonArray toQJsonArray() const;
};

struct Aria2TellWaitingResponse {
    QVector<Aria2TellStatusResponse> downloads;    // Same struct as tellStatus

    static Aria2TellWaitingResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2TellStopped
// ================================================================

struct Aria2TellStoppedParams {
    int offset;
    int num;
    std::optional<QStringList> keys;

    QJsonArray toQJsonArray() const;
};

struct Aria2TellStoppedResponse {
    QVector<Aria2TellStatusResponse> downloads;    // Same struct as tellStatus

    static Aria2TellStoppedResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2ChangePosition
// ================================================================

struct Aria2ChangePositionParams {

    enum class How {
        Set,
        Cur,
        End,
        Count
    };

    static unsigned int howToUnsignedInt(const How h);

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

    QJsonArray toQJsonArray() const;
};

struct Aria2ChangePositionResponse {
    int index = 0;    // Index of the file that was moved (0 if not applicable)

    static Aria2ChangePositionResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2ChangeUri
// ================================================================

struct Aria2ChangeUriParams {
    QString gid;
    int fileIndex;
    QStringList delUris;
    QStringList addUris;
    std::optional<int> position;

    QJsonArray toQJsonArray() const;
};

struct Aria2ChangeUriResponse {
    int index = 0;               // Index of the file that was changed
    QStringList uris;            // URIs that were added to the file

    static Aria2ChangeUriResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2GetOption
// ================================================================

struct Aria2GetOptionParams {
    QString gid;

    QJsonArray toQJsonArray() const;
};

struct Aria2GetOptionResponse {
    QJsonObject options;    // Key-value pairs of the current options

    static Aria2GetOptionResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2ChangeOption
// ================================================================

struct Aria2ChangeOptionParams {
    QString gid;
    QJsonObject options;

    QJsonArray toQJsonArray() const;
};

struct Aria2ChangeOptionResponse {
    bool ok = false;

    static Aria2ChangeOptionResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2GetGlobalOption
// ================================================================

struct Aria2GetGlobalOptionResponse {
    QJsonObject options;    // Key-value pairs of the global options

    static Aria2GetGlobalOptionResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2ChangeGlobalOption
// ================================================================

struct Aria2ChangeGlobalOptionParams {
    QJsonObject options;

    QJsonArray toQJsonArray() const;
};

struct Aria2ChangeGlobalOptionResponse {
    bool ok = false;

    static Aria2ChangeGlobalOptionResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2GetGlobalStat
// ================================================================

struct Aria2GetGlobalStatResponse {
    QString downloadSpeed;     // Overall download speed (byte/sec)
    QString uploadSpeed;       // Overall upload speed (byte/sec)
    QString numActive;         // Number of active downloads
    QString numWaiting;        // Number of waiting downloads
    QString numStopped;        // Number of stopped downloads

    static Aria2GetGlobalStatResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2PurgeDownloadResult
// ================================================================

struct Aria2PurgeDownloadResultResponse {
    bool ok = false;

    static Aria2PurgeDownloadResultResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2RemoveDownloadResult
// ================================================================

struct Aria2RemoveDownloadResultParams {
    QString gid;

    QJsonArray toQJsonArray() const;
};

struct Aria2RemoveDownloadResultResponse {
    bool ok = false;

    static Aria2RemoveDownloadResultResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2GetVersion
// ================================================================

struct Aria2GetVersionResponse {
    QString version;                  // Version number of aria2
    QStringList enabledFeatures;      // List of enabled features

    static Aria2GetVersionResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2GetSessionInfo
// ================================================================

struct Aria2GetSessionInfoResponse {
    QString sessionId;    // Session ID generated each time aria2 is invoked

    static Aria2GetSessionInfoResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2Shutdown
// ================================================================

struct Aria2ShutdownResponse {
    bool ok = false;

    static Aria2ShutdownResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2ForceShutdown
// ================================================================

struct Aria2ForceShutdownResponse {
    bool ok = false;

    static Aria2ForceShutdownResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// Aria2SaveSession
// ================================================================

struct Aria2SaveSessionResponse {
    bool ok = false;

    static Aria2SaveSessionResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// SystemMulticall
// ================================================================

// One entry in a system.multicall batch. `methodName` is the RPC method
// name (e.g. "aria2.getVersion"); `params` is the already-serialized
// QJsonArray from some Params::toQJsonArray().
struct SystemMulticallMethod {
    QString methodName;
    QJsonArray params;
};

struct SystemMulticallParams {
    QVector<SystemMulticallMethod> methods;

    QJsonArray toQJsonArray() const;
};

struct SystemMulticallResponse {
    // aria2 returns an array of one-item arrays. Each element is either:
    //   - [ <return value> ]                on success
    //   - { "fault": { "code": N, "message": M } }  on per-call failure
    //
    // We store the raw QJsonValue for each result and provide helpers
    // to check success and extract values.
    struct Entry {
        bool isFault = false;
        QJsonValue value;           // Success value (or fault object)
        int faultCode = 0;
        QString faultMessage;

        bool isSuccess() const;

        QJsonValue resultValue() const;
    };

    QVector<Entry> entries;

    static SystemMulticallResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// SystemListMethods
// ================================================================

struct SystemListMethodsResponse {
    QStringList methods;    // List of all available RPC method names

    static SystemListMethodsResponse fromQJsonValue(const QJsonValue& value);
};


// ================================================================
// SystemListNotifications
// ================================================================

struct SystemListNotificationsResponse {
    QStringList notifications;    // List of all available notification names

    static SystemListNotificationsResponse fromQJsonValue(const QJsonValue& value);
};
