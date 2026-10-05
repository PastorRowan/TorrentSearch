
#include "download/TorrentDownloadDatas.h"

#include <QString>

QString TorrentDownloadDatas::toQString() const {
    QString output = "";
    for (auto cit = cbegin(); cit != cend(); ++cit) {
        output += (*cit).toQString();
    };
    return output;
};
