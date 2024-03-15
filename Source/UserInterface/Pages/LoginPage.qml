// Copyright Notices: [...]

import QtQuick
import QtQuick.Controls

Item {
    readonly property int field_width: 200

    signal loginSuccessful(username: string)

    Column {
        anchors.centerIn: parent
        spacing: 8

        TextField {
            id: usernameInput
            placeholderText: "Username"
            width: field_width
        }

        Button {
            text: "Login"
            width: field_width

            onClicked: {
                loginSuccessful(usernameInput.text);
            }
        }
    }
}