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

    Material.theme: Material.Dark
    Material.accent: Material.Purple
    Material.primary: Material.Grey

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
                    AppSettings.firstTime = false
                    AppSettings.password = passwordInput.text
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

    Popup {
        id: errorDialog
        width: 300
        height: 150
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape

        Material.theme: Material.Dark
        Material.accent: Material.Purple
        Material.primary: Material.Grey

        Column {
            anchors.centerIn: parent
            spacing: 10

            Text {
                text: qsTr("Could not load the storage")
                color: "white"
                font.pixelSize: 16
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }

            Button {
                text: qsTr("OK")
                onClicked: errorDialog.close()
                anchors.horizontalCenter: parent.horizontalCenter
            }
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
