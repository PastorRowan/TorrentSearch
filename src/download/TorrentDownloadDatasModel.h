
#pragma once

#include "download/TorrentDownloadDatas.h"

#include <QAbstractListModel>
class QVariant;
class QModelIndex;
// class QHash;
class QByteArray;

class TorrentDownloadDatasModel : public QAbstractListModel {

    Q_OBJECT

    private:

        TorrentDownloadDatas torrentDownloadDatas;

    protected:

    public:

        enum Roles {
            TorrentDownloadDataRole = Qt::UserRole + 1
        };

        explicit TorrentDownloadDatasModel(
            QObject *parent = nullptr
        );

        int rowCount(const QModelIndex &parent = {}) const override;

        QVariant data(
            const QModelIndex &index,
            int role = Qt::DisplayRole
        ) const override;

        QHash<int, QByteArray> roleNames() const override;

        void setTorrentDownloadDatas(
            const TorrentDownloadDatas torrentDownloadDatasP
        );

};
