// Copyright Notices: [...]

import QtQuick
import QtQuick.Controls

import "Components" as Components
import "Pages" as Pages

Window {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Main Window")

    StackView {
        id: mainStack
        anchors.fill: parent

        initialItem: Pages.LoginPage
        {
            onLoginSuccessful: mainStack.push(compShell)
        }

        Component {
            id: compShell
            Components.ApplicationShell {
                id: appShell
            }
        }
    }
}