# Copyright Notices: [...]

# ----------- Global Definitions -----------
SET(LIBRARY_NAME UserInterface)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

# ------------- Library Setup --------------
SET(PRIVATE_MODULES_BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Source)
SET(PUBLIC_MODULES_BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME})

SET(PRIVATE_MODULES
        ${PRIVATE_MODULES_BASE_DIRECTORY}/ChatBoard.cpp
        ${PRIVATE_MODULES_BASE_DIRECTORY}/ChatMessage.cpp
)

SET(PUBLIC_MODULES
        ${PUBLIC_MODULES_BASE_DIRECTORY}/ChatBoard.hpp
        ${PUBLIC_MODULES_BASE_DIRECTORY}/ChatMessage.hpp
)

# ---------------- Qt Setup ----------------
FIND_PACKAGE(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Qml Quick)

SET(QML_FILES
        MainWindow.qml
        Pages/ChatPage.qml
        Pages/LoginPage.qml
        Pages/HomePage.qml
        Components/ApplicationShell.qml
)

QT_ADD_LIBRARY(${LIBRARY_NAME} STATIC)

QT_ADD_QML_MODULE(${LIBRARY_NAME}
        URI ${LIBRARY_NAME}
        VERSION 1.0
        QML_FILES ${QML_FILES}
        SOURCES ${PRIVATE_MODULES} ${PUBLIC_MODULES}
        RESOURCE_PREFIX /
)

FIND_PACKAGE(Boost REQUIRED COMPONENTS json headers system log)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} PRIVATE
        Qt6::Core
        Qt6::Qml
        Boost::headers
        Boost::system
        Boost::log
        Boost::json
)

TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PRIVATE
        ${PRIVATE_MODULES_BASE_DIRECTORY}
        ${PUBLIC_MODULES_BASE_DIRECTORY}
)

TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PUBLIC ${PUBLIC_MODULES_BASE_DIRECTORY}/..)
