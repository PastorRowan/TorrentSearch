
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QVariant>

#include "search/TorrentSearchManager.h"
#include "search/TorrentSearchResultsModel.h"
// #include "download/TorrentDownloadManager.h"

int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    TorrentSearchManager torrentSearchManager;

    TorrentSearchResultsModel searchPageTorrentSearchResultsModel;

    // TorrentDownloadManager torrentDownloadManager

    // TorrentDownloadsModel torrentDownloadsModel

    // QObject::connect( somehow connect them?

    QObject::connect(
        &torrentSearchManager,
        &TorrentSearchManager::searchResultsUpdated,
        &searchPageTorrentSearchResultsModel,
        &TorrentSearchResultsModel::setResults
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
        /*
        {
            "torrentDownloadManager",
            QVariant::fromValue()
        }
        */
    });

    engine.loadFromModule("TorrentSearch", "Main");

    return app.exec();

};
