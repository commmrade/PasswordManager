pragma Singleton
import QtQml
import QtCore

QtObject {
    id: root

    // Enums for GUI settings
    readonly property int gui_language_english: 0
    readonly property int gui_language_russian: 1

    readonly property int gui_type_widgets: 0
    readonly property int gui_type_quick: 1

    readonly property int gui_theme_dark: 0
    readonly property int gui_theme_light: 1

    // General settings
    property Settings general: Settings {
        property bool firstTime: true
    }

    // Security category
    property Settings security: Settings {
        category: "security"
        property string password: "none"
    }

    // Account category
    property Settings account: Settings {
        category: "account"
        property string jwtToken: ""
        property string refreshToken: ""
        property string email: ""
    }

    // Appearance category
    property Settings gui: Settings {
        category: "gui"
        property int language: gui_language_english
        property int type: gui_type_widgets
        property int theme: gui_theme_dark
    }
}
