
#include "conversions/conversions.h"

TorrentDownloadData conversions::aria2TellStatusResponseToTorrentDownloadData(
    const Aria2TellStatusResponse& aria2TellStatusResponse,
    TorrentDownloadData torrentDownloadData
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
        progress = static_cast<float>(aria2TellStatusResponseCompletedLengthLongLong) / aria2TellStatusResponseTotalLengthLongLong;
    };

    if (progress < 0.0f || progress > 1.0f) {
        progress = 0.0f;
    };

    torrentDownloadData.infoHash = aria2TellStatusResponse.infoHash;
    torrentDownloadData.seeders = aria2TellStatusResponse.numSeeders.toInt();
    torrentDownloadData.sizeBytes = aria2TellStatusResponse.totalLength.toLongLong();
    torrentDownloadData.numberOfFiles = aria2TellStatusResponse.files.size();
    torrentDownloadData.gid = aria2TellStatusResponse.gid;
    torrentDownloadData.status = torrentDownloadDataStatus;
    torrentDownloadData.progress = progress;

    return torrentDownloadData;

};
