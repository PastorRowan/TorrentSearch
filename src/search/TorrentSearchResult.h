
#pragma once

#include <QString>
#include <QMetaType>

struct TorrentSearchResult  {

    private:

        Q_GADGET

        Q_PROPERTY(QString name MEMBER name)
        Q_PROPERTY(QString infoHash MEMBER infoHash)
        Q_PROPERTY(int leechers MEMBER leechers)
        Q_PROPERTY(int seeders MEMBER seeders)
        Q_PROPERTY(qint64 sizeBytes MEMBER sizeBytes)
        Q_PROPERTY(int numberOfFiles MEMBER numberOfFiles)
        Q_PROPERTY(QString magnetUrl MEMBER magnetUrl)
        Q_PROPERTY(QString torrentUrl MEMBER torrentUrl)

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
        QString toQString() const;

};

Q_DECLARE_METATYPE(TorrentSearchResult)
