// Copyright Notices: [...]

#include "ChatBoard.hpp"

#include <memory>

using namespace UserInterface;

void ChatBoard::AppendMessage(QQmlListProperty<ChatMessage> *List, ChatMessage *Message)
{
    reinterpret_cast<ChatBoard *>(List->object)->AppendMessage(Message);
}

qsizetype ChatBoard::CountMessages(QQmlListProperty<ChatMessage> *List)
{
    return reinterpret_cast<ChatBoard *>(List->object)->CountMessages();
}

ChatMessage *ChatBoard::GetMessageAt(QQmlListProperty<ChatMessage> *List, const qsizetype Index)
{
    return reinterpret_cast<ChatBoard *>(List->object)->GetMessageAt(Index);
}

auto ChatBoard::ClearMessages(QQmlListProperty<ChatMessage> *List) -> void
{
    reinterpret_cast<ChatBoard *>(List->object)->ClearMessages();
}

void ChatBoard::ReplaceMessage(QQmlListProperty<ChatMessage> *List, const qsizetype Index, ChatMessage *Message)
{
    reinterpret_cast<ChatBoard *>(List->object)->ReplaceMessage(Index, Message);
}

void ChatBoard::PopMessage(QQmlListProperty<ChatMessage> *List)
{
    reinterpret_cast<ChatBoard *>(List->object)->PopMessage();
}

ChatBoard::ChatBoard(QObject *Parent) : QObject(Parent)
{
}

QString ChatBoard::GetUser() const
{
    return m_User;
}

void ChatBoard::SetUser(const QString &Value)
{
    if (m_User != Value)
    {
        m_User = Value;
        OnUserChanged();
    }
}

QQmlListProperty<ChatMessage> ChatBoard::GetHistory()
{
    return QQmlListProperty<ChatMessage> {this,
                                          this,
                                          &ChatBoard::AppendMessage,
                                          &ChatBoard::CountMessages,
                                          &ChatBoard::GetMessageAt,
                                          &ChatBoard::ClearMessages,
                                          &ChatBoard::ReplaceMessage,
                                          &ChatBoard::PopMessage};
}

void ChatBoard::AppendMessage(ChatMessage *Message)
{
    m_History.append(Message);
    OnHistoryChanged();
}

qsizetype ChatBoard::CountMessages() const
{
    return m_History.count();
}

ChatMessage *ChatBoard::GetMessageAt(const qsizetype Index) const
{
    return m_History.at(Index);
}

void ChatBoard::ClearMessages()
{
    if (!std::empty(m_History))
    {
        m_History.clear();
        OnHistoryChanged();
    }
}

void ChatBoard::ReplaceMessage(const qsizetype Index, ChatMessage *Message)
{
    if (m_History.at(Index) != Message)
    {
        m_History[Index] = Message;
        OnHistoryChanged();
    }
}

void ChatBoard::PopMessage()
{
    if (!std::empty(m_History))
    {
        m_History.removeLast();
        OnHistoryChanged();
    }
}

void ChatBoard::PostMessage(const QString &Message)
{
    const auto NewMessage = new ChatMessage(this);
    NewMessage->SetUser(GetUser());
    NewMessage->SetMessage(Message);
    AppendMessage(NewMessage);
}
