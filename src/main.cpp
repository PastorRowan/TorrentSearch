
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "providers/TorrentProviderManager.h"

int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    engine.loadFromModule("TorrentSearch", "Main");

    TorrentProviderManager torrentProviderManager;

    torrentProviderManager.search("ubuntu");

    return app.exec();

};
