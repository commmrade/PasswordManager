import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material
import QtCore
import QtQuick.Dialogs
import SettingsController

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

    SettingsController {
        id: settingsController
    }


    Component.onCompleted: {
        guiTypeBox.currentIndex = guiTypeBox.indexOfValue(guiSettings.type)
        languageBox.currentIndex = languageBox.indexOfValue(guiSettings.language)
        themeBox.currentIndex = themeBox.indexOfValue(guiSettings.theme)
        if (accountSettings.jwtToken === "" && accountSettings.refreshToken === "") {
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
                font.pointSize: 16
            }

            Rectangle {
                Layout.fillWidth: true
                height: 2  // Adjust thickness
                color: "darkgray"
            }

            // GUI Type Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5

                    Label {
                        text: qsTr("GUI type")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("PM offers different types of rendering")
                        Layout.fillWidth: true
                    }
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


                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("Language")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("Interface is available in different languages")
                        Layout.fillWidth: true
                    }
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


                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("Theme")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("There are several themes. You can manually edit them")
                        Layout.fillWidth: true
                    }
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

                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("Reset")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("Reset all settings and storages")
                        Layout.fillWidth: true
                    }
                }

                Button {
                    text: qsTr("Reset")

                    onClicked: {
                        resetPopup.open();
                    }
                }
            }

            // Encryption Section
            Label {
                text: qsTr("ENCRYPTION")
                font.bold: true
            }

            Rectangle {
                Layout.fillWidth: true
                height: 2  // Adjust thickness
                color: "darkgray"
            }

            // Load Database Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("Reset")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("Reset all settings and storages")
                        Layout.fillWidth: true
                    }
                }

                Button {
                    text: qsTr("Load")
                    onClicked: {
                        pLoader.open()
                    }
                }
            }

            // Export Storage Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("Export storage")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("Export storage so you can use it on another device (Don't forget the password)")
                        Layout.fillWidth: true
                    }
                }

                Button {
                    text: qsTr("Export")

                    onClicked: {
                        fileDialog.open()
                    }
                }
            }

            // Account Section
            Label {
                text: qsTr("ACCOUNT")
                font.bold: true
            }

            Rectangle {
                Layout.fillWidth: true
                height: 2  // Adjust thickness
                color: "darkgray"
            }

            Button {
                id: authBtn
                text: qsTr("Authenticate")
                Layout.fillWidth: true

                onClicked: {
                    authDialog.open()
                }
            }

            // Account Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                id: accountRow

                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("Account")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("Do stuff with your account")
                        Layout.fillWidth: true
                    }
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

                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("Upload")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("Upload your storage to the cloud")
                        Layout.fillWidth: true
                    }
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

                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("Load")
                        font.bold: true
                        font.weight: 200
                        font.pointSize: 11
                    }
                    Label {
                        text: qsTr("Load your storage from the cloud")
                        Layout.fillWidth: true
                    }
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

    MessageDialog {
        id: resetPopup
        title: qsTr("Resetting")
        text: qsTr("Are you sure, that you want to reset the app? It will delete settings, storage")
        buttons: MessageDialog.Ok | MessageDialog.Close
        Material.theme: Material.Dark
        Material.accent: Material.Purple
        Material.primary: Material.Grey

        onButtonClicked: function (button, role) {
            switch (button) {
                case MessageDialog.Ok:
                    settingsController.resetApp();
                    break;
                case MessageDialog.Close:

            }

        }
    }

    PLoader {
        id: pLoader;
        anchors.centerIn: root
        height: root.height - 100
        width: root.width - 100

        onAccepted: {
            root.close()
        }
    }

    AuthDialog {
        id: authDialog

        anchors.centerIn: root

        width: root.width
        height: root.height
    }

    Popup {
        id: errorDialog
        width: 300
        height: 150
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape

        Material.theme: Material.Dark
        Material.accent: Material.Purple
        Material.primary: Material.Grey

        Column {
            anchors.centerIn: parent
            spacing: 10

            Text {
                text: qsTr("Could not save the storage")
                color: "white"
                font.pixelSize: 16
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }

            Button {
                text: qsTr("OK")
                onClicked: errorDialog.close()
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Select a location")
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0] // Use first home location
        fileMode: FileDialog.SaveFile
        onAccepted: {
            if (!settingsController.exportStorage(fileDialog.selectedFile)) {
                errorDialog.open()
            }
        }
        onRejected: {
        }
    }
}
