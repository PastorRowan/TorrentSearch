
#include "providers/TorrentProvider.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QUrl>
#include <QDebug>

TorrentProvider::TorrentProvider(
    QNetworkAccessManager* networkAccessManagerP,
    QObject* parent
):
    networkAccessManager(networkAccessManagerP),
    networkReply(nullptr),
    QObject(parent) {

};

QNetworkAccessManager* TorrentProvider::getNetworkAccessManager() {
    return networkAccessManager;
};

void TorrentProvider::setNetworkAccessManager(
    QNetworkAccessManager* newNetworkAccessManager
) {
    networkAccessManager = newNetworkAccessManager;
};

QNetworkReply* TorrentProvider::getNetworkReply() {
    return networkReply;
};

void TorrentProvider::setNetworkReply(
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

void TorrentProvider::cancelSearch() {
    if (networkReply == nullptr) {
        return;
    };
    networkReply->abort();
    networkReply->deleteLater();
    networkReply = nullptr;
};

void TorrentProvider::onNetworkReplyFinished(
    const unsigned int searchId,
    QNetworkReply* reply
) {

    qDebug().noquote()
        << "TorrentProvider::onNetworkReplyFinished called with\n"
        << "searchId: " << searchId << "\n"
        << "reply:\n"
        << "request url: " << reply->request().url().toString() << "\n"
        << "HTTP status: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << "\n"
        << "Error: " << reply->error() << "\n"
        << "Error string: " << reply->errorString() << "\n"
        << "response:\n" << reply->readAll()
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

void TorrentProvider::search(
    const unsigned int searchId,
    const QString& query
) {

    qDebug().noquote()
        << "torrentProvider '" << getName() << "' called with\n"
        << "searchId: " << searchId << "\n"
        << "query: " << query
    ;

    cancelSearch();

    QUrl url = createSearchUrl(query);

    QNetworkRequest request(url);

    QNetworkReply* reply = getNetworkAccessManager()->get(request);

    setNetworkReply(searchId, reply);

};
