pragma Singleton
import QtCore
Settings {
    category: "security"
    property string password
    property bool firstTime: true

    // Component.onCompleted: {
    //     password = settings.value("password", "none")
    //     firstTime = settings.value("firstTime", true)
    // }
}
