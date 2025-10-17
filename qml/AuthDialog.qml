import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import AuthManager
import QtQuick.Dialogs
import PasswordManager

Dialog {
    id: dialog
    title: qsTr("Sign Up / Log In")
    modal: true

    required property AuthManager authManager

    Connections {
        target: authManager

        function onSuccessAuth() {
            dialog.accept()
        }

        function onErrorAuth(statusCode, errorMessage) {
            console.log("Auth error: " + errorMessage)
            authErrorDialog.text = errorMessage
            authErrorDialog.open()
        }
    }

    onAccepted: {
        clearFields()
    }
    onRejected: {
        clearFields()
    }

    Material.theme: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Dark : Material.Light
    Material.accent: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Purple : Material.LightBlue
    Material.primary: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Grey : Material.BlueGrey

    contentItem: RowLayout {
        anchors.fill: parent
        spacing: 20

        // Sign Up section (left half)
        Rectangle {
            id: recReg
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: AppSettings.gui.theme === AppSettings.gui_theme_dark ? "#333232" : "#F1F1F1"
            Layout.topMargin: 75
            Layout.leftMargin: 50
            Layout.bottomMargin: 20

            radius: 20
            border.color: "#555"
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                Text {
                    text: qsTr("Sign Up")
                    font.pointSize: 16
                    color: "white"
                }


                ScrollView {
                    id: regScrollView
                    // anchors.fill: recReg
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    ScrollBar.vertical.policy: ScrollBar.AsNeeded


                    ColumnLayout {
                        width: regScrollView.width
                        spacing: 10

                        TextField {
                            id: usernameReg
                            placeholderText: qsTr("Username...")
                            Layout.fillWidth: true
                        }

                        TextField {
                            id: emailReg
                            placeholderText: qsTr("Email...")
                            Layout.fillWidth: true
                        }

                        TextField {
                            id: passwordReg
                            placeholderText: qsTr("Password...")
                            echoMode: TextInput.Password
                            Layout.fillWidth: true
                        }
                        Item { Layout.fillHeight: true }  // Заполнитель, чтобы кнопка ушла вниз
                    }
                }

                Button {
                    id: continueBtnReg
                    text: qsTr("Continue")
                    Layout.fillWidth: true

                    onClicked: {
                        if (usernameReg.text.length < 4 || emailReg.text === "" || passwordReg.text.length < 6) {
                            errorDialog.open()
                            return
                        }
                        authManager.registerUser(usernameReg.text, emailReg.text, passwordReg.text)
                    }
                }
            }


        }

        Dialog {
            id: errorDialog
            Material.theme: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Dark : Material.Light
            Material.accent: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Purple : Material.LightBlue
            Material.primary: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Grey : Material.BlueGrey
            title: qsTr("Warning")
            standardButtons: Dialog.Ok
            Text {
                text: qsTr("Please, fill out all fields.")
                color: "white"
            }
        }

        MessageDialog {
            id: authErrorDialog
            Material.theme: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Dark : Material.Light
            Material.accent: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Purple : Material.LightBlue
            Material.primary: AppSettings.gui.theme === AppSettings.gui_theme_dark ? Material.Grey : Material.BlueGrey
            title: qsTr("Warning")
        }

        // Log In section (right half)
        Rectangle {
            id: recLogin
            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.topMargin: 75
            Layout.rightMargin: 50
            Layout.bottomMargin: 20

            color: AppSettings.gui.theme === AppSettings.gui_theme_dark ? "#333232" : "#F1F1F1"

            radius: 20
            border.color: "#555"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                Text {
                    text: qsTr("Log In")
                    font.pointSize: 16
                    color: "white"
                }

                ScrollView {
                    id: loginScrollView
                    // anchors.fill: recLogin
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    ScrollBar.vertical.policy: ScrollBar.AsNeeded

                    ColumnLayout {
                        width: loginScrollView.width
                        TextField {
                            id: emailLogin
                            placeholderText: qsTr("Email...")
                            Layout.fillWidth: true
                        }

                        TextField {
                            id: passwordLogin
                            placeholderText: qsTr("Password...")
                            echoMode: TextInput.Password
                            Layout.fillWidth: true
                        }
                    }

                }

                Button {
                    id: continueBtnLogin
                    text: qsTr("Continue")
                    Layout.fillWidth: true

                    onClicked: {
                        if (emailLogin.text === "" || passwordLogin.text.length < 6) {
                            errorDialog.open()
                            return
                        }

                        authManager.loginUser(emailLogin.text, passwordLogin.text)
                    }
                }
            }



        }
    }
    function clearFields() {
        usernameReg.text = ""
        emailReg.text = ""
        passwordReg.text = ""
        emailLogin.text = ""
        passwordLogin.text = ""
    }
}
