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

        Component {
            id: compShell

            Components.ApplicationShell {
            }
        }

        initialItem: Pages.LoginPage
        {
            onLoginSuccessful: function (username) {
                mainStack.push(compShell)
            }
        }
    }
}