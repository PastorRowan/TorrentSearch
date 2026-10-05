
#include "helpers/helpers.h"

#include <QString>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>
#include <QUrl>
#include <QUrlQuery>

QString helpers::prettyQJson(
    const QJsonValue& value
) {

    if (value.isObject()) {
        return QJsonDocument(value.toObject()).toJson(QJsonDocument::Indented);
    };

    if (value.isArray()) {
        return QJsonDocument(value.toArray()).toJson(QJsonDocument::Indented);
    };

    return value.toVariant().toString();

};

QString helpers::fromByteArrayToPrettyQJson(
    const QByteArray& json
) {

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(json, &error);

    if (error.error != QJsonParseError::NoError) {
        return QString::fromUtf8(json);
    };

    return QString::fromUtf8(
        document.toJson(QJsonDocument::Indented)
    );

};

QUrl helpers::encodeMagnetUrl(
    QUrl url
) {
    QString encodedQuery = url.query(QUrl::FullyEncoded);
    encodedQuery.replace("%20", "+");
    url.setQuery(encodedQuery);
    return url;
};
