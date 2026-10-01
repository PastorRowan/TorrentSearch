
#include "download/TorrentDownloads.h"

#include <QString>

template<TorrentDownloadPointer T>
QString TorrentDownloads<T>::toQString() const {
    QString output = "";
    for (auto cit = cbegin(); cit != cend(); ++cit) {
        if (*cit != nullptr) {
            output += (*cit)->toQString();
        };
    };
    return output;
};

template class TorrentDownloads<TorrentDownload*>;
template class TorrentDownloads<QPointer<TorrentDownload>>;
