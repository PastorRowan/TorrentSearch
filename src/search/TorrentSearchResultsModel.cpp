
#include "search/TorrentSearchResultsModel.h"
#include "search/TorrentSearchResults.h"
#include <QVariant>
#include <QModelIndex>
#include <QHash>
#include <QByteArray>
#include <QDebug>

TorrentSearchResultsModel::TorrentSearchResultsModel(
    QObject* parent
):
    QAbstractListModel(parent),
    torrentSearchResults({}) {

};

QHash<int, QByteArray> TorrentSearchResultsModel::roleNames() const {
    return {
        { NameRole, "name" },
        { MagnetUrlRole, "magnetUrl" },
        { SizeRole, "size" },
        { SeedersRole, "seeders" }
    };
};

QVariant TorrentSearchResultsModel::data(
    const QModelIndex &index,
    int role
) const {

    if (!index.isValid() || index.row() >= torrentSearchResults.size()) {
        return {};
    };

    const auto &result = torrentSearchResults[index.row()];

    switch (role) {

        case NameRole:
            return result.name;

        case MagnetUrlRole:
            return result.magnetUrl;

        case SizeRole:
            return result.sizeBytes;

        case SeedersRole:
            return result.seeders;

        case LeechersRole:
            return result.leechers;

        default:
            return {};

    };

};

int TorrentSearchResultsModel::rowCount(const QModelIndex &parent) const {

    qDebug().noquote()
        << "TorrentSearchResultsModel::rowCount called with"
        << "torrentSearchResults.size(): " << torrentSearchResults.size()
        << "parent: " << parent
    ;

    if (parent.isValid()) {
        return 0;
    };

    return static_cast<int>(torrentSearchResults.size());

};

void TorrentSearchResultsModel::setResults(
    const TorrentSearchResults results
) {

    beginResetModel();

    torrentSearchResults = results;

    endResetModel();

};
