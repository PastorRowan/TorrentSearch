
#pragma once

#include "download/TorrentDownloadData.h"

#include <QVector>
class QString;

class TorrentDownloadDatas : public QVector<TorrentDownloadData> {

    private:

    protected:

    public:

        QString toQString() const;

};
