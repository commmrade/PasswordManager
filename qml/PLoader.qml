import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore
import PasswordManager
import LoaderController
Dialog {
    id: root
    modal: true
    title: "File Selection" // Optional: adds a title to the dialog
    Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
    Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
    Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

    implicitWidth: 400
    implicitHeight: 200

    LoaderController {
        id: loaderController
    }
    onAccepted: {
        filePathInput.text = ""
        passwordInput.text = ""
        root.close()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // Поле для выбора файла
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: qsTr("Choose a file")
                color: "white"
                font.pixelSize: 16
            }

            TextField {
                id: filePathInput
                Layout.fillWidth: true
                readOnly: true
                text: root.filePath
                placeholderText: qsTr("Click to select a file")
                Material.foreground: "white"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        fileDialog.open()
                    }
                }
            }
        }

        // Поле для ввода пароля
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: qsTr("Enter password:")
                color: "white"
                font.pixelSize: 16
            }

            TextField {
                id: passwordInput
                Layout.fillWidth: true
                placeholderText: qsTr("Password...")
                echoMode: TextInput.Password // Режим пароля (точки вместо символов)
                Material.foreground: "white"
            }
        }

        // Кнопки Load и Close
        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            spacing: 10

            Button {
                text: qsTr("Load")
                highlighted: true
                onClicked: {
                    if (!loaderController.loadStorage(filePathInput.text, passwordInput.text)) {
                        errorDialog.open();
                        return
                    }
                    AppSettings.general.firstTime = false
                    AppSettings.security.password = passwordInput.text
                    root.accept()
                }
            }

            Button {
                text: qsTr("Close")
                onClicked: {
                    root.reject()
                }
            }
        }
    }

    Dialog {
        id: errorDialog

        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
        Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

        Text {
            text: qsTr("Could not load the storage")
            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Select a File")
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0] // Use first home location
        nameFilters: ["PM files (*.pm)"]
        onAccepted: {
            root.filePath = fileDialog.currentFile
        }
        onRejected: {
        }
    }

    // Свойство для хранения пути к файлу
    property string filePath: ""
}
