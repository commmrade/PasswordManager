import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic


MenuBar {
    id: root

    delegate: MenuBarItem {
        id: menuBarItem

        contentItem: Text {
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            text: menuBarItem.text
            font: menuBarItem.font
            elide: Text.ElideRight
            color: "white"
            opacity: enabled ? 1.0 : 0.3
        }

        background: Rectangle {
            id: background

            color: "#312F2F"
            Rectangle {
                id: indicator

                width: 0; height: 3
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom

                color: "gray"
                states: State {
                    name: "active"
                    when: menuBarItem.highlighted
                    PropertyChanges {
                        indicator.width: background.width - 2
                    }
                }
                transitions: Transition {
                    NumberAnimation {
                        properties: "width"
                        duration: 175
                    }
                }
            }
        }
    }
}
