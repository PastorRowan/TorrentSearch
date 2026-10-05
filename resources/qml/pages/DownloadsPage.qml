
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Page {

    id: root

    title: "Downloads"

    ColumnLayout {

        anchors.fill: parent
        anchors.margins: 20

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 100
        }

        TorrentDownloadDatasView {

            id: torrentDownloadDatasView

            model: downloadsPageTorrentDownloadDatasModel

        }

    }

}
