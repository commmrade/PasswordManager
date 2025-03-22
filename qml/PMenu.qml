import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic


Menu {
    id: root

    delegate: MenuItem {
        id: menuItem
        contentItem: Item {
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 5

                text: menuItem.text
                color: "white"
            }
            Rectangle {
                id: indicator

                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                width: 6
                height: parent.height

                visible: menuItem.highlighted
                color: "darkgray"
            }
        }
        background: Rectangle {
            implicitWidth: 210
            implicitHeight: 35
            color: "black"
        }
    }
    background: Rectangle {
        implicitWidth: 210
        implicitHeight: 35
        color: "red"
    }
}
