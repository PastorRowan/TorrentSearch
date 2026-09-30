
#include "TorrentSearchResult.h"

QString TorrentSearchResult::toQString() const {
    return
        "name: " + name +
        "infoHash: " + infoHash +
        "leechers: " + QString::number(leechers) +
        "seeders: " + QString::number(seeders) +
        "sizeBytes: " + QString::number(sizeBytes) +
        "numberOfFiles: " + QString::number(sizeBytes) +
        "magnetUrl: " + magnetUrl +
        "torrentUrl: " + torrentUrl
    ;
};
