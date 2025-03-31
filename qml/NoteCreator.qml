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
    property alias titleNote: titleField.text
    property alias url: urlField.text
    property alias username: usernameField.text
    property alias email: emailField.text
    property alias password: passwordField.text

    onClosed: reset()

    Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
    Material.accent: Material.Purple
    Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        Text {
            id: titleText
            text: qsTr("Create new note")
            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
            font.pixelSize: 24
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        ScrollView {
            id: scrollView
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            ColumnLayout {
                width: scrollView.width
                spacing: 10

                TextField {
                    id: titleField
                    Layout.topMargin: 5
                    placeholderText: qsTr("Title")
                    Layout.fillWidth: true
                    Layout.maximumHeight: 50
                    Material.background: AppSettings.gui.theme === "Dark" ? "#403F3F" : "#FFFFFF"
                    Material.foreground: AppSettings.gui.theme === "Dark" ? "white" : "black"
                }

                TextField {
                    id: urlField
                    placeholderText: qsTr("Url")
                    Layout.fillWidth: true
                    Layout.maximumHeight: 50
                    Material.background: AppSettings.gui.theme === "Dark" ? "#403F3F" : "#FFFFFF"
                    Material.foreground: AppSettings.gui.theme === "Dark" ? "white" : "black"
                }

                TextField {
                    id: usernameField
                    placeholderText: qsTr("Username")
                    Layout.fillWidth: true
                    Layout.maximumHeight: 50
                    Material.background: AppSettings.gui.theme === "Dark" ? "#403F3F" : "#FFFFFF"
                    Material.foreground: AppSettings.gui.theme === "Dark" ? "white" : "black"
                }

                TextField {
                    id: emailField
                    placeholderText: qsTr("Email")
                    Layout.fillWidth: true
                    Layout.maximumHeight: 50
                    Material.background: AppSettings.gui.theme === "Dark" ? "#403F3F" : "#FFFFFF"
                    Material.foreground: AppSettings.gui.theme === "Dark" ? "white" : "black"
                }

                TextField {
                    id: passwordField
                    placeholderText: qsTr("Password")
                    Layout.fillWidth: true
                    Layout.maximumHeight: 50
                    Material.background: AppSettings.gui.theme === "Dark" ? "#403F3F" : "#FFFFFF"
                    Material.foreground: AppSettings.gui.theme === "Dark" ? "white" : "black"
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Item { Layout.fillWidth: true }

            Button {
                text: qsTr("Generate password")
                Material.elevation: 2
                onClicked: passwordField.text = passwordGenerator.generatePassword()
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
