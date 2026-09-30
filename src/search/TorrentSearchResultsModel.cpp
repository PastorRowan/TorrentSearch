
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
        {
            TorrentSearchResultRole, "torrentSearchResult"
        }
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

        case TorrentSearchResultRole:
            return QVariant::fromValue(result);

        default:
            return {};

    };

};

int TorrentSearchResultsModel::rowCount(const QModelIndex &parent) const {

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
