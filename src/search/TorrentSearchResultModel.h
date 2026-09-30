
#pragma once

#include "search/TorrentSearchResult.h"

#include <QObject>
class QString;

class TorrentSearchResultModel : public QObject {

    Q_OBJECT

    private:

        TorrentSearchResult torrentSearchResult;

    protected:

    public:

        TorrentSearchResultModel(
            TorrentSearchResult torrentSearchResultP,
            QObject* parent = nullptr
        );

        QString toQString() const;

};
