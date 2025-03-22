import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: root
    width: parent.width
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    Material.theme: Material.Dark
    Material.accent: Material.Purple
    Material.primary: Material.Grey

    SplitView {
        id: splitView
        anchors.fill: parent  // Fill the root item

        // First element: Your original RowLayout
        RowLayout {
            id: globalRow
            SplitView.minimumWidth: 100  // Minimum width for resizing
            SplitView.preferredWidth: 300  // Initial width
            SplitView.fillHeight: true

            ColumnLayout {
                id: listColumn
                Layout.maximumWidth: 300
                Layout.minimumWidth: 100
                Layout.fillHeight: true
                Layout.fillWidth: true

                ListView {
                    id: listView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 10

                    model: noteController.getModel()
                    delegate: Rectangle {
                        width: parent.width
                        height: 50
                        color: "#403F3F"
                        radius: 8

                        Text {
                            anchors.centerIn: parent
                            text: model.url
                            color: "white"  // Changed to white for dark theme
                            font.pixelSize: 20
                        }
                    }
                }

                RowLayout {
                    id: listButtonsRow
                    spacing: 30

                    Button {
                        id: createNoteBtn
                        width: listView.width / 2 - 30
                        text: "Create"
                        Material.elevation: 2
                        Material.background: "#403F3F"
                    }
                    Button {
                        id: deleteNoteBtn
                        width: listView.width / 2 - 30
                        text: "Delete"
                        Material.elevation: 2
                        Material.background: "#403F3F"
                    }
                }
            }
        }


        StackView {
            id: stackView
            SplitView.minimumWidth: 50
            SplitView.preferredWidth: 150
            SplitView.fillHeight: true


            InfoNote {
                id: infoNote
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
