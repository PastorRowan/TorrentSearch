
#include "download/TorrentDownload.h"

#include <QMetaEnum>

TorrentDownload::TorrentDownload(
    QObject* parent
):
    QObject(parent) {

};

void TorrentDownload::download() {

    status = Status::DownloadStatus;

};

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
        "numberOfFiles: " + QString::number(sizeBytes) +
        "magnetUrl: " + magnetUrl +
        "torrentUrl: " + torrentUrl +
        "status: " + statusToQString() +
        "progress: " + QString::number(progress)
    ;
};
