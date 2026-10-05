
#include "download/TorrentDownloadData.h"

#include <QMetaEnum>

QString TorrentDownloadData::statusToQString() const {
    return QMetaEnum::fromType<Status>().valueToKey(static_cast<int>(status));
};

QString TorrentDownloadData::toQString() const {
    return
        QString("TorrentDownloadData:") + "\n" +
        "  name: " + name + "\n" +
        "  infoHash: " + infoHash + "\n" +
        "  leechers: " + QString::number(leechers) + "\n" +
        "  seeders: " + QString::number(seeders) + "\n" +
        "  sizeBytes: " + QString::number(sizeBytes) + "\n" +
        "  numberOfFiles: " + QString::number(numberOfFiles) + "\n" +
        "  magnetUrl: " + magnetUrl + "\n" +
        "  torrentUrl: " + torrentUrl + "\n" +
        "  status: " + statusToQString() + "\n" +
        "  progress: " + QString::number(progress)
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
