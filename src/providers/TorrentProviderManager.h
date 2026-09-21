
#pragma once

#include <QVector>
#include <QObject>
#include <QString>
#include "providers/EZTVProvider.h"
#include "providers/PirateBayProvider.h"
#include "providers/TorrentProvider.h"
#include "search/TorrentSearchResults.h"
#include "networking/NetworkManager.h"

using TorrentProviders = QVector<TorrentProvider*>;

class TorrentProviderManager : public QObject {

    Q_OBJECT

    private:

        unsigned int searchId = 0;

        TorrentProviders torrentProviders = {};

        unsigned int getSearchId();

        void setSearchId(
            const unsigned int newRequestId
        );

        unsigned int generateSearchId();

    public:

        explicit TorrentProviderManager(QObject* parent = nullptr);

        Q_INVOKABLE void search(const QString &query) const;

    signals:

        void searchCompleted(TorrentSearchResults torrentSearchResults) const;

};
