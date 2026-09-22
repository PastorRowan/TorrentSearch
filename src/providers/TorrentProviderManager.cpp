
#include "providers/TorrentProviderManager.h"
#include <QNetworkAccessManager>
#include <QString>
#include <QDebug>

TorrentProviderManager::TorrentProviderManager(
    QObject* parent
):
    QObject(parent),
    networkAccessManager(new QNetworkAccessManager(this)),
    searchId(0),
    torrentProviders({}),
    torrentSearchResults({}) {

    for (const TorrentProviderFactory& torrentProviderFactory : providerFactories) {
        TorrentProvider* torrentProvider = torrentProviderFactory(networkAccessManager, this);
        connect(
            torrentProvider,
            &TorrentProvider::searchCompleted,
            this,
            &TorrentProviderManager::providerSearchCompleted
        );
        torrentProviders.push_back(torrentProvider);
    };

};

unsigned int TorrentProviderManager::getSearchId() {
    return searchId;
};

void TorrentProviderManager::setSearchId(
    const unsigned int newSearchId
) {
    searchId = newSearchId;
};

void TorrentProviderManager::incrementSearchId() {
    searchId++;
};

unsigned int TorrentProviderManager::generateSearchId() {
    searchId++;
    return searchId;
};

void TorrentProviderManager::search(
    const QString &query
) {

    qDebug().noquote() << "TorrentProviderManager::search called with query '" << query << "'";

    incrementSearchId();

    torrentSearchResults.clear();

    emit searchResultsUpdated(torrentSearchResults);

    for (const auto& torrentProvider : torrentProviders) {

        torrentProvider->search(
            getSearchId(),
            query
        );

    };

};

void TorrentProviderManager::providerSearchCompleted(
    const unsigned int searchId,
    const TorrentSearchResults& providerSearchResults
) {

    qDebug().noquote()
        << "TorrentProviderManager::providerSearchCompleted slot called with\n"
        << "searchId: " << searchId << '\n'
    ;

    if (searchId != getSearchId()) {
        return;
    };

    torrentSearchResults.append(providerSearchResults);

    qDebug().noquote()
        << "torrentSearchResults after update:\n"
        << torrentSearchResults.toQString()
    ;

    emit searchResultsUpdated(torrentSearchResults);

};
