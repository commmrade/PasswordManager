import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import PasswordGenerator
import QtQuick.Dialogs

Dialog {
    id: root
    modal: true
    // Properties to access TextField values
    property alias titleNote: titleField.text
    property alias url: urlField.text
    property alias username: usernameField.text
    property alias email: emailField.text
    property alias password: passwordField.text



    // Ensure Material style is applied (can be overridden by parent)
    Material.theme: Material.Dark
    Material.accent: Material.Purple
    Material.primary: Material.Grey

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        // Title
        Text {
            text: "Create your note"
            color: "white"
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
                placeholderText: "Title"
                Layout.fillWidth: true
                Material.foreground: "white"
                Material.background: "#403F3F"
            }

            // Url field
            TextField {
                id: urlField
                placeholderText: "Url"
                Layout.fillWidth: true
                Material.foreground: "white"
                Material.background: "#403F3F"
            }

            // Username field
            TextField {
                id: usernameField
                placeholderText: "Username"
                Layout.fillWidth: true
                Material.foreground: "white"
                Material.background: "#403F3F"
            }

            // Email field
            TextField {
                id: emailField
                placeholderText: "Email"
                Layout.fillWidth: true
                Material.foreground: "white"
                Material.background: "#403F3F"
            }

            // Password field
            TextField {
                id: passwordField
                placeholderText: "Password"
                Layout.fillWidth: true
                Material.foreground: "white"
                Material.background: "#403F3F"
                // echoMode: TextInput.PasswordEchoOnEdit
            }
        }

        // Buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Item { Layout.fillWidth: true }  // Spacer

            Button {
                text: "Generate password"
                Material.elevation: 2
                Material.background: "#403F3F"
                Material.foreground: "white"
                onClicked: {
                    passwordField.text = passwordGenerator.generatePassword()
                }
            }

            Button {
                text: "Create"
                Material.elevation: 2
                Material.background: "#403F3F"
                Material.foreground: "white"
                onClicked: {
                    if (titleField.text === "" || passwordField.text === "") {
                        warningDialog.open()
                        return
                    }

                    noteController.createNote(titleField.text, urlField.text, usernameField.text, emailField.text, passwordField.text)
                    root.close()
                }
            }
            Button {
                text: "Close"
                Material.elevation: 2
                Material.background: "#403F3F"
                Material.foreground: "white"
                onClicked: {
                    root.close()
                }
            }
        }
    }
    PasswordGenerator {
        id: passwordGenerator
    }
    MessageDialog {
        id: warningDialog
        title: "Title or Password Empty"
        text: "Please, make sure both fields are filled with text."
        buttons: MessageDialog.Ok
    }
}
