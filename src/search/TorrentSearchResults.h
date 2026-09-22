
#pragma once

#include "search/TorrentSearchResult.h"
#include <QVector>
class QString;

class TorrentSearchResults : public QVector<TorrentSearchResult> {

    private:

    protected:

    public:

        QString toQString() const;

};
