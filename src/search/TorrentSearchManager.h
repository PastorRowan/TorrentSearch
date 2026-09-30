
#pragma once

#include <QObject>
#include <QVector>
class QNetworkAccessManager;
class QString;
#include "search/TorrentSearch.h"
#include "search/EZTVSearch.h"
#include "search/PirateBaySearch.h"
#include "search/TorrentSearchResults.h"

using TorrentSearchFactory = std::function<TorrentSearch*(QNetworkAccessManager*, QObject*)>;

const QVector<TorrentSearchFactory> providerFactories = {
    /*
    [](QNetworkAccessManager* networkAccessManager, QObject* parent) {
        return new EZTVSearch(networkAccessManager, parent);
    },
    */
    [](QNetworkAccessManager* networkAccessManager, QObject* parent) {
        return new PirateBaySearch(networkAccessManager, parent);
    }
};

using TorrentSearchs = QVector<TorrentSearch*>;

class TorrentSearchManager : public QObject {

    Q_OBJECT

    private:

        QNetworkAccessManager* networkAccessManager;

        TorrentSearchs torrentSearchs;

        TorrentSearchResults torrentSearchResults;

        unsigned int searchId;

        unsigned int getSearchId();

        void setSearchId(
            const unsigned int newSearchId
        );

        void incrementSearchId();

        unsigned int generateSearchId();

    protected:

    public:

        explicit TorrentSearchManager(QObject* parent = nullptr);

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
