
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "pages"

ApplicationWindow {

    visible: true
    width: 800
    height: 600
    title: "TorrentSearch"

    StackView {

        id: stackView

        anchors.fill: parent

        initialItem: SearchPage {}

    }

}
