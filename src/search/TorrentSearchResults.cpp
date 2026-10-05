
#include "search/TorrentSearchResults.h"

#include <QString>

QString TorrentSearchResults::toQString() const {

    QString output = "TorrentSearchResults:";

    for (auto cit = cbegin(); cit != cend(); ++cit) {
        const TorrentSearchResult& torrentSearchResult = *cit;
        output += torrentSearchResult.toQString();
    };

    return output;

};
