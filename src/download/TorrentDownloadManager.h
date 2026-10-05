
#pragma once

#include "Aria2c/Aria2c.h"
#include "download/TorrentDownloadDatas.h"

#include <QObject>
#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <QStringList>

class TorrentDownloadManager : public QObject {

    Q_OBJECT

    private:

        QString downloadDirectory =
            QDir(
                QStandardPaths::writableLocation(
                    QStandardPaths::DownloadLocation
                )
            ).filePath("TorrentSearch");

        Aria2c* aria2c;

        QTimer* statusTimer;

        TorrentDownloadDatas torrentDownloadDatas;

        void pollDownloadStatuses();

    // private slots:

        void onAria2cTellStatusResponses(
            const QVector<Aria2TellStatusResponse>& responses
        );

        void onAria2cStarted();

        void onAria2cStopped();

    protected:

    public:

        explicit TorrentDownloadManager(
            QObject* parent = nullptr
        );

        QString toQString() const;

        Q_INVOKABLE void addDownload(
            const QStringList& magnetUrls
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

        void downloadDatasChanged(
            const TorrentDownloadDatas& torrentDownloads
        );

};
