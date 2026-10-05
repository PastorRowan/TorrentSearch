
#include "search/TorrentSearchManager.h"

#include <QNetworkAccessManager>
#include <QString>
#include <QDebug>

TorrentSearchManager::TorrentSearchManager(
    QObject* parent
):
    QObject(parent),
    networkAccessManager(new QNetworkAccessManager(this)),
    searchId(0),
    torrentSearchs({}),
    torrentSearchResults({}) {

    for (const TorrentSearchFactory& torrentSearchFactory : providerFactories) {
        TorrentSearch* torrentSearch = torrentSearchFactory(networkAccessManager, this);
        connect(
            torrentSearch,
            &TorrentSearch::searchCompleted,
            this,
            &TorrentSearchManager::providerSearchCompleted
        );
        torrentSearchs.push_back(torrentSearch);
    };

};

unsigned int TorrentSearchManager::getSearchId() {
    return searchId;
};

void TorrentSearchManager::setSearchId(
    const unsigned int newSearchId
) {
    searchId = newSearchId;
};

void TorrentSearchManager::incrementSearchId() {
    searchId++;
};

unsigned int TorrentSearchManager::generateSearchId() {
    searchId++;
    return searchId;
};

void TorrentSearchManager::search(
    const QString &query
) {

    qDebug().noquote() << "TorrentSearchManager::search called with query '" << query << "'";

    incrementSearchId();

    torrentSearchResults.clear();

    emit searchResultsUpdated(torrentSearchResults);

    for (const auto& torrentSearch : torrentSearchs) {

        torrentSearch->search(
            getSearchId(),
            query
        );

    };

};

void TorrentSearchManager::providerSearchCompleted(
    const unsigned int searchId,
    const TorrentSearchResults& providerSearchResults
) {

    #define LOG_PROVIDER_SEARCH_RESULTS 0

    qDebug().noquote()
        << "TorrentSearchManager::providerSearchCompleted slot called with\n"
        << "searchId: " << searchId
        #if LOG_PROVIDER_SEARCH_RESULTS
        << "\n"
        << "providerSearchResults:\n"
        << providerSearchResults.toQString()
        #endif
        #undef LOG_PROVIDER_SEARCH_RESULTS
    ;

    if (searchId != getSearchId()) {
        return;
    };

    torrentSearchResults.append(providerSearchResults);

    emit searchResultsUpdated(torrentSearchResults);

};
