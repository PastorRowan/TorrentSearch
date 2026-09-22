
#pragma once

#include "providers/TorrentProvider.h"

class EZTVProvider : public TorrentProvider {

    private:

    protected:

        QUrl createSearchUrl(
            const QString& query
        ) override;

        TorrentSearchResults parseResponse(
            const QByteArray& response
        ) override;

    public:

        explicit EZTVProvider(
            QNetworkAccessManager* networkAccessManagerP,
            QObject* parent
        );

        QString getName() const override;

};
