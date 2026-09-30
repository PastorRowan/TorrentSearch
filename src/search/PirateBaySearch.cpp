
#include "search/PirateBaySearch.h"

#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <Qstring>

#define USE_MOCK_RESPONSE 1

#if USE_MOCK_RESPONSE
    #include <QFile>
#endif

PirateBaySearch::PirateBaySearch(
    QNetworkAccessManager* networkAccessManagerP,
    QObject* parent
): TorrentSearch(networkAccessManagerP, parent) {

};

QUrl PirateBaySearch::createSearchUrl(
    const QString& query
) {

    QUrl url("https://apibay.org/q.php");

    QUrlQuery parameters;

    parameters.addQueryItem("q", query);
    parameters.addQueryItem("cat", "0");

    url.setQuery(parameters);

    return url;

};

TorrentSearchResults PirateBaySearch::parseResponse(
    const QByteArray& response
) {

    TorrentSearchResults results = {};

    QJsonDocument document = QJsonDocument::fromJson(response);

    #if USE_MOCK_RESPONSE

        QFile file("PirateBaySearchResponse.json");

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            document = QJsonDocument::fromJson(file.readAll());
        };

    #endif

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

        result.magnetUrl = QString("magnet:?xt=urn:btih:%1").arg(result.infoHash);

        results.append(result);

    };

    return results;

};

QString PirateBaySearch::getName() const {
    return "PirateBaySearch";
};
