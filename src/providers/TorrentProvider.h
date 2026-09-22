
#pragma once

#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;
class QString;
class QUrl;
class QByteArray;
#include "search/TorrentSearchResults.h"

class TorrentProvider : public QObject {

    Q_OBJECT

    private:

        QNetworkAccessManager* networkAccessManager;

        QNetworkReply* networkReply;

        QNetworkAccessManager* getNetworkAccessManager();

        void setNetworkAccessManager(
            QNetworkAccessManager* newNetworkAccessManager
        );

        QNetworkReply* getNetworkReply();

        void setNetworkReply(
            const unsigned int searchId,
            QNetworkReply* newNetworkReply
        );

        void cancelSearch();

        void onNetworkReplyFinished(
            const unsigned int searchId,
            QNetworkReply* reply
        );

    protected:

        virtual QUrl createSearchUrl(
            const QString& query
        ) = 0;

        virtual TorrentSearchResults parseResponse(
            const QByteArray& response
        ) = 0;

    public:

        explicit TorrentProvider(
            QNetworkAccessManager* networkAccessManagerP,
            QObject* parent = nullptr
        );

        void search(
            const unsigned int searchId,
            const QString& query
        );

        virtual QString getName() const = 0;

    signals:

        void searchCompleted(
            const unsigned int searchId,
            const TorrentSearchResults& torrentSearchResults
        );

};
