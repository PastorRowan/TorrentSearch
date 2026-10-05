
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Page {

    id: root

    title: "Search"

    property var selectedSearchResults: torrentSearchResultsView.selectedSearchResults

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
                torrentSearchManager.search(text)
            }

        }

        TorrentSearchResultsView {

            id: torrentSearchResultsView

            model: searchPageTorrentSearchResultsModel

        }

    }

    RoundButton {

        id: downloadButton

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 24
        }

        width: 56
        height: 56

        text: "\u2B07"

        font.pixelSize: 28

        onClicked: {
            if (root.selectedSearchResults.length >= 1) {
                downloadDialog.open();
            } else {
                noSelectionDialog.open()
            };
        }

    }

    Dialog {

        id: noSelectionDialog

        title: "No torrent selected"
        modal: true
        width: 400

        anchors.centerIn: parent

        standardButtons: Dialog.Ok

    }

    Dialog {

        id: downloadDialog

        title: "Download Torrent"
        modal: true

        width: 400
        height: 500

        anchors.centerIn: parent

        standardButtons: Dialog.Ok | Dialog.Cancel

        contentItem: ColumnLayout {

            anchors.fill: parent
            spacing: 10

            Label {
                Layout.fillWidth: true
                Layout.preferredHeight: 20
                Layout.topMargin: 70
                text: "The following torrents will be downloaded:"
                wrapMode: Text.Wrap
            }

            ListView {

                id: torrentList

                Layout.fillWidth: true
                Layout.fillHeight: true

                clip: true

                model: root.selectedSearchResults

                delegate: Label {

                    width: ListView.view.width
                    height: 40
                    text: "• " + modelData.name

                    Component.onCompleted: {
                        console.log("modelData: ", modelData);
                    }

                }

            }

        }

        onAccepted: {
            const magnetUrls = root.selectedSearchResults.map(
                result => result.magnetUrl
            )
            torrentDownloadManager.addDownload(magnetUrls)
        }

        onRejected: {
            console.log("Download rejected")
        }

    }

}
