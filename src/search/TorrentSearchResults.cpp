
#include "search/TorrentSearchResults.h"
#include <QString>

QString TorrentSearchResults::toQString() const {

    QString output = "TorrentSearchResults:";

    for (auto cit = cbegin(); cit != cend(); ++cit) {
        const TorrentSearchResult& torrentSearchResult = *cit;
        output += "\n\tName: " + torrentSearchResult.name;
        output += "\n\tMagnet url: " + torrentSearchResult.magnetUrl.toDisplayString();
        output += "\n";
    };

    return output;

};
