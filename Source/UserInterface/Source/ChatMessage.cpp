// Copyright Notices: [...]

#include "ChatMessage.hpp"

using namespace UserInterface;

ChatMessage::ChatMessage(QObject *Parent) : QObject(Parent)
{
}

QString ChatMessage::GetUser() const
{
    return m_User;
}

void ChatMessage::SetUser(const QString &Value)
{
    if (m_User != Value)
    {
        m_User = Value;
        OnUserChanged();
    }
}

QString ChatMessage::GetMessage() const
{
    return m_Message;
}

void ChatMessage::SetMessage(const QString &Value)
{
    if (m_Message != Value)
    {
        m_Message = Value;
        OnMessageChanged();
    }
}
