
#pragma once

#include "search/TorrentSearch.h"

class PirateBaySearch : public TorrentSearch {

    private:

    protected:

        QUrl createSearchUrl(
            const QString& query
        ) override;

        TorrentSearchResults parseResponse(
            const QByteArray& response
        ) override;

    public:

        explicit PirateBaySearch(
            QNetworkAccessManager* networkAccessManagerP,
            QObject* parent = nullptr
        );

        QString getName() const override;

};
