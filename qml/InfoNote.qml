import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import PasswordGenerator

Item {
    id: root
    anchors.fill: parent  // Fill all available space in the parent

    // Properties to access TextField values
    property alias title: titleField.text
    property alias url: urlField.text
    property alias username: usernameField.text
    property alias email: emailField.text
    property alias password: passwordField.text

    property int currentIndex: -1

    // Signals for button clicks
    signal urlFieldChanged(id: int, url: string)
    signal closeRequested()

    // Ensure Material style is applied (can be overridden by parent)
    Material.theme: Material.Dark
    Material.accent: Material.Purple
    Material.primary: Material.Grey

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        // Form fields
        ColumnLayout {
            spacing: 15
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Title field
            TextField {
                id: titleField
                placeholderText: qsTr("Title")
                Layout.fillWidth: true

                onAccepted: {
                    if (currentIndex !== -1) {
                        noteController.setTitle(currentIndex, titleField.text)
                    }
                }
            }

            // Url field
            TextField {
                id: urlField
                placeholderText: qsTr("Url")
                Layout.fillWidth: true

                onAccepted: {
                    if (currentIndex !== -1) {
                        noteController.setUrl(currentIndex, urlField.text)
                        urlFieldChanged(currentIndex, urlField.text)
                    }
                }
            }

            // Username field
            TextField {
                id: usernameField
                placeholderText: qsTr("Username")
                Layout.fillWidth: true

                onAccepted: {
                    if (currentIndex !== -1) {
                        noteController.setUsername(currentIndex, usernameField.text)
                    }
                }
            }

            // Email field
            TextField {
                id: emailField
                placeholderText: qsTr("Email")
                Layout.fillWidth: true

                onAccepted: {
                    if (currentIndex !== -1) {
                        noteController.setEmail(currentIndex, emailField.text)
                    }
                }
            }

            // Password field
            TextField {
                id: passwordField
                placeholderText: qsTr("Password")
                Layout.fillWidth: true
                // echoMode: TextInput.Password

                onAccepted: {
                    if (currentIndex !== -1) {
                        noteController.setPassword(currentIndex, passwordField.text)
                    }
                }
            }
        }

        // Buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Item { Layout.fillWidth: true }  // Spacer



            Button {
                text: qsTr("Save")
                Material.elevation: 2
                Material.background: "#403F3F"
                Material.foreground: "white"
                onClicked: {
                    noteController.editNote(currentIndex, titleField.text, urlField.text, usernameField.text, emailField.text, passwordField.text)
                }
            }
            Button {
                text: qsTr("Generate new password")
                Material.elevation: 2
                Material.background: "#403F3F"
                Material.foreground: "white"
                onClicked: {
                    passwordField.text = passwordGenerator.generatePassword()
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Item { Layout.fillWidth: true }  // Spacer
            Button {
                text: qsTr("Close")
                Material.elevation: 2
                Material.background: "#403F3F"
                Material.foreground: "white"
                onClicked: {
                    closeRequested()
                }
            }
        }

    }

    function setNote(id, title, url, username, email, password) {
        currentIndex = id
        root.title = title
        root.url = url
        root.username = username
        root.email = email
        root.password = password
    }

    PasswordGenerator {
        id: passwordGenerator
    }
}
