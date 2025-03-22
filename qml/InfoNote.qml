import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: root
    anchors.fill: parent  // Fill all available space in the parent

    // Properties to access TextField values
    property alias title: titleField.text
    property alias url: urlField.text
    property alias username: usernameField.text
    property alias email: emailField.text
    property alias password: passwordField.text

    // Signals for button clicks
    signal createClicked()
    signal generatePasswordClicked()

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
            Layout.fillHeight: true  // Ensure the form takes remaining vertical space

            // Title field
            TextField {
                id: titleField
                placeholderText: "Title"
                Layout.fillWidth: true
            }

            // Url field
            TextField {
                id: urlField
                placeholderText: "Url"
                Layout.fillWidth: true
            }

            // Username field
            TextField {
                id: usernameField
                placeholderText: "Username"
                Layout.fillWidth: true
            }

            // Email field
            TextField {
                id: emailField
                placeholderText: "Email"
                Layout.fillWidth: true
            }

            // Password field
            TextField {
                id: passwordField
                placeholderText: "Password"
                Layout.fillWidth: true
                echoMode: TextInput.Password
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
                onClicked: root.generatePasswordClicked()
            }

            Button {
                text: "Create"
                Material.elevation: 2
                Material.background: "#403F3F"
                Material.foreground: "white"
                onClicked: root.createClicked()
            }
        }
    }
}
