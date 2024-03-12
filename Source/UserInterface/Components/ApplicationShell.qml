// Copyright Notices: [...]

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../Pages" as Pages

Item {
    anchors.fill: parent

    TabBar {
        id: shellBar
        width: parent.width

        TabButton {
            text: qsTr("Home")
        }

        TabButton {
            text: qsTr("Placeholder")
        }
    }

    StackLayout {
        anchors.fill: parent
        anchors.top: shellBar.bottom

        currentIndex: shellBar.currentIndex

        Pages.HomePage {
            id: homePage
        }

        Pages.PlaceholderPage {
            id: placeholderPage
        }
    }
}