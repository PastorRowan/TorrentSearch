
#include "search/TorrentSearchManager.h"
#include "search/TorrentSearchResultsModel.h"
#include "download/TorrentDownloadManager.h"
#include "download/TorrentDownloadDatasModel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QVariant>

int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    TorrentSearchManager torrentSearchManager;

    TorrentSearchResultsModel searchPageTorrentSearchResultsModel;

    TorrentDownloadManager torrentDownloadManager;

    TorrentDownloadDatasModel downloadsPageTorrentDownloadDatasModel;

    QObject::connect(
        &torrentSearchManager,
        &TorrentSearchManager::searchResultsUpdated,
        &searchPageTorrentSearchResultsModel,
        &TorrentSearchResultsModel::setResults
    );

    QObject::connect(
        &torrentDownloadManager,
        &TorrentDownloadManager::downloadDatasChanged,
        &downloadsPageTorrentDownloadDatasModel,
        &TorrentDownloadDatasModel::setTorrentDownloadDatas
    );

    engine.rootContext()->setContextProperties({
        {
            "torrentSearchManager",
            QVariant::fromValue(&torrentSearchManager)
        },
        {
            "searchPageTorrentSearchResultsModel",
            QVariant::fromValue(&searchPageTorrentSearchResultsModel)
        },
        {
            "torrentDownloadManager",
            QVariant::fromValue(&torrentDownloadManager)
        },
        {
            "downloadsPageTorrentDownloadDatasModel",
            QVariant::fromValue(&downloadsPageTorrentDownloadDatasModel)
        },
    });

    engine.loadFromModule("TorrentSearch", "Main");

    return app.exec();

};
