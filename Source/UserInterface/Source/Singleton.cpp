// Copyright Notices: [...]

#include "Singleton.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <boost/asio/io_context.hpp>
#include <boost/bind/bind.hpp>
#include <boost/function.hpp>

import ChatBackEnd.SocketInterface;
import ChatBackEnd.Client;
import ChatBackEnd.Server;

using namespace UserInterface;

class Singleton::Handler
{
    boost::asio::io_context m_Context;
    std::jthread m_ContextThread;
    std::function<void(std::string)> m_DataReceivedCallback {};

    std::unique_ptr<ChatBackEnd::SocketInterface> m_Service {};

    void Callback(const char *Data)
    {
        m_DataReceivedCallback({Data});
    }

    template<typename T>
    constexpr void RunService()
    {
        constexpr const char *HostIP = "127.0.0.1";
        constexpr std::uint16_t Port = 25000u;

        if (m_Service = std::make_unique<T>(m_Context, HostIP, Port); m_Service)
        {
            m_Service->Connect(
                    [this](const char *Data)
                    {
                        Callback(Data);
                    });
            m_ContextThread = std::jthread(boost::bind(&boost::asio::io_context::run, &m_Context));
        }
        else
        {
            qDebug() << __FUNCTION__ << ": Failed to establish communication.";
        }
    }

public:
    Handler(const std::function<void(std::string)> &DataReceived) : m_DataReceivedCallback(DataReceived)
    {
    }

    ~Handler() = default;

    void StartServer()
    {
        RunService<ChatBackEnd::Server>();
    }

    void StartClient()
    {
        RunService<ChatBackEnd::Client>();
    }

    void StopService()
    {
        m_Service.reset();
    }

    [[nodiscard]] bool IsRunning() const
    {
        return m_Service && m_Service->IsConnected();
    }

    void PostChatMessage(const QString &User, const QString &Message)
    {
        m_Service->Post(std::data(Message.toStdString()));
    }
};

void Singleton::DataReceived(const std::string &Data)
{
    for (const auto &[ID, Callback]: m_MessageReceivedCallbacks)
    {
        Callback(Data);
    }
}

Singleton::Singleton()
    : m_ServiceHandler(std::make_unique<Singleton::Handler>(
              [this](const std::string &Data)
              {
                  DataReceived(Data);
              }))
{
}

Singleton &Singleton::Get()
{
    static Singleton Instance {};
    return Instance;
}

QString Singleton::GetUser() const
{
    return m_User;
}

void Singleton::SetUser(const QString &Value)
{
    m_User = Value;
}

void Singleton::StartServer()
{
    m_ServiceHandler->StartServer();
}

void Singleton::StartClient()
{
    m_ServiceHandler->StartClient();
}

Singleton::MessageReceivedHandler Singleton::BindMessageReceived(const std::function<void(std::string)> &DataReceived)
{
    static std::atomic<std::uint8_t> ID {0U};

    if (std::empty(m_MessageReceivedCallbacks))
    {
        ID = 0U;
    }

    const std::uint8_t NewID = ID.fetch_add(1U);
    m_MessageReceivedCallbacks.emplace(NewID, DataReceived);

    return NewID;
}

void Singleton::UnbindMessageReceived(const Singleton::MessageReceivedHandler ID)
{
    m_MessageReceivedCallbacks.erase(ID);
}

void Singleton::StopService()
{
    m_ServiceHandler->StopService();
}

bool Singleton::IsRunning() const
{
    return m_ServiceHandler->IsRunning();
}

void Singleton::PostChatMessage(const QString &Message)
{
    m_ServiceHandler->PostChatMessage(m_User, Message);
}

void Singleton::DispatchToMainThread(const std::function<void()> &Function)
{
    QTimer *Timer = new QTimer();
    Timer->moveToThread(qApp->thread());
    Timer->setSingleShot(true);

    QObject::connect(Timer,
                     &QTimer::timeout,
                     [=]()
                     {
                         Function();
                         Timer->deleteLater();
                     });

    QMetaObject::invokeMethod(Timer, "start", Qt::QueuedConnection, Q_ARG(int, 0));
}
