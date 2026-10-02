
#include "conversions/conversions.h"

/*
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
*/

/*
struct TorrentDownloadData {

    enum class Status {
        QueuedStatus,
        DownloadingStatus,
        PausedStatus,
        CompletedStatus,
        CancelledStatus,
        ErrorStatus
    };
    Q_ENUM(Status)

    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString infoHash MEMBER infoHash)
    Q_PROPERTY(int leechers MEMBER leechers)
    Q_PROPERTY(int seeders MEMBER seeders)
    Q_PROPERTY(long long sizeBytes MEMBER sizeBytes)
    Q_PROPERTY(int numberOfFiles MEMBER numberOfFiles)
    Q_PROPERTY(QString magnetUrl MEMBER magnetUrl)
    Q_PROPERTY(QString torrentUrl MEMBER torrentUrl)
    Q_PROPERTY(QString gid MEMBER gid)
    Q_PROPERTY(Status status MEMBER status)
    Q_PROPERTY(float progress MEMBER progress)

    private:

    protected:

    public:

        QString name = "";
        QString infoHash = "";
        int leechers = 0;
        int seeders = 0;
        long long sizeBytes = 0;
        int numberOfFiles = 0;
        QString magnetUrl = "";
        QString torrentUrl = "";

        QString gid;
        Status status = Status::QueuedStatus;
        float progress = 0.0f;

        QString statusToQString() const;

        QString toQString() const;

        friend bool operator==(
            const TorrentDownloadData& leftData,
            const TorrentDownloadData& rightData
        );

};
*/

TorrentDownloadData conversions::aria2TellStatusResponse(
    const Aria2TellStatusResponse& aria2TellStatusResponse
) {

    // active, waiting, paused, error, complete, removed

    const QString& statusAria = aria2TellStatusResponse.status;

    TorrentDownloadData::Status torrentDownloadDataStatus = TorrentDownloadData::Status::QueuedStatus;

    if (statusAria == "active") {
        torrentDownloadDataStatus = TorrentDownloadData::Status::DownloadingStatus;
    } else if (statusAria == "waiting") {
        torrentDownloadDataStatus = TorrentDownloadData::Status::QueuedStatus;
    } else if (statusAria == "paused") {
        torrentDownloadDataStatus = TorrentDownloadData::Status::PausedStatus;
    } else if (statusAria == "error") {
        torrentDownloadDataStatus = TorrentDownloadData::Status::ErrorStatus;
    } else if (statusAria == "complete") {
        torrentDownloadDataStatus = TorrentDownloadData::Status::CompletedStatus;
    } else if (statusAria == "removed") {
        torrentDownloadDataStatus = TorrentDownloadData::Status::CancelledStatus;
    };

    bool ok = false;

    const long long aria2TellStatusResponseTotalLengthLongLong = aria2TellStatusResponse.totalLength.toULongLong(&ok);
    const long long aria2TellStatusResponseCompletedLengthLongLong = aria2TellStatusResponse.completedLength.toULongLong(&ok);

    float progress = 0.0f;

    if (aria2TellStatusResponseTotalLengthLongLong != 0.0f) {
        progress = float(aria2TellStatusResponseCompletedLengthLongLong / aria2TellStatusResponseTotalLengthLongLong);
    };

    if (progress < 0.0f || progress > 1.0f) {
        progress = 0.0f;
    };

    return TorrentDownloadData{
        .infoHash = aria2TellStatusResponse.infoHash,
        .seeders = aria2TellStatusResponse.numSeeders.toInt(),
        .sizeBytes = aria2TellStatusResponse.totalLength.toLongLong(),
        .numberOfFiles = aria2TellStatusResponse.files.size(),
        .gid = aria2TellStatusResponse.gid,
        .status = torrentDownloadDataStatus,
        .progress = progress
    };

};
