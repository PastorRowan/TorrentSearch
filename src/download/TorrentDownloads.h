
#pragma once

#include "download/TorrentDownload.h"

#include <QVector>
#include <QPointer>
class QString;
#include <concepts>

template<typename T>
concept TorrentDownloadPointer =
    std::same_as<T, TorrentDownload*> ||
    std::same_as<T, QPointer<TorrentDownload>>;

template<TorrentDownloadPointer T>
class TorrentDownloads : public QVector<T> {

    private:

    protected:

    public:

        QString toQString() const;

};
