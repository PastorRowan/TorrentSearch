
#include "download/TorrentDownloadManager.h"
#include "conversions/conversions.h"
#include "helpers/helpers.h"

#include <QUrl>
#include <QUrlQuery>
#include <QDebug>

TorrentDownloadManager::TorrentDownloadManager(
    QObject* parent
):
    QObject(parent),
    aria2c(new Aria2c(this)),
    statusTimer(new QTimer(this)),
    torrentDownloadDatas({}) {

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

    aria2c->start();

};

void TorrentDownloadManager::pollDownloadStatuses() {

    if (!aria2c->isRunning()) {
        return;
    };

    aria2c->systemMulticall({
        .methods = {
            {
                .method = RpcMethod::Aria2TellActive,
                .params = {}
            },
            {
                .method = RpcMethod::Aria2TellWaiting,
                .params = {
                    0,
                    torrentDownloadDatas.size() * 4
                }
            }
        }
    },
    this,
    [ this ](std::variant<SystemMulticallResponse, Aria2Error> response) {

        QVector<Aria2TellStatusResponse> responses = {};

        if (auto* err = std::get_if<Aria2Error>(&response)) {
            qDebug() << "Failed to call 'systemMulticall' with params" << err->message;
        } else if (auto* result = std::get_if<SystemMulticallResponse>(&response)) {
            for (const auto& entry : result->entries) {
                if (!entry.isSuccess()) {
                    qDebug() << "Entry has an error";
                    return;
                };
                const QJsonArray resultWrapper = entry.resultValue().toArray();
                const QJsonArray downloads = resultWrapper.at(0).toArray();
                for (const auto& download : downloads) {
                    responses.append(
                        Aria2TellStatusResponse::fromQJsonValue(download)
                    );
                };
            };
            onAria2cTellStatusResponses(responses);
        } else {
            qFatal() << "Error: 'systemMulticall' responded with an unknown type";
        };

    });

};

void TorrentDownloadManager::onAria2cTellStatusResponses(
    const QVector<Aria2TellStatusResponse>& responses
) {

    #define LOG_RESPONSES 0

    #if LOG_RESPONSES
        qDebug() << "TorrentDownloadManager::onAria2cTellStatusResponses called with:";
    #endif

    torrentDownloadDatas.clear();
    for (const Aria2TellStatusResponse& response : responses) {
        #if LOG_RESPONSES
            qDebug().noquote()
                << "  response.gid: " << response.gid << "\n"
                << "  response.u'bitfield: " << response.bitfield << "\n"
                << "  response.u'completedLength: " << response.completedLength << "\n"
                << "  response.connections: " << response.connections << "\n"
                << "  response.dir: " << response.dir << "\n"
                << "  response.downloadSpeed: " << response.downloadSpeed << "\n"
                << "  response.status: " << response.status << "\n"
                << "  response.totalLength: " << response.totalLength << "\n"
                << "  response.files:"
            ;
        for (const Aria2File& file : response.files) {
            qDebug().noquote()
                << "    file.index: " << file.index << "\n"
                << "    file.length: " << file.length << "\n"
                << "    file.completedLength: " << file.completedLength << "\n"
                << "    file.path: " << file.path << "\n"
                << "    file.selected: " << file.selected << "\n"
                << "    file.uris:"
            ;
            for (const Aria2Uri& uri : file.uris) {
                qDebug().noquote()
                    << "      uri.uri" << uri.uri << "\n"
                    << "      uri.status" << uri.status
                ;
            };
        };
        #endif
        #undef LOG_RESPONSES
        if (response.gid.isEmpty()) {
            continue;
        };
        TorrentDownloadData newTorrentDownloadData = conversions::aria2TellStatusResponseToTorrentDownloadData(response);
        torrentDownloadDatas.append(newTorrentDownloadData);
    };
    emit downloadDatasChanged(torrentDownloadDatas);
};

void TorrentDownloadManager::onAria2cStarted() {
    statusTimer->start(2000);
};

void TorrentDownloadManager::onAria2cStopped() {
    statusTimer->stop();
};

QString TorrentDownloadManager::toQString() const {
    return torrentDownloadDatas.toQString();
};

void TorrentDownloadManager::addDownload(
    const QStringList& magnetUrls
) {

    static const QStringList trackerList = {
        "http://tracker.opentrackr.org:1337/announce",
        "udp://open.stealth.si:80/announce",
        "udp://tracker.torrent.eu.org:451/announce",
        "udp://open.demonii.com:1337/announce",
        "http://tracker.qu.ax:6969/announce",
        "udp://tracker.skynetcloud.site:6969/announce",
        "udp://tracker.gmi.gd:6969/announce",
        "udp://tracker.tryhackx.org:6969/announce",
        "udp://explodie.org:6969/announce",
        "udp://tracker.theoks.net:6969/announce",
        "udp://tracker.nyaa.vc:6969/announce",
        "udp://tracker.corpscorp.online:80/announce",
        "udp://tracker.bittor.pw:1337/announce",
        "udp://tracker-udp.gbitt.info:80/announce",
        "http://tracker.dler.org:6969/announce",
        "udp://tracker2.dler.org:80/announce",
        "http://tracker.dler.com:6969/announce",
        "udp://tracker.ducks.party:1984/announce",
        "udp://retracker01-msk-virt.corbina.net:80/announce",
        "http://tracker.renfei.net:8080/announce"
    };

    try {

        QDir dir(downloadDirectory);
        dir.mkpath(".");

        for (const QString& magnetUrl : magnetUrls) {

            QUrl url(magnetUrl);
            QUrlQuery query(url);

            for (const QString& tracker : trackerList) {
                query.addQueryItem("tr", tracker);
            };

            url.setQuery(query);

            const QString magnetUrlWithTrackers = helpers::encodeMagnetUrl(url).toString();

            qDebug().noquote() << "Adding magnetUrl: " << magnetUrlWithTrackers;

            aria2c->aria2AddUri(
                {
                    .uris = { magnetUrl },
                    .tail1 = Aria2AddUriParams::Tail1{
                        .options = {
                            { "dir", downloadDirectory },
                            { "seed-time", "0" }
                        }
                    }
                },
                this,
                [](std::variant<Aria2AddUriResponse, Aria2Error> response) {
                    if (auto* err = std::get_if<Aria2Error>(&response)) {
                        qDebug().noquote() << "Failed to run method 'aria2AddTorrent': " << err->message;
                    } else if (auto* result = std::get_if<Aria2AddUriResponse>(&response)) {
                        qDebug().noquote() << "Successfully called 'aria2AddUri' with result.gid: " << result->gid;
                    } else {
                        qFatal("Error: 'aria2AddUri' responded with an unknown type");
                    };
                }
            );
        };

    } catch (const std::exception& err) {
        qDebug().noquote()
            << "Error: failed to call TorrentDownloadManager::addDownload with magnetUrls '" << magnetUrls << "'\n"
            << "err.what(): " << err.what()
        ;
    };

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
