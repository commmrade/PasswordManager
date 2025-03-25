import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 2.15
import PasswordGenerator
import QtCore
import PasswordManager 1.0


Item {
    id: root
    visible: true
    width: 600
    height: 400

    Material.theme: Material.Dark
    Material.accent: Material.Purple
    Material.primary: Material.Grey

    PasswordGenerator {
        id: passwordGenerator
    }
    Component.onCompleted: {
        passwordText.text = passwordGenerator.generatePassword(32)
    }

    PLoader {
        id: pLoader;
        x: (root.width - width) / 2  // Center horizontally
        y: -(root.height - height) * 8   // Center vertically
        height: root.height - 100
        width: root.width - 100
    }

    Dialog {
        id: confirmationDialog
        x: (root.width - width) / 2  // Center horizontally
        y: root.height / 2  // Center vertically
        title: "Confirmation"
        modal: true
        Material.theme: Material.Dark
        Material.accent: Material.Purple
        Material.primary: Material.Grey
        contentItem: Column {
            spacing: 10

            Text {
                text: "Do you really want to close this? Save the password!"
                wrapMode: Text.Wrap
                color: "white"
                font.pixelSize: 16
            }

            Row {
                spacing: 10

                Button {
                    text: "Yes"
                    onClicked: {
                        confirmationDialog.accept() // Use accept() instead of close()
                    }
                }

                Button {
                    text: "No"
                    onClicked: {
                        confirmationDialog.reject()
                    }
                }
            }
        }

        onAccepted: {
            AppSettings.firstTime = false
            AppSettings.password = passwordText.text
            console.log(AppSettings.password)
            AppSettings.sync()
            Qt.callLater(function() { root.destroy() })
        }
    }




    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 100
        spacing: 10

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: "#393939"
            radius: 10

            Label {
                anchors.centerIn: parent
                text: "Password Manager"
                font.pixelSize: 20
                color: "white"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#393939"
            radius: 10

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10

                Label {
                    text: "Your password:"
                    font.pixelSize: 16
                    color: "white"
                    Layout.alignment: Qt.AlignHCenter
                }
                Label {
                    text: "(Password is needed to be able to load storage on another device)"
                    font.pixelSize: 16
                    color: "gray"
                    Layout.alignment: Qt.AlignHCenter
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    color: "#292828"
                    radius: 5

                    TextInput {
                        id: passwordText
                        text: "Kqs9AeMuP23FkVdgTzlwE"
                        font.pixelSize: 14
                        color: "white"
                        anchors.centerIn: parent
                        readOnly: true
                        selectByMouse: true
                    }
                }

                Button {
                    text: "📋"
                    Material.elevation: 2
                    onClicked: {
                        // Логика копирования в буфер обмена
                        clipboard.copyText(passwordText.text)
                    }
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: "Copy. You can't recover the password"
                    font.pixelSize: 12
                    color: Material.hintTextColor
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            spacing: 10

            Button {
                text: "Continue"
                Material.background: "#403F3F"
                Material.foreground: "white"
                Material.elevation: 2
                Layout.fillWidth: true
                onClicked: {
                    confirmationDialog.open()
                }
            }

            Button {
                text: "Load existing passwords"
                Material.background: "#403F3F"
                Material.foreground: "white"
                Material.elevation: 2
                Layout.fillWidth: true
                onClicked: {
                    console.log("clicked")
                    pLoader.open()
                }
            }
        }
    }
}