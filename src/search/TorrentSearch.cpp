
#include "search/TorrentSearch.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QUrl>
#include <QDebug>

#define LOG_SEARCH_RESULTS 0

#if LOG_SEARCH_RESULTS
    #include "helpers/helpers.h"
#endif

TorrentSearch::TorrentSearch(
    QNetworkAccessManager* networkAccessManagerP,
    QObject* parent
):
    networkAccessManager(networkAccessManagerP),
    networkReply(nullptr),
    QObject(parent) {

};

QNetworkAccessManager* TorrentSearch::getNetworkAccessManager() {
    return networkAccessManager;
};

void TorrentSearch::setNetworkAccessManager(
    QNetworkAccessManager* newNetworkAccessManager
) {
    networkAccessManager = newNetworkAccessManager;
};

QNetworkReply* TorrentSearch::getNetworkReply() {
    return networkReply;
};

void TorrentSearch::setNetworkReply(
    const unsigned int searchId,
    QNetworkReply* newNetworkReply
) {

    networkReply = newNetworkReply;

    connect(
        newNetworkReply,
        &QNetworkReply::finished,
        this,
        [this, searchId, newNetworkReply]() {
            onNetworkReplyFinished(searchId, newNetworkReply);
        }
    );

};

void TorrentSearch::cancelSearch() {
    if (networkReply == nullptr) {
        return;
    };
    networkReply->abort();
    networkReply->deleteLater();
    networkReply = nullptr;
};

void TorrentSearch::onNetworkReplyFinished(
    const unsigned int searchId,
    QNetworkReply* reply
) {

    qDebug().noquote()
        << "TorrentSearch::onNetworkReplyFinished called with\n"
        << "searchId: " << searchId << "\n"
        << "reply:\n"
        << "request url: " << reply->request().url().toString(QUrl::FullyEncoded) << "\n"
        << "HTTP status: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << "\n"
        << "Error: " << reply->error() << "\n"
        << "Error string: " << reply->errorString()
        #if LOG_SEARCH_RESULTS
        << "\n"
        << "response:\n" << helpers::fromByteArrayToPrettyQJson(reply->readAll())
        #endif
        #undef LOG_SEARCH_RESULTS
    ;

    if (reply != networkReply) {
        reply->deleteLater();
        return;
    };

    TorrentSearchResults results = {};

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        results = parseResponse(response);
    };

    networkReply = nullptr;
    reply->deleteLater();

    emit searchCompleted(searchId, results);

};
void TorrentSearch::search(
    const unsigned int searchId,
    const QString& query
) {

    cancelSearch();

    QUrl url = createSearchUrl(query);

    QNetworkRequest request(url);

    QNetworkReply* reply = getNetworkAccessManager()->get(request);

    setNetworkReply(searchId, reply);

};
