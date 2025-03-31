import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import PasswordGenerator

Item {
    id: root
    anchors.fill: parent

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

    Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
    Material.accent: Material.Purple
    Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

    Rectangle {
        anchors.fill: root
        anchors.margins: 20
        radius: 20
        color: AppSettings.gui.theme === "Dark" ? "#333232" : "#F1F1F1"

        ColumnLayout {
            anchors.fill: parent
            spacing: 15
            anchors.margins: 20

            ScrollView {
                id: scrollView
                Layout.fillWidth: true
                Layout.fillHeight: true
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AsNeeded


                ColumnLayout {
                    width: scrollView.width
                    spacing: 15


                    // Title field
                    TextField {
                        Layout.topMargin: 5
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
                    RowLayout {
                        Layout.fillWidth: true

                        TextField {
                            id: passwordField
                            placeholderText: qsTr("Password")
                            Layout.fillWidth: true
                            echoMode: TextInput.Password

                            onAccepted: {
                                if (currentIndex !== -1) {
                                    noteController.setPassword(currentIndex, passwordField.text)
                                }
                            }
                        }

                        Button {
                            text: "📋"
                            Material.elevation: 2
                            Material.roundedScale: Material.ExtraSmallScale
                            Layout.preferredHeight: passwordField.height
                            Layout.preferredWidth: 64
                            onClicked: {
                                clipboard.copyText(passwordField.text)
                            }
                        }
                    }
                }
            }


            RowLayout {
                Layout.fillWidth: true
                Layout.margins: 20
                spacing: 10

                Item { Layout.fillWidth: true }  // Spacer

                Button {
                    text: qsTr("Save")
                    Material.elevation: 2
                    onClicked: {
                        noteController.editNote(currentIndex, titleField.text, urlField.text, usernameField.text, emailField.text, passwordField.text)
                    }
                }

                Button {
                    text: qsTr("Generate new password")
                    Material.elevation: 2
                    onClicked: {
                        passwordField.text = passwordGenerator.generatePassword()
                    }
                }

                Button {
                    text: qsTr("Close")
                    Material.elevation: 2
                    onClicked: {
                        closeRequested()
                    }
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
