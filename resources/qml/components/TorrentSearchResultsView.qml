
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ListView {

    id: root

    signal downloadRequested(string name, string magnetUrl)

    Layout.fillWidth: true
    Layout.fillHeight: true
    clip: true

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
                text: "Seeders"
                font.bold: true
            }

        }

    }

    delegate: Rectangle {

        width: root.width
        height: 48
        color: index % 2 === 0 ? "#ffffff" : "#f7f7f7"

        RowLayout {

            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            spacing: 16

            Label {
                Layout.fillWidth: true
                text: name
                elide: Text.ElideRight
            }

            Label {
                Layout.preferredWidth: 100
                text: size
            }

            Label {
                Layout.preferredWidth: 80
                horizontalAlignment: Text.AlignRight
                text: seeders
            }

            Button {
                Layout.preferredWidth: 40
                Layout.fillHeight:true
                onClicked: console.log("Download: ", name, magnetUrl)
            }

        }

    }

}
