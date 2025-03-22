import QtQuick 2.15
import QtQuick.Controls


Window {
    id: root
    width: 1280
    height: 720
    visible: true
    color: "#292828"
    title: qsTr("Hello World")


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


    Notes {
        id: notesView
        anchors.topMargin: menuBar.height + 10
        anchors.bottomMargin: menuBar.height
    }
}
