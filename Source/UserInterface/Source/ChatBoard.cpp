// Copyright Notices: [...]

#include "ChatBoard.hpp"
#include "Singleton.hpp"

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

void ChatBoard::OnMessageReceived(const QString &User, const QString &Message)
{
    const auto NewMessage = new ChatMessage(this);
    NewMessage->SetUser(User);
    NewMessage->SetMessage(Message);
    AppendMessage(NewMessage);
}

ChatBoard::ChatBoard(QObject *Parent) : QObject(Parent)
{
    m_MessageReceivedHandler = Singleton::Get().BindMessageReceived(
            [this](const std::string &Data)
            {
                Singleton::Get().DispatchToMainThread(
                        [this, Data]
                        {
                            OnMessageReceived("Undefined", QString::fromStdString(Data));
                        });
            });
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
    m_History.push_front(Message);
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
void ChatBoard::PostChatMessage(const QString &Message)
{
    Singleton::Get().PostChatMessage(Message);
    OnMessageReceived(Singleton::Get().GetUser(), Message);
}
