
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Page {

    title: "Search"

    ColumnLayout {

        anchors.fill: parent
        anchors.margins: 20

        Label {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            text: "Search"
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        TextField {
            Layout.preferredHeight: 50
            Layout.fillWidth: true
            placeholderText: "Search torrents..."
        }

        TorrentSearchResult {

        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

    }

}
