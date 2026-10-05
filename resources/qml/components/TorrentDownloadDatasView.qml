
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ListView {

    id: root

    property var selectedTorrentDownloadDatas: [];

    function toggleSelection(torrentDownloadData) {

        const alreadySelected = selectedTorrentDownloadDatas.some(
            result => result.magnetUrl === torrentDownloadData.magnetUrl
        );

        if (alreadySelected) {
            selectedTorrentDownloadDatas = selectedTorrentDownloadDatas.filter(
                result => result.magnetUrl !== torrentDownloadData.magnetUrl
            );
        } else {
            selectedTorrentDownloadDatas = [
                ...selectedTorrentDownloadDatas,
                torrentDownloadData
            ];
        };

    }

    Layout.fillWidth: true;
    Layout.fillHeight: true;
    clip: true;

    header: Rectangle {

        width: root.width
        height: 36
        color: "#eeeeee"

        RowLayout {

            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            spacing: 16

            Label {
                Layout.fillWidth: true
                text: "Name"
                font.bold: true
            }

            Label {
                Layout.preferredWidth: 100
                text: "Size"
                font.bold: true
            }

            Label {
                Layout.preferredWidth: 80
                horizontalAlignment: Text.AlignRight
                text: "Peers"
                font.bold: true
            }

        }

    }

    delegate: Rectangle {

        width: root.width
        height: 48

        property bool selected: {
            for (const downloadData of root.selectedTorrentDownloadDatas) {
                if (downloadData.magnetUrl === torrentDownloadData.magnetUrl) {
                    return true;
                };
            };
            return false;
        }

        color: selected
            ? "#cce5ff"
            : (index % 2 === 0 ? "#ffffff" : "#f7f7f7")

        ColumnLayout {

            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12

            RowLayout {

                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 16

                Label {
                    Layout.fillWidth: true
                    text: torrentDownloadData.name
                    elide: Text.ElideRight
                }

                Label {
                    Layout.preferredWidth: 100
                    text: torrentDownloadData.sizeBytes
                }

                Label {
                    Layout.preferredWidth: 80
                    horizontalAlignment: Text.AlignRight
                    text: torrentDownloadData.seeders + torrentDownloadData.leechers
                }

            }

            ProgressBar {

                Layout.fillWidth: true

                from: 0.0
                to: 1.0
                value: torrentDownloadData.progress
                indeterminate: false
            }

        }

        MouseArea {

            id: mouseArea

            anchors.fill: parent
            hoverEnabled: true

            onClicked: {
                console.log("calling root.toggleSelection with");
                console.log(`torrentDownloadData:\n${torrentDownloadData}`);
                root.toggleSelection(torrentDownloadData);
            }

        }

    }

}
