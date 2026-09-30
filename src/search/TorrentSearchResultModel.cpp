
#include "search/TorrentSearchResultModel.h"

TorrentSearchResultModel::TorrentSearchResultModel(
    TorrentSearchResult torrentSearchResultP,
    QObject* parent
):
    torrentSearchResult(torrentSearchResultP),
    QObject(parent) {

};
