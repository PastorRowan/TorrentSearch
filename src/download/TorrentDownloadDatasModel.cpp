
#include "download/TorrentDownloadDatasModel.h"

#include <QVariant>
#include <QModelIndex>
#include <QHash>
#include <QByteArray>
#include <QDebug>

TorrentDownloadDatasModel::TorrentDownloadDatasModel(
    QObject* parent
):
    QAbstractListModel(parent),
    torrentDownloadDatas({}) {

};

QHash<int, QByteArray> TorrentDownloadDatasModel::roleNames() const {
    return {
        {
            TorrentDownloadDataRole, "torrentDownloadData"
        }
    };
};

QVariant TorrentDownloadDatasModel::data(
    const QModelIndex &index,
    int role
) const {

    if (!index.isValid() || index.row() >= torrentDownloadDatas.size()) {
        return {};
    };

    const auto &result = torrentDownloadDatas[index.row()];

    switch (role) {

        case TorrentDownloadDataRole:
            return QVariant::fromValue(result);

        default:
            return {};

    };

};

int TorrentDownloadDatasModel::rowCount(const QModelIndex &parent) const {

    if (parent.isValid()) {
        return 0;
    };

    return static_cast<int>(torrentDownloadDatas.size());

};

void TorrentDownloadDatasModel::setTorrentDownloadDatas(
    const TorrentDownloadDatas torrentDownloadDatasP
) {

    #define LOG_DOWNLOAD_DATAS 0

    #if LOG_DOWNLOAD_DATAS
        qDebug().noquote() << "torrentDownloadDatasP:\n";
        for (const TorrentDownloadData& torrentDownloadData : torrentDownloadDatasP) {
            qDebug().noquote() << torrentDownloadData.toQString();
        };
    #endif
    #undef LOG_DOWNLOAD_DATAS

    beginResetModel();

    torrentDownloadDatas = torrentDownloadDatasP;

    endResetModel();

};
