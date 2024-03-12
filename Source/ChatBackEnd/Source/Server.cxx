// Copyright Notices: [...]

module;

#include <boost/asio/ip/tcp.hpp>
#include <boost/bind/bind.hpp>
#include <boost/log/trivial.hpp>
#include <queue>

module ChatBackEnd.Server;

import ChatBackEnd.Session;

using namespace ChatBackEnd;

class Server::Impl
{
    mutable std::mutex m_Mutex;

    boost::asio::io_context &m_Context;

    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_Acceptor;
    std::list<std::unique_ptr<Session>> m_Connections;

    std::queue<std::string> m_MessagesQueue;

    bool m_IsConnected;

    boost::function<void(const char *)> m_Callback;

public:
    Impl(boost::asio::io_context &Context, const char *Host, const std::int32_t Port)
        : m_Context(Context)
        , m_Acceptor(std::make_unique<boost::asio::ip::tcp::acceptor>(Context,
                                                                      boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(Host), Port)))
        , m_IsConnected(false)
    {
    }

    ~Impl()
    {
        Disconnect();
    }

    void Connect(const boost::function<void(const char *)> &Callback)
    {
        m_IsConnected = true;
        m_Callback = Callback;

        AcceptConnection();
    }

    void Disconnect()
    {
        try
        {
            m_IsConnected = false;
            m_Acceptor->close();
            m_Connections.clear();

            while (!m_MessagesQueue.empty())
            {
                m_MessagesQueue.pop();
            }
        }
        catch (const std::exception &Exception)
        {
            BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                     << " - An error has occurred: " << Exception.what();
        }
    }

    void Post(const char *Data)
    {
        if (m_Connections.empty())
        {
            m_MessagesQueue.emplace(Data);
        }
        else
        {
            for (const std::unique_ptr<Session> &ConnectionIterator: m_Connections)
            {
                ConnectionIterator->Post(Data);
            }
        }
    }

private:
    void AcceptConnection()
    {
        if (m_IsConnected)
        {
            CheckConnections();

            const auto Callback = [this]<typename ErrorTy, typename SocketTy>(ErrorTy &&Error, SocketTy &&Socket)
            {
                AcceptCallback(std::forward<ErrorTy>(Error), std::forward<SocketTy>(Socket));
            };
            m_Acceptor->async_accept(Callback);
        }
    }

    void AcceptCallback(const boost::system::error_code &Error, boost::asio::ip::tcp::socket Socket)
    {
        if (Error)
        {
            if (Error != boost::asio::error::operation_aborted)
            {
                BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                                         << " - An error has occurred: " << Error.message() << " (" << Error << ")";
            }

            Disconnect();
        }
        else
        {
            BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                     << " - Accepting new Session on: " << Socket.remote_endpoint().address() << ":" << Socket.remote_endpoint().port();

            const auto DisconnectCallback = [this]<typename SessionTy>(SessionTy &&Session)
            {
                OnClientDisconnected(std::forward<SessionTy>(Session));
            };

            m_Connections.push_back(std::make_unique<Session>(m_Context, std::move(Socket), DisconnectCallback));
            m_Connections.back()->Connect(m_Callback);

            while (!m_MessagesQueue.empty())
            {
                const std::string DequeuedMessage = m_MessagesQueue.front();
                BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                         << " - Sending pending message to new connection: " << DequeuedMessage;

                m_Connections.back()->Post(DequeuedMessage.c_str());
                m_MessagesQueue.pop();
            }
        }

        AcceptConnection();
    }

    void CheckConnections()
    {
        std::_Erase_remove_if(m_Connections,
                              [](const std::unique_ptr<Session> &ConnectionIterator)
                              {
                                  return !ConnectionIterator->IsConnected();
                              });

        BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                 << " - Num of existing connections: " << m_Connections.size();
    }

    void OnClientDisconnected(const Session *const Session)
    {
        BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                 << " - A client was disconnected.";

        std::_Erase_remove_if(m_Connections,
                              [Session](const std::unique_ptr<ChatBackEnd::Session> &ConnectionIterator)
                              {
                                  return ConnectionIterator.get() == Session;
                              });

        CheckConnections();
    }
};

Server::Server(boost::asio::io_context &Context, const char *Host, const std::int32_t Port)
    : Service(Context, Host, Port)
    , m_Impl(std::make_unique<Impl>(Context, Host, Port))
{
}

Server::~Server()
{
    Disconnect();
}

void Server::Connect(const boost::function<void(const char *)> &Callback)
{
    if (m_Impl)
    {
        m_Impl->Connect(Callback);
    }
}

void Server::Disconnect()
{
    if (m_Impl)
    {
        m_Impl->Disconnect();
    }
}

void Server::Post(const char *Data)
{
    if (m_Impl)
    {
        m_Impl->Post(Data);
    }
}
