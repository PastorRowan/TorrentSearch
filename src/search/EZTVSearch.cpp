
#include "search/EZTVSearch.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <Qstring>

EZTVSearch::EZTVSearch(
    QNetworkAccessManager* networkAccessManagerP,
    QObject* parent
): TorrentSearch(networkAccessManagerP, parent) {

};

QUrl EZTVSearch::createSearchUrl(
    const QString& query
) {

    QUrl url("http://qt-project.org");

    return url;

};

TorrentSearchResults EZTVSearch::parseResponse(
    const QByteArray& response
) {
    return {};
};

bool EZTVSearch::isUrlValid(
    const QString& url
) const {
    return true;
};

QString EZTVSearch::getName() const {
    return "EZTVSearch";
};
