import QtQuick
import QtQuick.Controls
import QtCore
import QtQuick.LocalStorage
import PasswordManager 1.0  //


Window {
    id: root
    width: 1280
    height: 720
    visible: true
    color: "#292828"
    title: qsTr("Password Manager")

    Component.onCompleted: {
        console.log("Initial settings - firstTime: " + AppSettings.firstTime + ", password: " + AppSettings.password)
        if (AppSettings.firstTime === true) {
            welcomeWidget.visible = true
        } else {
            welcomeWidget.destroy()
            notesView.visible = true
        }
        console.log(AppSettings.password)
    }


    SettingsDialog {
        id: settingsDialog
        anchors.centerIn: parent  // Center in the parent (root)

        // width: parent.width - parent.width / 4
        height: parent.height - parent.height / 4
    }


    PMenubar {
        id: menuBar
        height: 20
        PMenu {
            id: managerMenu
            title: qsTr("Manager")
            Action {
                text: qsTr("Settings");
                onTriggered: {
                    settingsDialog.open()
                }
            }
        }
    }

    Welcome {
        id: welcomeWidget
        visible: false
        anchors.centerIn: parent  // Center in the parent (root)
        anchors.topMargin: menuBar.height + 10  // Offset from the menu bar
        width: parent.width
        height: parent.height

        Component.onDestruction: {
            notesView.visible = true
        }
    }

    Notes {
        id: notesView

        visible: false
        anchors.topMargin: menuBar.height + 10
        anchors.bottomMargin: menuBar.height
    }
}
