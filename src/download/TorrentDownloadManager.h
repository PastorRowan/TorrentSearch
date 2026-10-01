
#pragma once

#include "download/TorrentDownloads.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QTimer>

class TorrentDownloadManager : QObject {

    Q_OBJECT

    private:

        TorrentDownloads<TorrentDownload*> torrentDownloads;

        QTimer statusTimer;

        void pollDownloadStatuses();

    protected:

    public:

        explicit TorrentDownloadManager(
            QObject* parent = nullptr
        );

        QString toQString() const;

        void requestAria2c(
            const QString& method,
            const QJsonArray& params,
            std::function<void(QNetworkReply*)> callback
        ) {

        };

        void addDownload(
            TorrentDownloadData data
        );

        void resumeDownload(
            const QString& infoHash
        );

        void pauseDownload(
            const QString& infoHash
        );

        void cancelDownload(
            const QString& infoHash
        );

    signals:

        void downloadUpdated(
            const QString& infoHash
        );

};
