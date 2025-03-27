pragma Singleton
import QtCore
import QtQml

QtObject {
    id: root

    // Security category

    property Settings general: Settings {
        property bool firstTime: true
    }

    property Settings security: Settings {
        category: "security"
        property string password: "none"
    }

    // Appearance category
    property Settings gui: Settings {
        category: "gui"
        property string language: "English"
        property string type: "Widgets"
        property string theme: "Dark"
    }

}
