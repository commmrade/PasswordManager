import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import AuthManager
import QtQuick.Dialogs

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
            console.log("failed auth")
            console.log(errorMessage)
            console.log(statusCode)

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


    Material.theme: Material.Dark
    Material.accent: Material.Purple
    Material.primary: Material.Grey

    contentItem: RowLayout {
        anchors.fill: parent
        spacing: 20

        // Sign Up section (left half)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.topMargin: 75
            Layout.leftMargin: 50
            color: "#403F3F"
            radius: 20
            border.color: "#555"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10

                Text {
                    text: qsTr("Sign Up")
                    font.pointSize: 16
                    color: "white"
                }

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

        MessageDialog {
            id: errorDialog
            Material.theme: Material.Dark
            Material.accent: Material.Purple
            Material.primary: Material.Grey
            title: qsTr("Warning")
            text: qsTr("Please, fill out all fields.")
        }

        MessageDialog {
            id: authErrorDialog
            Material.theme: Material.Dark
            Material.accent: Material.Purple
            Material.primary: Material.Grey
            title: qsTr("Warning")
        }

        // Log In section (right half)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: "#403F3F"
            Layout.topMargin: 75
            Layout.rightMargin: 50

            radius: 20
            border.color: "#555"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10

                Text {
                    text: qsTr("Log In")
                    font.pointSize: 16
                    color: "white"
                }

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

                Item { Layout.fillHeight: true }  // Заполнитель

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
