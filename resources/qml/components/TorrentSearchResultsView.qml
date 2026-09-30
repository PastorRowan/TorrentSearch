
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ListView {

    id: root

    property var selectedSearchResults: [];

    function toggleSelection(torrentSearchResult) {

        // Loop over selected search results
        for (let i = 0; i < selectedSearchResults.length; ++i) {
            // If toggled search result is already in selectedSearchResults
            // Then remove the search result and notify of state change
            if (selectedSearchResults[i].magnetUrl === torrentSearchResult.magnetUrl) {
                selectedSearchResults.splice(i, 1);
                selectedSearchResultsChanged();
                return;
            };
        };

        // Otherwise, add the new search result into selected search results
        // and notify of state change
        selectedSearchResults.push(torrentSearchResult);
        selectedSearchResultsChanged();

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
            for (const result of root.selectedSearchResults) {
                if (result.magnetUrl === torrentSearchResult.magnetUrl) {
                    return true;
                };
            };
            return false;
        }

        color: selected
            ? "#cce5ff"
            : (index % 2 === 0 ? "#ffffff" : "#f7f7f7")

        RowLayout {

            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            spacing: 16

            Label {
                Layout.fillWidth: true
                text: torrentSearchResult.name
                elide: Text.ElideRight
            }

            Label {
                Layout.preferredWidth: 100
                text: torrentSearchResult.sizeBytes
            }

            Label {
                Layout.preferredWidth: 80
                horizontalAlignment: Text.AlignRight
                text: torrentSearchResult.seeders + torrentSearchResult.leechers
            }

        }

        MouseArea {

            id: mouseArea

            anchors.fill: parent
            hoverEnabled: true

            onClicked: {
                console.log("calling root.toggleSelection with");
                console.log(`torrentSearchResult:\n${torrentSearchResult}`);
                root.toggleSelection(torrentSearchResult);
            }

        }

    }

}
