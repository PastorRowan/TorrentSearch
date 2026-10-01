
#pragma once

#include "download/TorrentDownloadData.h"

#include <QObject>
class QString;

class TorrentDownload : public QObject {

    Q_OBJECT

    Q_PROPERTY(
        TorrentDownloadData data
        MEMBER data
        READ getData
        WRITE setData
        NOTIFY dataChanged
    )

    private:

        TorrentDownloadData data;

    protected:

    public:

        explicit TorrentDownload(
            TorrentDownloadData dataP,
            QObject* parent = nullptr
        );

        const TorrentDownloadData& getData() const;

        void setData(
            const TorrentDownloadData& newData
        );

        /*

        void setName();

        void setInfoHash();

        void setLeechers();

        void setSeeders();

        void setSizeBytes();

        void setNumberOfFiles();

        void setMagnetUrl();

        void setTorrentUrl();

        void setProgress();

        */

        QString toQString() const;

        void resume();

        void pause();

        void cancel();

    signals:

        void dataChanged();

};
