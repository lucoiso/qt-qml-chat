// Copyright Notices: [...]

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../Pages" as Pages

Item {
    TabBar {
        id: shellBar

        z: 999
        width: parent.width

        TabButton {
            text: qsTr("Service")
        }

        TabButton {
            text: qsTr("Chat Board")
        }
    }

    StackLayout {
        anchors.left: parent.left
        anchors.top: shellBar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        currentIndex: shellBar.currentIndex

        Pages.ServicePage {
            id: servicePage
        }

        Pages.ChatPage {
            id: chatPage
        }
    }
}