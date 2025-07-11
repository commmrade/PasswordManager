import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 2.15
import PasswordGenerator
import QtCore
// import PasswordManager 1.0


Item {
    id: root
    visible: true
    width: 600
    height: 400

    Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
    Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
    Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

    PasswordGenerator {
        id: passwordGenerator
    }
    Component.onCompleted: {
        passwordText.text = passwordGenerator.generatePassword(32)
    }

    PLoader {
        id: pLoader;
        anchors.centerIn: root
        height: root.height - 100
        width: root.width - 100


        onAccepted: {
            root.destroy()
        }
    }

    Dialog {
        id: confirmationDialog
        x: (root.width - width) / 2  // Center horizontally
        y: root.height / 2  // Center vertically
        title: qsTr("Confirmation")
        modal: true
        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
        Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey
        standardButtons: Dialog.Ok | Dialog.No

        Text {
            text: qsTr("Do you really want to close this? Save the password!")
            wrapMode: Text.Wrap
            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
            font.pixelSize: 16
        }

        onAccepted: {
            AppSettings.general.firstTime = false
            AppSettings.security.password = passwordText.text
            AppSettings.security.sync()
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
            color: AppSettings.gui.theme === "Dark" ? "#393939" : "#E2E2E2"
            radius: 10

            Label {
                anchors.centerIn: parent
                text: qsTr("Password Manager")
                font.pixelSize: 20
                color: "white"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: AppSettings.gui.theme === "Dark" ? "#393939" : "#E2E2E2"
            radius: 10

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10

                Label {
                    text: qsTr("Your password:")
                    font.pixelSize: 16
                    color: AppSettings.gui.theme === "Dark" ? "white" : "black"
                    Layout.alignment: Qt.AlignHCenter
                }
                Label {
                    text: qsTr("(Password is needed to be able to load storage on another device)")
                    font.pixelSize: 16
                    color: "gray"
                    Layout.alignment: Qt.AlignHCenter
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    color: AppSettings.gui.theme === "Dark" ? "#292828" : "#FFFFFF"
                    radius: 5

                    TextInput {
                        id: passwordText
                        text: ""
                        font.pixelSize: 14
                        color: AppSettings.gui.theme === "Dark" ? "white" : "black"
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
                    text: qsTr("Copy. You can't recover the password")
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
                text: qsTr("Continue")
                Material.elevation: 2
                Layout.fillWidth: true
                onClicked: {
                    confirmationDialog.open()
                }
            }

            Button {
                text: qsTr("Load existing passwords")

                Material.elevation: 2
                Layout.fillWidth: true
                onClicked: {
                    pLoader.open()
                }
            }
        }
    }
}
