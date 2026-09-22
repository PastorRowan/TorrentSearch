
#pragma once

#include <QObject>
#include <QVector>
class QNetworkAccessManager;
class QString;
#include "providers/TorrentProvider.h"
#include "providers/EZTVProvider.h"
#include "providers/PirateBayProvider.h"
#include "search/TorrentSearchResults.h"

using TorrentProviderFactory = std::function<TorrentProvider*(QNetworkAccessManager*, QObject*)>;

const QVector<TorrentProviderFactory> providerFactories = {
    /*
    [](QNetworkAccessManager* networkAccessManager, QObject* parent) {
        return new EZTVProvider(networkAccessManager, parent);
    },
    */
    [](QNetworkAccessManager* networkAccessManager, QObject* parent) {
        return new PirateBayProvider(networkAccessManager, parent);
    }
};

using TorrentProviders = QVector<TorrentProvider*>;

class TorrentProviderManager : public QObject {

    Q_OBJECT

    private:

        QNetworkAccessManager* networkAccessManager;

        TorrentProviders torrentProviders;

        TorrentSearchResults torrentSearchResults;

        unsigned int searchId;

        unsigned int getSearchId();

        void setSearchId(
            const unsigned int newSearchId
        );

        void incrementSearchId();

        unsigned int generateSearchId();

    public:

        explicit TorrentProviderManager(QObject* parent = nullptr);

        Q_INVOKABLE void search(const QString &query);

    // public slots:

        void providerSearchCompleted(
            const unsigned int searchId,
            const TorrentSearchResults& providerSearchResults
        );

    signals:

        void searchResultsUpdated(
            TorrentSearchResults torrentSearchResults
        ) const;

};
