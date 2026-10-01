
#pragma once

#include <QMetaType>
#include <QString>

struct TorrentDownloadData {

    enum class Status {
        QueuedStatus,
        DownloadingStatus,
        PausedStatus,
        CompletedStatus,
        CancelledStatus,
        ErrorStatus
    };
    Q_ENUM(Status)

    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString infoHash MEMBER infoHash)
    Q_PROPERTY(int leechers MEMBER leechers)
    Q_PROPERTY(int seeders MEMBER seeders)
    Q_PROPERTY(long long sizeBytes MEMBER sizeBytes)
    Q_PROPERTY(int numberOfFiles MEMBER numberOfFiles)
    Q_PROPERTY(QString magnetUrl MEMBER magnetUrl)
    Q_PROPERTY(QString torrentUrl MEMBER torrentUrl)
    Q_PROPERTY(QString gid MEMBER gid)
    Q_PROPERTY(Status status MEMBER status)
    Q_PROPERTY(float progress MEMBER progress)

    private:

    protected:

    public:

        QString name = "";
        QString infoHash = "";
        int leechers = 0;
        int seeders = 0;
        long long sizeBytes = 0;
        int numberOfFiles = 0;
        QString magnetUrl = "";
        QString torrentUrl = "";

        QString gid;
        Status status = Status::QueuedStatus;
        float progress = 0.0f;

        QString statusToQString() const;

        QString toQString() const;

        friend bool operator==(
            const TorrentDownloadData& leftData,
            const TorrentDownloadData& rightData
        );

};

Q_DECLARE_METATYPE(TorrentDownloadData)
