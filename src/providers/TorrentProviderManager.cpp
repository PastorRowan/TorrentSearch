
#include "providers/TorrentProviderManager.h"

TorrentProviderManager::TorrentProviderManager(
    QObject* parent = nullptr
): QObject(parent) {

};

unsigned int TorrentProviderManager::getSearchId() {
    return searchId;
};

void TorrentProviderManager::setSearchId(
    const unsigned int newSearchId
) {
    searchId = newSearchId;
};

unsigned int TorrentProviderManager::generateSearchId() {
    searchId++;
    return searchId;
};

void TorrentProviderManager::search(
    const QString &query
) const {

    TorrentSearchResults torrentSearchResults = {};

    for (const auto& torrentProvider : torrentProviders) {

        torrentProvider->search(
            generateSearchId(),
            query
        );

    };

    emit searchCompleted(torrentSearchResults);

};
