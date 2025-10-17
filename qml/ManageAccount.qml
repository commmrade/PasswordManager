import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material
import QtCore
import QtQuick.Dialogs
import SettingsController
import AuthManager
import StorageManager

Dialog {
    id: root
    modal: true
    // Layout.maximumWidth: 900  // Set maximum width to 600
    // Layout.maximumHeight: 600  // Set maximum width to 600
    // Layout.fillWidth: true    // Make the dialog take all available width
    Material.theme: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Dark : Material.Light
    Material.accent: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Purple : Material.LightBlue
    Material.primary: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Grey : Material.BlueGrey
    signal loggedOut()
    property AuthManager authManager: AuthManager{}
    onClosed: resetFields()

    Connections {
        target: authManager
        function onSuccessAuth() {
            infoDialog.infoTitle = qsTr("Success")
            infoDialog.infoMessage = qsTr("Successfully changed user password")
            infoDialog.open()
        }

        function onErrorAuth(statusCode, errorMessage) {
            infoDialog.infoTitle = qsTr("Error")
            infoDialog.infoMessage = qsTr("Could not change password: " + errorMessage)
            infoDialog.open()
        }
    }

    Settings {
        id: accountSettings
        category: "account"
        property string jwtToken
        property string refreshToken
    }

    ScrollView {
        anchors.fill: parent
        clip: true
        Layout.fillWidth: true

        ColumnLayout {
            spacing: 10
            anchors.margins: 10

            Label {
                text: qsTr("Account")
                font.bold: true
                font.pointSize: 16
            }

            Rectangle {
                Layout.fillWidth: true
                height: 2
                color: "darkgray"
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5

                    Label {
                        text: qsTr("You can change your password here")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10


                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("Current password")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                }

                TextField {
                    id: curPassEdit
                    placeholderText: qsTr("password...")
                    echoMode: TextInput.Password
                }
            }

            // Theme Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10


                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("New Password")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("You should remember this one, i think")
                        Layout.fillWidth: true
                    }
                }

                TextField {
                    id: newPassEdit1
                    placeholderText: qsTr("password...")
                    echoMode: TextInput.Password
                }
            }

            // Reset Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("New Password v2")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("Can never be too careful, right")
                        Layout.fillWidth: true
                    }
                }

                TextField {
                    id: newPassEdit2
                    placeholderText: qsTr("password...")
                    echoMode: TextInput.Password
                }
            }

            Button {
                text: qsTr("Change password")
                onClicked: {
                    let curPass = curPassEdit.text
                    let newPass1 = newPassEdit1.text
                    let newPass2 = newPassEdit2.text
                    tryChangePassword(curPass, newPass1, newPass2)
                }
            }

            Label {
                text: qsTr("Scary section")
                font.bold: true
                font.pointSize: 16
            }

            Rectangle {
                Layout.fillWidth: true
                height: 2
                color: "darkgray"
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("Log Out")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("Bye bye, see you")
                        Layout.fillWidth: true
                    }
                }

                Button {
                    text: qsTr("Log out")
                    onClicked: {
                        authManager.logOut()

                        loggedOut()
                        close()
                    }
                }
            }
        }
    }

    Dialog {
        id: infoDialog
        Material.theme: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Dark : Material.Light
        Material.accent: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Purple : Material.LightBlue
        Material.primary: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Grey : Material.BlueGrey
        title: infoTitle
        standardButtons: Dialog.Ok

        property string infoMessage: ""
        property string infoTitle: qsTr("Error")
        Text {
            text: infoDialog.infoMessage
            color: "white"
        }
    }

    function resetFields() {
        curPassEdit.text = ""
        newPassEdit1.text = ""
        newPassEdit2.text = ""
    }

    function tryChangePassword(curPass, newPass1, newPass2) {
        if (!curPass) {
            return;
        }
        if (newPass1 !== newPass2) {
            infoDialog.infoTitle = qsTr("Success")
            infoDialog.infoMessage = qsTr("New password fields do not match")
            infoDialog.open()
            return;
        }

        if (!newPass1 || newPass1.length < 6) {
            infoDialog.infoTitle = qsTr("Success")
            infoDialog.infoMessage = qsTr("New password is too short, must be 6+ characters long")
            infoDialog.open()
            return;
        }

        authManager.changePassword(curPass, newPass1)
    }
}
