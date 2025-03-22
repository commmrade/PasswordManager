import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import IconManager

Item {
    id: root
    width: parent.width
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    IconManager {
        id: iconManager
    }

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

                        Row {
                            anchors.centerIn: parent
                            spacing: 10

                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                text: model.title
                                color: "white"
                                font.pixelSize: 20
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                console.log("Clicked on item with title:", model.title, "and ID:", model.id)
                                let password = noteController.getPassword(model.id)
                                console.log(password)
                                infoNote.setNote(model.id, model.title, model.url, model.username, model.email, password)
                                infoNote.currentIndex = model.id
                                infoNote.visible = true
                            }
                        }

                        Component.onCompleted: {
                            if (!iconManager.hasIcon(model.id)) {
                                let link = model.url + "/favicon.ico"
                                iconManager.downloadImage(link, model.id)
                            }
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

                        NoteCreator {
                            id: noteCreator
                            x: (root.width - width) / 2  // Center horizontally
                            y: -(root.height - height) * 8   // Center vertically
                            height: root.height - 100
                            width: root.width - 100
                        }

                        onClicked: {
                            noteCreator.open()
                        }
                    }
                    Button {
                        id: deleteNoteBtn
                        width: listView.width / 2 - 30
                        text: "Delete"
                        Material.elevation: 2
                        Material.background: "#403F3F"

                        onClicked: {
                            noteController.deleteNote(infoNote.currentIndex)
                            infoNote.visible = false
                            infoNote.currentIndex = -1
                        }
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
                visible: false
            }
        }
    }

    Connections {
        target: infoNote
        function onUrlFieldChanged(id, url) {
            let urlFull = url + "/favicon.ico"
            iconManager.downloadImage(urlFull, id)
        }
        function onCloseRequested() {
            console.log("hi")
            infoNote.visible = false
            infoNote.currentIndex = -1
        }
    }
}
