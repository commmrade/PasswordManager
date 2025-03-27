import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material


Dialog {
    id: dialog
    title: qsTr("Sign Up / Log In")
    modal: true

    Material.theme: Material.Dark
    Material.accent: Material.Purple
    Material.primary: Material.Grey

    contentItem: RowLayout {
        anchors.fill: parent
        spacing: 0


        // Sign Up section (left half)
        ColumnLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 100

            Text {
                text: qsTr("Sign Up")
                font.pointSize: 16
            }

            TextField {
                placeholderText: qsTr("Username...")
            }

            TextField {
                placeholderText: qsTr("Email...")
            }

            TextField {
                placeholderText: qsTr("Password...")
                echoMode: TextInput.Password
            }

            Button {
                text: qsTr("Continue")
            }
        }

        // Log In section (right half)
        ColumnLayout {
            Layout.fillWidth: true
            // Layout.stretch: 1  // Ensures this column takes the other half

            Text {
                text: qsTr("Log In")
                font.pointSize: 16
            }

            TextField {
                placeholderText: qsTr("Email...")
            }

            TextField {
                placeholderText: qsTr("Password...")
                echoMode: TextInput.Password
            }

            Button {
                text: qsTr("Continue")
            }
        }
    }
}
