// Copyright Notices: [...]

import QtQuick
import QtQuick.Controls

Item {
    Rectangle {
        anchors.centerIn: parent
        width: 200
        height: 200
        color: "lightsteelblue"
        radius: 4

        Column {
            spacing: 10
            anchors.centerIn: parent

            Button {
                text: "Start Server"
                onClicked: {
                    console.log("ss wip")
                }
            }

            Button {
                text: "Start Client"
                onClicked: {
                    console.log("sc wip")
                }
            }
        }
    }
}