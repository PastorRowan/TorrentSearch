
#pragma once

#include <QObject>
#include <QString>

class TorrentDownload : public QObject {

    enum class Status {
        QueuedStatus,
        DownloadingStatus,
        PausedStatus,
        CompletedStatus,
        ErrorStatus
    };
    Q_ENUM(Status)

    Q_OBJECT

    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString infoHash READ getInfoHash NOTIFY infoHashChanged)
    Q_PROPERTY(int leechers READ getLeecher NOTIFY leechersChanged)
    Q_PROPERTY(int seeders READ getSeeders NOTIFY seedersChanged)
    Q_PROPERTY(long long sizeBytes READ getSizeBytes NOTIFY sizeBytesChanged)
    Q_PROPERTY(int numberOfFiles READ getNumberOfFiles NOTIFY numberOfFilesChanged)
    Q_PROPERTY(QString magnetUrl READ getMagnetUrl NOTIFY magnetUrlChanged)
    Q_PROPERTY(QString torrentUrl READ getTorrentUrl NOTIFY torrentUrlChanged)
    Q_PROPERTY(Status status READ getStatus NOTIFY statusChanged)
    Q_PROPERTY(float progress READ getProgress NOTIFY progressChanged)

    private:

        QString name = "";
        QString infoHash = "";
        int leechers = 0;
        int seeders = 0;
        long long sizeBytes = 0;
        int numberOfFiles = 0;
        QString magnetUrl = "";
        QString torrentUrl = "";

        Status status = Status::QueuedStatus;
        float progress = 0.0f;

    protected:

    public:

        TorrentDownload(
            QObject* parent = nullptr
        );

        QString getName();

        void setName(const QString& newName);

        QString getInfoHash();

        void setInfohash(const QString& newInfoHash);



        void download();

};
