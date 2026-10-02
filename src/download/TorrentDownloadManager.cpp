
#include "download/TorrentDownloadManager.h"
#include "conversions/conversions.h"

#include <QDebug>

TorrentDownloadManager::TorrentDownloadManager(
    QObject* parent = nullptr
):
    QObject(parent),
    aria2c(new Aria2c(this)),
    statusTimer(new QTimer(this)),
    torrentDownloads({}) {

    connect(
        aria2c,
        &Aria2c::started,
        this,
        &TorrentDownloadManager::onAria2cStarted
    );

    connect(
        statusTimer,
        &QTimer::timeout,
        this,
        &TorrentDownloadManager::pollDownloadStatuses
    );

    connect(
        this,
        &TorrentDownloadManager::aria2cTellStatusResponses,
        this,
        &TorrentDownloadManager::onAria2cTellStatusResponses
    );

    aria2c->start();

};

void TorrentDownloadManager::pollDownloadStatuses() {

    if (!aria2c->isRunning()) {
        return;
    };

    aria2c->aria2TellActive(
        [ this ](std::variant<Aria2TellActiveResponse, Aria2Error> response) {

            if (auto* err = std::get_if<Aria2Error>(&response)) {
                qDebug() << "Failed to run method 'aria2TellActive': " << err->message;
            } else if (auto* result = std::get_if<Aria2TellActiveResponse>(&response)) {
                // result
                emit aria2cTellStatusResponses(result->downloads);
            } else {
                qFatal() << "Error: 'aria2TellActive' responded with a unknown type";
            };

        }
    );

    aria2c->aria2TellStopped(
        {
            .offset = 0,
            .num = torrentDownloads.size() * 2
        },
        [ this ](std::variant<Aria2TellStoppedResponse, Aria2Error> response) {

            if (auto* err = std::get_if<Aria2Error>(&response)) {
                qDebug() << "Failed to run method 'aria2TellActive': " << err->message;
            } else if (auto* result = std::get_if<Aria2TellStoppedResponse>(&response)) {
                emit aria2cTellStatusResponses(result->downloads);
            } else {
                qFatal() << "Error: 'aria2TellStopped' responded with a unknown type";
            };

        }
    );

    aria2c->aria2TellWaiting(
        {
            .offset = 0,
            .num = torrentDownloads.size() * 2
        },
        [ this ](std::variant<Aria2TellWaitingResponse, Aria2Error> response) {

            if (auto* err = std::get_if<Aria2Error>(&response)) {
                qDebug() << "Failed to run method 'aria2TellWaiting': " << err->message;
            } else if (auto* result = std::get_if<Aria2TellWaitingResponse>(&response)) {
                emit aria2cTellStatusResponses(result->downloads);
            } else {
                qFatal() << "Error: 'aria2TellWaiting' responded with a unknown type";
            };

        }
    );

};

void TorrentDownloadManager::onAria2cTellStatusResponses(
    const QVector<Aria2TellStatusResponse> responses
) {

    for (const Aria2TellStatusResponse& response : responses) {

        const QString& infoHash = response.infoHash;

        for (TorrentDownload* download : torrentDownloads) {

            if (download->getData().infoHash == infoHash) {
                TorrentDownloadData newData = conversions::aria2TellStatusResponse(response);
                download->setData(newData);
                break;
            };

        };

    };

};

void TorrentDownloadManager::onAria2cStarted() {
    statusTimer->start(1000);
};

void TorrentDownloadManager::onAria2cStopped() {
    statusTimer->stop();
};

QString TorrentDownloadManager::toQString() const {
    return torrentDownloads.toQString();
};

void TorrentDownloadManager::addDownload(
    TorrentDownloadData data
) {

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
