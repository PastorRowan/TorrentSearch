
#include "providers/PirateBayProvider.h"
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <Qstring>

PirateBayProvider::PirateBayProvider(
    QNetworkAccessManager* networkAccessManagerP,
    QObject* parent
): TorrentProvider(networkAccessManagerP, parent) {

};

QUrl PirateBayProvider::createSearchUrl(
    const QString& query
) {

    QUrl url("https://apibay.org/q.php");

    QUrlQuery parameters;

    parameters.addQueryItem("q", query);
    parameters.addQueryItem("cat", "0");

    url.setQuery(parameters);

    return url;

};

TorrentSearchResults PirateBayProvider::parseResponse(
    const QByteArray& response
) {

    TorrentSearchResults results = {};

    QJsonDocument document = QJsonDocument::fromJson(response);

    if (!document.isArray()) {
        return results;
    };

    QJsonArray torrents = document.array();

    for (const QJsonValue& value : torrents) {

        QJsonObject object = value.toObject();

        TorrentSearchResult result;

        result.name = object["name"].toString();
        result.infoHash = object["info_hash"].toString();
        result.sizeBytes = object["size"].toString().toLongLong();
        result.seeders = object["seeders"].toInt();
        result.leechers = object["leechers"].toInt();

        results.append(result);
    }

    return results;

};

QString PirateBayProvider::getName() const {
    return "PirateBayProvider";
};
