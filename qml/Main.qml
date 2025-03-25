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
    title: qsTr("Hello World")

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


    PMenubar {
        id: menuBar
        height: 20
        PMenu {
            id: managerMenu
            title: qsTr("Manager")
            Action {
                text: qsTr("Settings");
                onTriggered: {
                    console.log("open settings")
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
