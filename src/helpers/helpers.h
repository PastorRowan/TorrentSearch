
#pragma once

class QString;
class QJsonValue;
class QByteArray;
class QUrl;

namespace helpers {

    QString prettyQJson(
        const QJsonValue& value
    );

    QString fromByteArrayToPrettyQJson(
        const QByteArray& json
    );

    QUrl encodeMagnetUrl(
        QUrl url
    );

};
