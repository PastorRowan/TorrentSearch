
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {

    property StackView stackView

    RowLayout {
        anchors.fill: parent

        Button {
            text: "Search"
            onClicked: stackView.replace("../pages/SearchPage.qml")
        }

        Button {
            text: "Downloads"
            onClicked: stackView.replace("../pages/DownloadsPage.qml")
        }

        Button {
            text: "Settings"
            onClicked: stackView.replace("../pages/SettingsPage.qml")
        }
    }
}
