
#pragma once

#include <QObject>
#include <QString>
#include "search/TorrentSearchResults.h"

class TorrentProvider : public QObject {

    Q_OBJECT

    public:

        explicit TorrentProvider(
            QObject* parent = nullptr
        );

        virtual void search(
            const unsigned int searchId,
            const QString& query
        ) const = 0;

    signals:

        void searchCompleted(TorrentSearchResults torrentSearchResults);

};
