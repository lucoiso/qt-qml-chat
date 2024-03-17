// Copyright Notices: [...]

import QtQuick
import QtQuick.Controls

import UserInterface

Item {
    readonly property int field_width: 200

    signal loginSuccessful()

    Column {
        anchors.centerIn: parent
        spacing: 8

        LoginSetup {
            id: loginSetup
        }

        TextField {
            id: usernameInput
            placeholderText: "Username"
            width: field_width
        }

        Button {
            text: "Login"
            width: field_width
            enabled: usernameInput.text.trim() !== ""

            onClicked: {
                loginSetup.User = usernameInput.text;
                loginSuccessful();
            }
        }
    }
}