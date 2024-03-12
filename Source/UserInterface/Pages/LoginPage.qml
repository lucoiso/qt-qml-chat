// Copyright Notices: [...]

import QtQuick
import QtQuick.Controls

Item {
    anchors.fill: parent

    readonly property int field_width: 200

    signal loginSuccessful()

    Column {
        anchors.centerIn: parent
        spacing: 8

        Text {
            text: "Login"
            font.pixelSize: 20
        }

        TextField {
            id: usernameInput
            placeholderText: "Username"
            width: field_width
        }

        TextField {
            id: passwordInput
            placeholderText: "Password"
            width: field_width
            echoMode: TextInput.Password
        }

        Button {
            text: "Login"
            width: field_width

            onClicked: {
                loginSuccessful();
            }
        }
    }
}