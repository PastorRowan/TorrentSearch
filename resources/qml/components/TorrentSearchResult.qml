
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {

    Layout.fillWidth: true
    Layout.preferredHeight: 50

    ColumnLayout {

        anchors.fill: parent

        Label {
            Layout.fillWidth: true
            Layout.preferredHeight: 14
            font.pixelSize: 14
            text: "Title"
        }

        Label {
            Layout.fillWidth: true
            Layout.preferredHeight: 12
            font.pixelSize: 12
            text: "Title value"
        }

    }

}
