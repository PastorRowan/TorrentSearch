
#include "download/TorrentDownload.h"

#include <QMetaEnum>

TorrentDownload::TorrentDownload(
    TorrentDownloadData dataP,
    QObject* parent
):
    QObject(parent),
    data(dataP) {

};

const TorrentDownloadData& TorrentDownload::getData() const {
    return data;
};

void TorrentDownload::setData(
    const TorrentDownloadData& newData
) {
    if (data == newData) {
        return;
    };
    data = newData;
    emit dataChanged();
};

QString TorrentDownload::toQString() const {
    return data.toQString();
};

void TorrentDownload::resume() {

    data.status = TorrentDownloadData::Status::DownloadingStatus;

};

void TorrentDownload::pause() {

    data.status = TorrentDownloadData::Status::PausedStatus;

};

void TorrentDownload::cancel() {

    data.status = TorrentDownloadData::Status::CancelledStatus;

};
