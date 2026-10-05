
import QtQuick
import QtQuick.Controls

import "components"
import "pages"

ApplicationWindow {

    visible: true
    width: 800
    height: 600
    title: "TorrentSearch"

    AppHeader {
        id: header
        stackView: stackView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    StackView {

        id: stackView

        anchors {
                top: header.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

        initialItem: SearchPage {}

    }

}
