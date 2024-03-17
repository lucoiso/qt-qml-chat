// Copyright Notices: [...]

#include "Singleton.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <boost/asio/io_context.hpp>
#include <boost/bind/bind.hpp>
#include <boost/function.hpp>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>

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

    template<typename T>
    constexpr void RunService()
    {
        constexpr const char *HostIP = "127.0.0.1";
        constexpr std::uint16_t Port = 25000u;

        if (m_Service = std::make_unique<T>(m_Context, HostIP, Port); m_Service)
        {
            m_Service->Connect(
                    [this](const std::string &Data)
                    {
                        m_DataReceivedCallback({Data});
                    });

            m_ContextThread = std::jthread(
                    [this](const std::stop_token &Token)
                    {
                        while (!Token.stop_requested())
                        {
                            m_Context.run_one();
                        }
                    });
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
        if (m_Service)
        {
            m_Service->Disconnect();
            m_Service.reset();
        }
    }

    [[nodiscard]] bool IsRunning() const
    {
        return m_Service && m_Service->IsConnected();
    }

    void PostChatMessage(const std::string_view Data)
    {
        m_Service->Post(Data);
    }
};

Singleton::Singleton()
    : m_ServiceHandler(std::make_unique<Singleton::Handler>(
              [this](const std::string &Data)
              {
                  DataReceived(Data);
              }))
{
    QObject::connect(qApp,
                     &QCoreApplication::aboutToQuit,
                     [this]
                     {
                         if (m_ServiceHandler)
                         {
                             m_ServiceHandler->StopService();
                         }
                     });
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

Singleton::MessageReceivedHandler Singleton::BindMessageReceived(const std::function<void(QString, QString)> &DataReceived)
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
    boost::json::object MessageObject;
    MessageObject.emplace("user", m_User.toStdString());
    MessageObject.emplace("message", Message.toStdString());

    m_ServiceHandler->PostChatMessage(serialize(MessageObject));
}

void Singleton::DataReceived(const std::string &Data)
{
    boost::json::value const JsonContent = boost::json::parse(Data);
    boost::json::object const &JsonObject = JsonContent.get_object();

    const QString UserObj = QString::fromStdString(std::data(JsonObject.at("user").as_string()));
    const QString MessageObj = QString::fromStdString(std::data(JsonObject.at("message").as_string()));

    for (const auto &[ID, Callback]: m_MessageReceivedCallbacks)
    {
        Callback(UserObj, MessageObj);
    }
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
