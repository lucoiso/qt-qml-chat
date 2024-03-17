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
            Layout.margins: 8
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop

            ScrollBar.vertical: ScrollBar {
            }

            verticalLayoutDirection: ListView.BottomToTop
            displayMarginBeginning: 40
            displayMarginEnd: 40
            spacing: 12

            orientation: ListView.Vertical
            model: chatBoard.History

            delegate: ColumnLayout {
                Rectangle {
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

        Pane {
            Layout.fillWidth: true

            TextField {
                id: messageInput
                anchors.fill: parent

                placeholderText: "Message"

                onAccepted: {
                    if (text.trim() !== "") {
                        chatBoard.PostChatMessage(text)
                        text = "";
                    }
                }
            }
        }
    }
}
