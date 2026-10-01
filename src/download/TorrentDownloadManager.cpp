
#include "download/TorrentDownloadManager.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>

TorrentDownloadManager::TorrentDownloadManager(
    QObject* parent = nullptr
):
    QObject(parent),
    networkAccessManager(this),
    torrentDownloads({}) {

    aria2Process.start(
        "aria2c",
        {
            "--enable-rpc=true",
            "--rpc-listen-all=false",
            "--rpc-listen-port=6800",
            "--rpc-secret=mySecret"
        }
    );

    connect(
        &aria2Process,
        &QProcess::started,
        this,
        [] {
            qDebug() << "aria2c QProcess started";
        }
    );

    connect(
        &aria2Process,
        &QProcess::errorOccurred,
        this,
        [](QProcess::ProcessError error) {
            qDebug() << "aria2c error:" << error;
        }
    );

    connect(
        &statusTimer,
        &QTimer::timeout,
        this,
        &TorrentDownloadManager::pollDownloadStatuses
    );

    statusTimer.start(1000);

};

void TorrentDownloadManager::requestAria2c(
    const QString& method,
    const QJsonArray& params,
    std::function<void(QNetworkReply*)> callback
) {

};

void TorrentDownloadManager::pollDownloadStatuses() {

    requestAria2c(
        ""
    );

    QJsonObject request{
        { "jsonrpc", "2.0" },
        { "id", "status" },
        { "method", "aria2.tellActive" },
        { "params", QJsonArray{
            "token:mySecret"
        }}
    };

    QNetworkRequest networkRequest(
        QUrl("http://127.0.0.1:6800/jsonrpc")
    );

    networkRequest.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json"
    );

    QNetworkReply* reply =
        networkAccessManager.post(
            networkRequest,
            QJsonDocument(request).toJson(QJsonDocument::Compact)
        );

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        [this, reply]()
        {
            // parse response
            // update downloads

            reply->deleteLater();
        }
    );

    QJsonObject request{
        { "jsonrpc", "2.0" },
        { "id", "status" },
        { "method", "aria2.tellStopped" },
        { "params", QJsonArray{
            "token:mySecret"
        }}
    };

    QNetworkRequest networkRequest(
        QUrl("http://127.0.0.1:6800/jsonrpc")
    );

    networkRequest.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json"
    );

    QNetworkReply* reply =
        networkAccessManager.post(
            networkRequest,
            QJsonDocument(request).toJson(QJsonDocument::Compact)
        );

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        [this, reply]()
        {
            // parse response
            // update downloads

            reply->deleteLater();
        }
    );

    QJsonObject request{
        { "jsonrpc", "2.0" },
        { "id", "status" },
        { "method", "aria2.tellWaiting" },
        { "params", QJsonArray{
            "token:mySecret"
        }}
    };

    QNetworkRequest networkRequest(
        QUrl("http://127.0.0.1:6800/jsonrpc")
    );

    networkRequest.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json"
    );

    QNetworkReply* reply =
        networkAccessManager.post(
            networkRequest,
            QJsonDocument(request).toJson(QJsonDocument::Compact)
        );

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        [this, reply]()
        {
            // parse response
            // update downloads

            reply->deleteLater();
        }
    );

};

QString TorrentDownloadManager::toQString() const {
    return torrentDownloads.toQString();
};

void TorrentDownloadManager::addDownload(
    TorrentDownloadData data
) {

    QJsonObject request{
        { "jsonrpc", "2.0" },
        { "id", data.infoHash },
        { "method", "aria2.addUri" },
        { "params", QJsonArray {
            "token:mySecret",
            QJsonArray{data.magnetUrl}
        }}
    };

    QNetworkRequest networkRequest(
        QUrl("http://127.0.0.1:6800/jsonrpc")
    );

    networkRequest.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json"
    );

    QNetworkReply* reply =
        networkAccessManager.post(
            networkRequest,
            QJsonDocument(request).toJson(QJsonDocument::Compact)
        );

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        [ reply ]() {
            qDebug()
                << "aria2 response:"
                << reply->readAll()
            ;
            reply->deleteLater();
        }
    );

};

void TorrentDownloadManager::resumeDownload(
    const QString& infoHash
) {

};

void TorrentDownloadManager::pauseDownload(
    const QString& infoHash
) {

};

void TorrentDownloadManager::cancelDownload(
    const QString& infoHash
) {

};
