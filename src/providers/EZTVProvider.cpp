
#include "providers/EZTVProvider.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <Qstring>

EZTVProvider::EZTVProvider(
    QNetworkAccessManager* networkAccessManagerP,
    QObject* parent
): TorrentProvider(networkAccessManagerP, parent) {

};

QUrl EZTVProvider::createSearchUrl(
    const QString& query
) {

    QUrl url("http://qt-project.org");

    return url;

};

TorrentSearchResults EZTVProvider::parseResponse(
    const QByteArray& response
) {
    return {};
};

QString EZTVProvider::getName() const {
    return "EZTVProvider";
};
