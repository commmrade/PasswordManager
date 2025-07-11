import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import IconManager
import PasswordManager 1.0

Item {
    id: root
    width: parent.width
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    IconManager {
        id: iconManager
    }

    Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
    Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
    Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

    SplitView {
        id: splitView
        anchors.fill: parent  // Fill the root item

        // First element: Your original RowLayout
        RowLayout {
            id: globalRow
            SplitView.minimumWidth: 200  // Minimum width for resizing
            SplitView.preferredWidth: 300  // Initial width
            SplitView.fillHeight: true

            ColumnLayout {
                id: listColumn

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

                        color: AppSettings.gui.theme === "Dark" ? "#333232" : "#F1F1F1"
                        radius: 8

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.title
                            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
                            font.pixelSize: 20
                            Layout.leftMargin: 10
                        }
                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            onClicked: (mouse) => {
                                if (mouse.button == Qt.LeftButton) {
                                    emptyText.visible = false
                                    let password = noteController.getPassword(model.id)
                                    infoNote.setNote(model.id, model.title, model.url, model.username, model.email, password)
                                    infoNote.currentIndex = model.id
                                    infoNote.visible = true
                                } else {

                                    contextMenu.popup()
                                }
                            }
                        }

                    }
                }

                RowLayout {
                    id: listButtonsRow
                    Layout.leftMargin: 10

                    Button {
                        id: createNoteBtn
                        // width: listView.width / 2 - 30
                        text: qsTr("Create")
                        Material.elevation: 2
                        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
                        Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
                        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey


                        onClicked: {
                            noteCreator.open()
                        }
                    }
                    Button {
                        id: deleteNoteBtn
                        text: qsTr("Delete")
                        Material.elevation: 2
                        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
                        Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
                        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

                        onClicked: {
                            if (infoNote.currentIndex !== -1) {
                                noteController.deleteNote(infoNote.currentIndex)
                                resetNote()
                            }
                        }
                    }
                }
            }
        }


        StackView {
            id: stackView
            SplitView.minimumWidth: root.width / 3
            // SplitView.preferredWidth: 500
            SplitView.fillHeight: true

            Text {
                id: emptyText
                text: "Empty"
                wrapMode: Text.WordWrap
                anchors.centerIn: parent
                font.pointSize: 22
                font.italic: true
                color: AppSettings.gui.theme === "Dark" ? "white" : "black"
                Layout.fillWidth: true
                Layout.fillHeight: true

            }

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
            resetNote()
        }
    }

    NoteCreator {
        id: noteCreator
        anchors.centerIn: root
        height: root.height - root.height / 8
        width: root.width / 2 + root.width / 6
    }

    Menu {
        id: contextMenu
        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
        Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

        MenuItem {
            text: "Delete"
            onTriggered: {
                if (infoNote.currentIndex !== -1) {
                    noteController.deleteNote(infoNote.currentIndex)
                    root.resetNote()
                }
            }
        }
    }


    function resetNote() {
        emptyText.visible = true
        infoNote.visible = false
        infoNote.currentIndex = -1
    }
}
