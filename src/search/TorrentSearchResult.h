
#pragma once

#include <QString>
#include <QUrl>

struct TorrentSearchResult {

    QString name = "";
    QString infoHash = "";
    int leechers = 0;
    int seeders = 0;
    long long sizeBytes = 0;
    int numberOfFiles = 0;
    QString magnetUrl = "";
    QString torrentUrl = "";
    QString toQString() const;

};
