
#pragma once

#include <QAbstractListModel>
#include "search/TorrentSearchResults.h"
class QVariant;
class QModelIndex;
// class QHash;
class QByteArray;

class TorrentSearchResultsModel : public QAbstractListModel {

    Q_OBJECT

    private:

        TorrentSearchResults torrentSearchResults;

    public:

        enum Roles {
            NameRole = Qt::UserRole + 1,
            MagnetUrlRole,
            SizeRole,
            SeedersRole,
            LeechersRole
        };

        explicit TorrentSearchResultsModel(
            QObject *parent = nullptr
        );

        int rowCount(const QModelIndex &parent = {}) const override;

        QVariant data(
            const QModelIndex &index,
            int role = Qt::DisplayRole
        ) const override;

        QHash<int, QByteArray> roleNames() const override;

        void setResults(
            const TorrentSearchResults results
        );

};
