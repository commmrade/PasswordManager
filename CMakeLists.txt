cmake_minimum_required(VERSION 3.16)

project(PasswordManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Manually specify the path to Crypto++ headers and libraries
# Adjust these paths to where Crypto++ is installed on your system
set(CRYPTOPP_INCLUDE_DIRS "/usr/include/cryptopp")  # Adjust to the correct include path
set(CRYPTOPP_LIBRARIES "/usr/lib/libcryptopp.so")   # Adjust to the correct library path

# Add Crypto++ headers to the include directories
include_directories(${CRYPTOPP_INCLUDE_DIRS})

set(PROJECT_SOURCES
    main.cpp
    common/mainwindow.cpp
    common/mainwindow.h
    common/mainwindow.ui
    crypt/cipher.h crypt/cipher.cpp
    models/notemodel.h models/notemodel.cpp
    dialogs/notecreatedialog.h dialogs/notecreatedialog.cpp dialogs/notecreatedialog.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PasswordManager
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        models/notecontroller.h models/notecontroller.cpp
        widgets/noteswidget.h widgets/noteswidget.cpp widgets/noteswidget.ui
        crypt/passwordgenerator.h crypt/passwordgenerator.cpp
        widgets/infowidget.h widgets/infowidget.cpp widgets/infowidget.ui
        common/Note.h
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
    endif()
endif()

# Link Qt libraries
target_link_libraries(PasswordManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Link Crypto++ library (shared .so)
target_link_libraries(PasswordManager PRIVATE ${CRYPTOPP_LIBRARIES})

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
