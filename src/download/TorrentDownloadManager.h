
#pragma once

#include "Aria2c/Aria2c.h"
#include "download/TorrentDownloads.h"

#include <QObject>
#include <QString>
#include <QStandardPaths>
#include <QTimer>

class TorrentDownloadManager : public QObject {

    Q_OBJECT

    private:

        QString downloadDirectory = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/TorrentSearch";

        Aria2c* aria2c;

        QTimer* statusTimer;

        TorrentDownloads<TorrentDownload*> torrentDownloads;

        void pollDownloadStatuses();

    // private slots:

        void onAria2cTellStatusResponses(
            const QVector<Aria2TellStatusResponse> responses
        );

        void onAria2cStarted();

        void onAria2cStopped();

    protected:

    public:

        explicit TorrentDownloadManager(
            QObject* parent = nullptr
        );

        QString toQString() const;

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

        void aria2cTellStatusResponses(
            const QVector<Aria2TellStatusResponse> responses
        );

};
