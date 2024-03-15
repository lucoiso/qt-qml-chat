// Copyright Notices: [...]

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import UserInterface

Item {
    ChatBoard {
        id: chatBoard
    }

    ColumnLayout {
        id: chatLayout
        anchors.fill: parent

        ListView {
            id: messageListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            orientation: ListView.Vertical
            model: chatBoard.History

            delegate: ColumnLayout {
                Rectangle {
                    Layout.leftMargin: 8
                    Layout.topMargin: 4
                    Layout.rightMargin: 8
                    Layout.bottomMargin: 4

                    width: chatLayout.width - 16
                    height: textLayout.height
                    color: "lightsteelblue"
                    radius: 4

                    ColumnLayout {
                        id: textLayout

                        Text {
                            Layout.margins: 4
                            text: modelData.User
                            font.bold: true
                        }

                        Text {
                            Layout.margins: 8
                            text: modelData.Message
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            }
        }

        TextField {
            id: messageInput
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft | Qt.AlignBottom

            Layout.margins: 2

            placeholderText: "Message"

            onAccepted: {
                if (text.trim() !== "") {
                    chatBoard.PostMessage(text)
                    text = "";
                }
            }
        }
    }
}
