import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material
import QtCore
import QtQuick.Dialogs

Dialog {
    id: root
    modal: true
    Layout.maximumWidth: 900  // Set maximum width to 600
    Layout.maximumHeight: 600  // Set maximum width to 600
    Layout.fillWidth: true    // Make the dialog take all available width
    Material.theme: Material.Dark
    Material.accent: Material.Purple
    Material.primary: Material.Grey

    Settings {
        id: guiSettings
        category: "gui"
        property string language: "English"
        property string type: "Widgets"
        property string theme: "Dark"
    }
    Settings {
        id: accountSettings
        category: "account"
        property string jwtToken
        property string refreshToken
    }

    Component.onCompleted: {
        guiTypeBox.currentIndex = guiTypeBox.indexOfValue(guiSettings.type)
        languageBox.currentIndex = languageBox.indexOfValue(guiSettings.language)
        themeBox.currentIndex = themeBox.indexOfValue(guiSettings.theme)

        if (accountSettings.jwtToken === null && accountSettings.refreshToken === null) {
            disableAccount()
        }
    }

    ScrollView {
        anchors.fill: parent
        clip: true
        Layout.fillWidth: true

        ColumnLayout {
            spacing: 10
            anchors.margins: 10

            // General Section
            Label {
                text: qsTr("GENERAL")
                font.bold: true
            }

            // GUI Type Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Label {
                    text: qsTr("GUI type\nPM offers different types of rendering")
                    Layout.fillWidth: true
                }
                Item { Layout.fillWidth: true }  // Spacer
                ComboBox {
                    id: guiTypeBox
                    model: ["Widgets", "Quick"]

                    onActivated: (index) => {
                        let val = guiTypeBox.textAt(index)
                        guiSettings.type = val
                        changePopup.open()
                    }
                }
            }

            // Language Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Label {
                    text: qsTr("Language\nInterface is available in different languages")
                    Layout.fillWidth: true
                }

                ComboBox {
                    id: languageBox
                    model: ["English", "Russian"]

                    onActivated: (index) => {
                        let val = languageBox.textAt(index)
                        guiSettings.language = val
                        changePopup.open()
                    }
                }
            }

            // Theme Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Label {
                    text: qsTr("Theme\nThere are several themes. You can manually edit them")
                    Layout.fillWidth: true
                }

                ComboBox {
                    id: themeBox
                    model: ["Dark", "Light"]

                    onActivated: (index) => {
                        let val = themeBox.textAt(index)
                        guiSettings.theme = val
                        changePopup.open()
                    }
                }
            }

            // Reset Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Label {
                    text: qsTr("Reset\nReset all settings and storages")
                    Layout.fillWidth: true
                }

                Button {
                    text: qsTr("Reset")
                }
            }

            // Encryption Section
            Label {
                text: qsTr("ENCRYPTION")
                font.bold: true
            }

            // Load Database Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Label {
                    text: qsTr("Load database\nYou can load PM database from other device")
                    Layout.fillWidth: true
                }

                Button {
                    text: qsTr("Load")
                }
            }

            // Export Storage Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Label {
                    text: qsTr("Export storage\nExport storage so you can use it on another device (Don't forget the password)")
                    Layout.fillWidth: true
                }

                Button {
                    text: qsTr("Export")
                }
            }

            // Account Section
            Label {
                text: qsTr("ACCOUNT")
                font.bold: true
            }

            Button {
                id: authBtn
                text: qsTr("Authenticate")
                Layout.fillWidth: true
            }

            // Account Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                id: accountRow
                Label {
                    text: qsTr("Account\nDo stuff with your account")
                    Layout.fillWidth: true
                }

                Button {
                    text: qsTr("Log Out")
                }
            }

            // Upload Row
            RowLayout {
                id: uploadRow
                Layout.fillWidth: true
                spacing: 10
                Label {
                    text: qsTr("Upload\nUpload your storage to the cloud")
                    Layout.fillWidth: true
                }

                Button {
                    text: qsTr("Upload")
                }
            }

            // Load from Cloud Row
            RowLayout {
                id: loadRow
                Layout.fillWidth: true
                spacing: 10
                Label {
                    text: qsTr("Load\nLoad your storage from the cloud")
                    Layout.fillWidth: true
                }

                Button {
                    text: qsTr("Load")
                }
            }

            Button {
                id: closeBtn
                text: qsTr("Close")
                onClicked: {
                    root.close()
                }
            }
        }
    }

    function disableAccount() {
        accountRow.visible = false
        uploadRow.visible = false
        loadRow.visible = false
        authBtn.visible = true
    }
    function enableAccount() {
        accountRow.visible = true
        uploadRow.visible = true
        loadRow.visible = true
        authBtn.visible = false
    }

    // Popup that appears when language, GUI type, or theme changes
    MessageDialog {
        id: changePopup
        title: qsTr("Setting Changed")
        text: qsTr("Settings have been changed. Restart to apply them?")
        buttons: MessageDialog.Ok | MessageDialog.Close
        Material.theme: Material.Dark
        Material.accent: Material.Purple
        Material.primary: Material.Grey

        onButtonClicked: function (button, role) {
            switch (button) {
            case MessageDialog.Ok:
                Qt.quit()
                break;
            case MessageDialog.Close:
                changePopup.close()
            }

        }
    }
}
