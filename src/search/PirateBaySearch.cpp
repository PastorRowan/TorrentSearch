
#include "search/PirateBaySearch.h"
#include "helpers/helpers.h"

#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <Qstring>

#define MOCK_RESPONSE 0

#if MOCK_RESPONSE
    #include "search/PIRATE_BAY_SEARCH_RESPONSE.h"
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

    return helpers::encodeMagnetUrl(url);

};

TorrentSearchResults PirateBaySearch::parseResponse(
    const QByteArray& response
) {

    TorrentSearchResults results = {};

    QJsonDocument document = QJsonDocument::fromJson(response);

    #if MOCK_RESPONSE
        document = QJsonDocument::fromJson(PIRATE_BAY_SEARCH_RESPONSE);
    #endif
    #undef MOCK_RESPONSE

    if (!document.isArray()) {
        return results;
    };

    QJsonArray torrents = document.array();

    for (const QJsonValue& value : torrents) {

        QJsonObject object = value.toObject();

        TorrentSearchResult result;

        result.name = object["name"].toString();
        result.infoHash = object["info_hash"].toString();
        result.leechers = object["leechers"].toString().toInt();
        result.seeders = object["seeders"].toString().toInt();
        result.sizeBytes = object["size"].toString().toLongLong();
        result.numberOfFiles = object["num_files"].toString().toInt();

        result.magnetUrl = QString("magnet:?xt=urn:btih:%1").arg(result.infoHash);

        results.append(result);

    };

    return results;

};

bool PirateBaySearch::isUrlValid(
    const QString& url
) const {

    const QUrl qUrl(url);
    const QUrlQuery query(qUrl);

    return (
        qUrl.isValid()
        && qUrl.scheme() == "https"
        && qUrl.host() == "apibay.org"
        && qUrl.path() == "/q.php"
        && query.hasQueryItem("q")
        && query.hasQueryItem("cat")
    );

};

QString PirateBaySearch::getName() const {
    return "PirateBaySearch";
};
