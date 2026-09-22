
#pragma once

#include "providers/TorrentProvider.h"

class PirateBayProvider : public TorrentProvider {

    private:

    protected:

        QUrl createSearchUrl(
            const QString& query
        ) override;

        TorrentSearchResults parseResponse(
            const QByteArray& response
        ) override;

    public:

        explicit PirateBayProvider(
            QNetworkAccessManager* networkAccessManagerP,
            QObject* parent = nullptr
        );

        QString getName() const override;

};
