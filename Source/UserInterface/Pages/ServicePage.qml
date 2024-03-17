// Copyright Notices: [...]

import QtQuick
import QtQuick.Controls

import UserInterface

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

            ServiceProvider {
                id: serviceProvider
            }

            Button {
                text: serviceProvider.IsConnected ? "Stop Server" : "Start Server"
                visible: serviceProvider.Type !== 1

                onClicked: {
                    serviceProvider.TryConnect(serviceProvider.IsConnected ? 0 : 2);
                }
            }

            Button {
                text: serviceProvider.IsConnected ? "Stop Client" : "Start Client"
                visible: serviceProvider.Type !== 2

                onClicked: {
                    serviceProvider.TryConnect(serviceProvider.IsConnected ? 0 : 1);
                }
            }
        }
    }
}