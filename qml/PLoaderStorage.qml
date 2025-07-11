import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore
// import PasswordManager
import StorageManager
import AuthManager

Dialog {
    id: root
    modal: true
    title: "Prompt"

    Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
    Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
    Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

    implicitWidth: 400
    implicitHeight: 500

    property AuthManager authManager: AuthManager{}
    property StorageManager storageManager: StorageManager {}

    Connections {
        target: storageManager

        function onSuccess() {
            noteController.resetStorage()
        }

        function onError(statusCode, errorMessage) {
            storageErrorDialog.text = errorMessage
            storageErrorDialog.open()

            if (statusCode === 401) {
                authManager.updateToken()
            }
        }
    }

    onAccepted: {
        passwordInput.text = ""
        root.close()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20


        // Поле для ввода пароля
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10

            Label {
                text: qsTr("Enter the master password of the backup:")
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
                    storageManager.loadStorage(passwordInput.text)
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

}
