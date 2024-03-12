# Copyright Notices: [...]

# ----------- Global Definitions -----------
SET(LIBRARY_NAME UserInterface)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

# ---------------- Qt Setup ----------------
FIND_PACKAGE(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Qml Quick)

SET(QML_FILES
        MainWindow.qml
        Components/ApplicationShell.qml
        Pages/HomePage.qml
        Pages/LoginPage.qml
        Pages/PlaceholderPage.qml
)

QT_ADD_LIBRARY(${LIBRARY_NAME} STATIC)

QT_ADD_QML_MODULE(${LIBRARY_NAME}
        URI ${LIBRARY_NAME}
        VERSION 1.0
        QML_FILES ${QML_FILES}
        RESOURCE_PREFIX /
)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} PRIVATE
        Qt6::Qml
)
