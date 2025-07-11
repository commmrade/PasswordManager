import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
// import PasswordManager

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
            color: AppSettings.gui.theme === "Dark" ? "white" : "black"
            opacity: enabled ? 1.0 : 0.3
        }

        background: Rectangle {
            id: background

            color: AppSettings.gui.theme === "Dark" ? "#312F2F" : "#FFFFFF"
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
