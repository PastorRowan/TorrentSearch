
#pragma once

#include <QString>
#include <QUrl>

struct TorrentSearchResult {

    QString name;

    QString infoHash;

    QUrl magnetUrl;
    QUrl torrentUrl;

    long long sizeBytes = 0;

    int seeders = 0;
    int leechers = 0;

    QString category;

    QString source;

};
