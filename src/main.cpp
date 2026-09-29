
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QVariant>

#include "providers/TorrentProviderManager.h"
#include "search/TorrentSearchResultsModel.h"

int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    TorrentProviderManager torrentProviderManager;

    TorrentSearchResultsModel searchPageTorrentSearchResultsModel;

    QObject::connect(
        &torrentProviderManager,
        &TorrentProviderManager::searchResultsUpdated,
        &searchPageTorrentSearchResultsModel,
        &TorrentSearchResultsModel::setResults
    );

    engine.rootContext()->setContextProperties({
        {
            "torrentProviderManager",
            QVariant::fromValue(&torrentProviderManager)
        },
        {
            "searchPageTorrentSearchResultsModel",
            QVariant::fromValue(&searchPageTorrentSearchResultsModel)
        }
    });

    engine.loadFromModule("TorrentSearch", "Main");

    return app.exec();

};
