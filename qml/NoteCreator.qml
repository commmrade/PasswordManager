import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import PasswordGenerator
import QtQuick.Dialogs
import PasswordManager

Dialog {
    id: root
    modal: true
    // Properties to access TextField values
    property alias titleNote: titleField.text
    property alias url: urlField.text
    property alias username: usernameField.text
    property alias email: emailField.text
    property alias password: passwordField.text

    onClosed: {
        reset()
    }

    // Ensure Material style is applied (can be overridden by parent)
    Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
    Material.accent: Material.Purple
    Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        // Title
        Text {
            text: qsTr("Create new note")
            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
            font.pixelSize: 24
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        // Form fields
        ColumnLayout {
            spacing: 15
            Layout.fillWidth: true

            // Title field
            TextField {
                id: titleField
                placeholderText: qsTr("Title")
                Layout.fillWidth: true
                Material.background: AppSettings.gui.theme === "Dark" ? "#403F3F" : "#FFFFFF"
                Material.foreground: AppSettings.gui.theme === "Dark" ? "white" : "black"
            }

            // Url field
            TextField {
                id: urlField
                placeholderText: qsTr("Url")
                Layout.fillWidth: true
                Material.background: AppSettings.gui.theme === "Dark" ? "#403F3F" : "#FFFFFF"
                Material.foreground: AppSettings.gui.theme === "Dark" ? "white" : "black"
            }

            // Username field
            TextField {
                id: usernameField
                placeholderText: qsTr("Username")
                Layout.fillWidth: true
                Material.background: AppSettings.gui.theme === "Dark" ? "#403F3F" : "#FFFFFF"
                Material.foreground: AppSettings.gui.theme === "Dark" ? "white" : "black"
            }

            // Email field
            TextField {
                id: emailField
                placeholderText: qsTr("Email")
                Layout.fillWidth: true
                Material.background: AppSettings.gui.theme === "Dark" ? "#403F3F" : "#FFFFFF"
                Material.foreground: AppSettings.gui.theme === "Dark" ? "white" : "black"
            }

            // Password field
            TextField {
                id: passwordField
                placeholderText: qsTr("Password")
                Layout.fillWidth: true
                Material.background: AppSettings.gui.theme === "Dark" ? "#403F3F" : "#FFFFFF"
                Material.foreground: AppSettings.gui.theme === "Dark" ? "white" : "black"
                // echoMode: TextInput.PasswordEchoOnEdit
            }
        }

        // Buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Item { Layout.fillWidth: true }  // Spacer

            Button {
                text: qsTr("Generate password")
                Material.elevation: 2

                onClicked: {
                    passwordField.text = passwordGenerator.generatePassword()
                }
            }

            Button {
                text: qsTr("Create")
                Material.elevation: 2

                onClicked: {
                    if (titleField.text === "" || passwordField.text === "") {
                        warningDialog.open()
                        return
                    }

                    noteController.createNote(titleField.text, urlField.text, usernameField.text, emailField.text, passwordField.text)
                    reset()
                    root.close()
                }
            }
            Button {
                text: qsTr("Close")
                Material.elevation: 2

                onClicked: {
                    reset()
                    root.close()
                }
            }
        }
    }
    PasswordGenerator {
        id: passwordGenerator
    }


    Dialog {
        id: warningDialog
        title: qsTr("Title or Password Empty")
        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
        Material.accent: Material.Purple
        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey
        Text {
            text: qsTr("Please, make sure both fields are filled with text.")
            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
        }
        standardButtons: Dialog.Ok
    }

    function reset() {
        titleField.text = ""
        urlField.text = ""
        usernameField.text = ""
        emailField.text = ""
        passwordField.text = ""
    }
}
