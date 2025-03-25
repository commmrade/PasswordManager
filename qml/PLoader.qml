import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import PasswordManager
import LoaderController
Dialog {
    id: root
    modal: true
    title: "File Selection" // Optional: adds a title to the dialog

    Material.theme: Material.Dark
    Material.accent: Material.Purple
    Material.primary: Material.Grey

    implicitWidth: 400
    implicitHeight: 200

    LoaderController {
        id: loaderController
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
                text: "Choose a file"
                color: "white"
                font.pixelSize: 16
            }

            TextField {
                id: filePathInput
                Layout.fillWidth: true
                readOnly: true
                text: root.filePath
                placeholderText: "Click to select a file"
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
                text: "Enter password:"
                color: "white"
                font.pixelSize: 16
            }

            TextField {
                id: passwordInput
                Layout.fillWidth: true
                placeholderText: "Password..."
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
                text: "Load"
                highlighted: true
                onClicked: {
                    console.log("File:", filePathInput.text, "Password:", passwordInput.text)
                    if (!loaderController.loadStorage(filePathInput.text, passwordInput.text)) {
                        console.log("Damn error");
                        return
                    }
                    AppSettings.firstTime = false
                    AppSettings.password = passwordInput.text
                    root.accept()
                }
            }

            Button {
                text: "Close"
                onClicked: {
                    root.reject()
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Select a File"
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0] // Use first home location
        nameFilters: ["PM files (*.pm)"]
        onAccepted: {
            root.filePath = fileDialog.currentFile
        }
        onRejected: {
            console.log("File selection canceled")
        }
    }

    // Свойство для хранения пути к файлу
    property string filePath: ""
}