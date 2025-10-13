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
    Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
    Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
    Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

    property bool guiTypeChanged: false
    property bool languageChanged: false
    property bool themeChanged: false
    property bool restartRequired: false

    property AuthManager authManager: AuthManager{}
    property StorageManager storageManager: StorageManager {}

    PLoaderStorage {
        id: loaderStorage
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
    }

    Connections {
        target: storageManager

        function onSuccess() {
            noteController.resetStorage()
        }

        function onError(statusCode, errorMessage) {
            storageErrorDialog.text = errorMessage
            storageErrorDialog.open()

            if (statusCode === 401) {
                authManager.updateToken()
            }
        }
    }

    Connections {
        target: authManager

        function onErrorAuth(statusCode, errorMessage) {
            if (statusCode === 403) {
                disableAccount()
                storageErrorDialog.text = "Please, log in again"
                storageErrorDialog.open()
            }
        }
    }

    Connections {
        target: manageAccountDialog

        function onLoggedOut() {
            disableAccount()
        }
    }

    MessageDialog {
        id: storageErrorDialog
        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
        Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey
        title: qsTr("Warning")
    }

    Dialog {
        id: loadWarningDialog
        title: qsTr("Warning")
        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
        Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey
        Text {
            text: qsTr("Are you sure you want to load the storage backup? It will overwrite the current storage")
            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
        }

        standardButtons: MessageDialog.Ok | MessageDialog.Cancel
        onAccepted: {
            loaderStorage.open()
        }
    }

    Dialog {
        id: uploadWarningDialog
        title: qsTr("Warning")
        y: (parent.height - height) / 2
        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
        Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey
        Text {
            text: qsTr("Are you sure you want to upload the storage? It will overwrite the backup storage")
            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
            wrapMode: Text.WordWrap
        }

        standardButtons: MessageDialog.Ok | MessageDialog.Cancel
        onAccepted: {
            storageManager.saveStorage()
        }
    }
    // Settings {
    //     id: guiSettings
    //     category: "gui"
    //     property string language: "English"
    //     property string type: "Widgets"
    //     property string theme: "Dark"
    // }
    Settings {
        id: accountSettings
        category: "account"
        property string jwtToken
        property string refreshToken
    }

    SettingsController {
        id: settingsController
    }


    property int guiTypeBoxPrevIndex
    property int languageBoxPrevIndex
    property int themeBoxPrevIndex
    Component.onCompleted: {
        guiTypeBox.currentIndex = guiTypeBox.indexOfValue(AppSettings.gui.type)
        languageBox.currentIndex = languageBox.indexOfValue(AppSettings.gui.language)
        themeBox.currentIndex = themeBox.indexOfValue(AppSettings.gui.theme)
        guiTypeBoxPrevIndex = guiTypeBox.currentIndex
        languageBoxPrevIndex = languageBox.currentIndex
        themeBoxPrevIndex = themeBox.currentIndex

        if (accountSettings.jwtToken === "" && accountSettings.refreshToken === "") {
            disableAccount()
        } else {
            enableAccount()
        }

    }    

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        ScrollView {
            anchors.margins: 10
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

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
                        Layout.maximumHeight: 50
                        onActivated: (index) => {
                            if (index !== guiTypeBoxPrevIndex) {
                                root.guiTypeChanged = true
                                root.restartRequired = true
                            }
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
                        Layout.maximumHeight: 50
                        onActivated: (index) => {
                            if (index !== languageBoxPrevIndex) {
                                root.languageChanged = true
                                root.restartRequired = true
                            }
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
                        Layout.maximumHeight: 50
                        onActivated: (index) => {
                            themeChanged = true
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
                            text: qsTr("Load")
                            font.bold: true
                            font.weight: 200
                            font.pointSize: 11
                        }
                        Label {
                            text: qsTr("Load your storage from a file")
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
                        // text: qsTr("Log Out")
                        text: qsTr("Manage")

                        onClicked: {
                            // authManager.logOut()
                            // disableAccount()
                            manageAccountDialog.open()
                        }
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

                        onClicked: {
                            uploadWarningDialog.open()
                        }
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

                        onClicked: {
                            loadWarningDialog.open()
                        }
                    }
                }
            }
        }


        RowLayout {
            id: btnsRow
            Layout.fillWidth: true
            spacing: 10

            Item {
                Layout.fillWidth: true
            }

            Button {
                id: closeBtn
                text: qsTr("Close")
                onClicked: {
                    root.close()
                }
            }
            Button {
                id: applyBtn
                text: qsTr("Apply")
                onClicked: {
                    applyGeneralSettings()
                }
            }
        }
    }



    function applyGeneralSettings() {
        if (guiTypeChanged) {
            let val = guiTypeBox.currentText
            AppSettings.gui.type = val
        }
        if (themeChanged) {
            let val = themeBox.currentText
            AppSettings.gui.theme = val
        }
        if (languageChanged) {
            let val = languageBox.currentText
            AppSettings.gui.language = val
        }

        if (restartRequired) {
            changePopup.open()
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
    Dialog {
        id: changePopup
        title: qsTr("Setting Changed")
        standardButtons: MessageDialog.Ok | MessageDialog.Close
        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
        Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

        Text {
            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
            text: qsTr("Settings have been changed. Restart to apply them?")
        }

        onAccepted: {
            // Qt.quit()
            appControl.restartApp()
        }
    }

    Dialog {
        id: resetPopup
        title: qsTr("Resetting")

        Text {
            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
            text: qsTr("Are you sure, that you want to reset the app? It will delete settings, storage")
        }
        standardButtons: MessageDialog.Ok | MessageDialog.Close
        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
        Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey

        onAccepted: {
            settingsController.resetApp();
        }
    }

    PLoader {
        id: pLoader;
        height: root.height - 100
        width: root.width - 100
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        onAccepted: {
            root.close()
        }
    }

    AuthDialog {
        id: authDialog

        authManager: root.authManager

        width: root.width
        height: root.height

        onAccepted: {
            enableAccount()
        }
    }

    ManageAccount {
        id: manageAccountDialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
    }

    Dialog {
        id: errorDialog


        Material.theme: AppSettings.gui.theme === "Dark" ? Material.Dark : Material.Light
        Material.accent: AppSettings.gui.theme === "Dark" ? Material.Purple : Material.LightBlue
        Material.primary: AppSettings.gui.theme === "Dark" ? Material.Grey : Material.BlueGrey
        standardButtons: Dialog.Ok
        Text {
            text: qsTr("Could not save the storage")
            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
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
