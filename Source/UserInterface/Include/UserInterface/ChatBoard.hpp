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
        Q_PROPERTY(QString User READ GetUser WRITE SetUser NOTIFY OnUserChanged)
        Q_PROPERTY(QQmlListProperty<ChatMessage> History READ GetHistory NOTIFY OnHistoryChanged)
        QML_ELEMENT

        QString m_User {"User"};
        QList<ChatMessage *> m_History {};

        static void AppendMessage(QQmlListProperty<ChatMessage> *, ChatMessage *);
        static [[nodiscard]] qsizetype CountMessages(QQmlListProperty<ChatMessage> *);
        static [[nodiscard]] ChatMessage *GetMessageAt(QQmlListProperty<ChatMessage> *, qsizetype);
        static void ClearMessages(QQmlListProperty<ChatMessage> *);
        static void ReplaceMessage(QQmlListProperty<ChatMessage> *, qsizetype, ChatMessage *);
        static void PopMessage(QQmlListProperty<ChatMessage> *);

    public:
        explicit ChatBoard(QObject *Parent = nullptr);

        [[nodiscard]] QString GetUser() const;
        void SetUser(const QString &);

        [[nodiscard]] QQmlListProperty<ChatMessage> GetHistory();

        void AppendMessage(ChatMessage *);
        [[nodiscard]] qsizetype CountMessages() const;
        [[nodiscard]] ChatMessage *GetMessageAt(qsizetype) const;
        void ClearMessages();
        void ReplaceMessage(qsizetype, ChatMessage *);
        void PopMessage();

        Q_INVOKABLE void PostMessage(const QString &);

    signals:
        void OnUserChanged();
        void OnHistoryChanged();
    };
} // namespace UserInterface
