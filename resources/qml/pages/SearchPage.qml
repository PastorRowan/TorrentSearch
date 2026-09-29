
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Page {

    id: root

    title: "Search"

    property string selectedMagnetUrl: ""
    property string selectedTorrentName: ""

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
            text: ""
            placeholderText: "Search torrents..."

            onAccepted: {
                torrentProviderManager.search(text)
            }

        }

        TorrentSearchResultsView {

            id: torrentSearchResultsView

            model: searchPageTorrentSearchResultsModel

            onDownloadRequested: function(name, magnetUrl) {
                root.selectedTorrentName = name
                root.selectedMagnetUrl = magnetUrl
                downloadDialog.open()
            }

        }

    }

    Dialog {

        id: downloadDialog

        title: "Download Torrent"
        modal: true

        width: 400

        anchors.centerIn: parent

        standardButtons: Dialog.Ok | Dialog.Cancel

        contentItem: ColumnLayout {

            Label {
                Layout.fillWidth: true
                text: root.selectedTorrentName
                wrapMode: Text.Wrap
            }

            Label {
                Layout.fillWidth: true
                text: "Download this torrent?"
            }

        }

        /*
        onAccepted: {
            torrentProviderManager.download(root.selectedMagnetUrl)
        }
        */

    }

}
