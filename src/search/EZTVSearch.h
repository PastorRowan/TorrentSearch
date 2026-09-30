
#pragma once

#include "search/TorrentSearch.h"

class EZTVSearch : public TorrentSearch {

    private:

    protected:

        QUrl createSearchUrl(
            const QString& query
        ) override;

        TorrentSearchResults parseResponse(
            const QByteArray& response
        ) override;

    public:

        explicit EZTVSearch(
            QNetworkAccessManager* networkAccessManagerP,
            QObject* parent
        );

        QString getName() const override;

};
