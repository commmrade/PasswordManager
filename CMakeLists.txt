cmake_minimum_required(VERSION 3.29)

project(PasswordManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_VERSION_MAJOR 6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Network LinguistTools)
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} translations/translation.ts translations/translation_ru.ts)
# configure_file(${QM_FILES} ${CMAKE_SOURCE_DIR} COPYONLY)

find_package(CryptoPP QUIET)

if(CryptoPP_FOUND)
    message(STATUS "Found Crypto++: ${CryptoPP_INCLUDE_DIRS}, ${CryptoPP_LIBRARIES}")
else()
    # fallback: найти вручную
    find_path(CRYPTOPP_INCLUDE_DIR cryptopp/cryptlib.h)
    find_library(CRYPTOPP_LIBRARY cryptopp)
    if(NOT CRYPTOPP_INCLUDE_DIR OR NOT CRYPTOPP_LIBRARY)
        message(FATAL_ERROR "Could not find Crypto++ library")
    endif()
    set(CryptoPP_INCLUDE_DIRS ${CRYPTOPP_INCLUDE_DIR})
    set(CryptoPP_LIBRARIES ${CRYPTOPP_LIBRARY})
endif()

set(PROJECT_SOURCES
    main.cpp
    common/mainwindow.cpp
    common/mainwindow.h
    common/mainwindow.ui
    crypt/cipher.h crypt/cipher.cpp
    models/notemodel.h models/notemodel.cpp
    dialogs/notecreatedialog.h dialogs/notecreatedialog.cpp dialogs/notecreatedialog.ui
)

# set(PROJ_QML_FILES
#     ${PROJ_QML_SINGLETON_FILES}
#     qml/Main.qml
# )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PasswordManager
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${QM_FILES}
        models/notecontroller.h models/notecontroller.cpp
        widgets/noteswidget.h widgets/noteswidget.cpp widgets/noteswidget.ui
        crypt/passwordgenerator.h crypt/passwordgenerator.cpp
        widgets/infowidget.h widgets/infowidget.cpp widgets/infowidget.ui
        common/Note.h
        widgets/secretpasswidget.h widgets/secretpasswidget.cpp widgets/secretpasswidget.ui
        resources.qrc
        dialogs/loaderpasswords.h dialogs/loaderpasswords.cpp dialogs/loaderpasswords.ui
        common/consts.h
        dialogs/settingsdialog.h dialogs/settingsdialog.cpp dialogs/settingsdialog.ui
        net/iconmanager.h net/iconmanager.cpp
        translations.qrc
        dialogs/authdialog.h dialogs/authdialog.cpp dialogs/authdialog.ui
        net/authmanager.h net/authmanager.cpp
        net/storagemanager.h net/storagemanager.cpp
        common/clipboard.h common/clipboard.cpp
        common/loadercontroller.h common/loadercontroller.cpp
        common/settingscontroller.h common/settingscontroller.cpp
        common/dotenv.h common/dotenv.cpp
        dialogs/manageaccountdialog.h dialogs/manageaccountdialog.cpp dialogs/manageaccountdialog.ui
        common/appcontrol.h common/appcontrol.cpp
        common/settingsvalues.h
    )

else()
    if(ANDROID)
        add_library(PasswordManager SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(PasswordManager
            ${PROJECT_SOURCES}
        )
        set(QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/qml)
        set(QML2_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/qml)
       install(DIRECTORY qml DESTINATION ${CMAKE_INSTALL_PREFIX})
    endif()
endif()



find_package(Qt6 REQUIRED COMPONENTS Qml)
# Link Qt libraries
target_link_libraries(PasswordManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Qml)

set_source_files_properties(qml/AppSettings.qml PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(PasswordManagerQML
        URI PasswordManager
        VERSION 1.0
        QML_FILES qml/Main.qml qml/PMenubar.qml qml/PMenu.qml qml/Notes.qml qml/InfoNote.qml qml/NoteCreator.qml qml/Welcome.qml qml/AppSettings.qml qml/PLoader.qml qml/SettingsDialog.qml qml/AuthDialog.qml
        qml/PLoaderStorage.qml
        qml/ManageAccount.qml
        STATIC
)

target_include_directories(PasswordManager PRIVATE ${CryptoPP_INCLUDE_DIRS})


# Link Crypto++ library (shared .so)
# target_link_libraries(PasswordManager PRIVATE ${CRYPTOPP_LIBRARIES} PasswordManagerQML ${CryptoPP_LIBRARIES})
target_link_libraries(PasswordManager PRIVATE ${CRYPTOPP_LIBRARIES} PasswordManagerQML PasswordManagerQMLplugin ${CryptoPP_LIBRARIES})


include_directories(common)
include_directories(dialogs)
include_directories(widgets)
include_directories(crypt)
include_directories(models)
include_directories(net)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PasswordManager)
endif()
set_target_properties(PasswordManager PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
include(GNUInstallDirs)
install(TARGETS PasswordManager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PasswordManager)
endif()
