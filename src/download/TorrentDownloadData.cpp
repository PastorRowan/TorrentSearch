
#include "download/TorrentDownloadData.h"

#include <QMetaEnum>

QString TorrentDownloadData::statusToQString() const {
    return QMetaEnum::fromType<Status>().valueToKey(static_cast<int>(status));
};

QString TorrentDownloadData::toQString() const {
    return
        "name: " + name +
        "infoHash: " + infoHash +
        "leechers: " + QString::number(leechers) +
        "seeders: " + QString::number(seeders) +
        "sizeBytes: " + QString::number(sizeBytes) +
        "numberOfFiles: " + QString::number(numberOfFiles) +
        "magnetUrl: " + magnetUrl +
        "torrentUrl: " + torrentUrl +
        "status: " + statusToQString() +
        "progress: " + QString::number(progress)
    ;
};

bool operator==(
    const TorrentDownloadData& leftData,
    const TorrentDownloadData& rightData
) {
    return
        leftData.name == rightData.name &&
        leftData.infoHash == rightData.infoHash &&
        leftData.leechers == rightData.leechers &&
        leftData.seeders == rightData.seeders &&
        leftData.sizeBytes == rightData.sizeBytes &&
        leftData.numberOfFiles == rightData.numberOfFiles &&
        leftData.magnetUrl == rightData.magnetUrl &&
        leftData.torrentUrl == rightData.torrentUrl &&
        leftData.status == rightData.status &&
        leftData.progress == rightData.progress
    ;
};
