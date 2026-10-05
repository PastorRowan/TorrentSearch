
#include "TorrentSearchResult.h"

QString TorrentSearchResult::toQString() const {
    return
        "TorrentSearchResult:\n"
        "  name: " + name + "\n" +
        "  infoHash: " + infoHash + "\n" +
        "  leechers: " + QString::number(leechers) + "\n" +
        "  seeders: " + QString::number(seeders) + "\n" +
        "  sizeBytes: " + QString::number(sizeBytes) + "\n" +
        "  numberOfFiles: " + QString::number(sizeBytes) + "\n" +
        "  magnetUrl: " + magnetUrl + "\n" +
        "  torrentUrl: " + torrentUrl
    ;
};
