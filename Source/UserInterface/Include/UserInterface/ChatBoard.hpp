// Copyright Notices: [...]

#pragma once

#include "ChatMessage.hpp"

#include <QObject>
#include <QQmlListProperty>
#include <QtQml/qqmlregistration.h>

namespace UserInterface
{
    class ChatBoard : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QQmlListProperty<ChatMessage> History READ GetHistory NOTIFY OnHistoryChanged)
        QML_ELEMENT

        QList<ChatMessage *> m_History {};
        std::uint8_t m_MessageReceivedHandler {0U};

        static void AppendMessage(QQmlListProperty<ChatMessage> *, ChatMessage *);
        static [[nodiscard]] qsizetype CountMessages(QQmlListProperty<ChatMessage> *);
        static [[nodiscard]] ChatMessage *GetMessageAt(QQmlListProperty<ChatMessage> *, qsizetype);
        static void ClearMessages(QQmlListProperty<ChatMessage> *);
        static void ReplaceMessage(QQmlListProperty<ChatMessage> *, qsizetype, ChatMessage *);
        static void PopMessage(QQmlListProperty<ChatMessage> *);

        void OnMessageReceived(const QString &, const QString &);

    public:
        explicit ChatBoard(QObject *Parent = nullptr);

        [[nodiscard]] QQmlListProperty<ChatMessage> GetHistory();

        void AppendMessage(ChatMessage *);
        [[nodiscard]] qsizetype CountMessages() const;
        [[nodiscard]] ChatMessage *GetMessageAt(qsizetype) const;
        void ClearMessages();
        void ReplaceMessage(qsizetype, ChatMessage *);
        void PopMessage();

        Q_INVOKABLE void PostChatMessage(const QString &Message);

    signals:
        void OnHistoryChanged();
    };
} // namespace UserInterface
