// Copyright Notices: [...]

module;

#include <boost/asio/ip/tcp.hpp>

module ChatBackEnd.Client;

using namespace ChatBackEnd;

class Client::Impl
{
    boost::asio::io_context &m_Context;
    std::shared_ptr<Service> m_Service;

    std::string m_Host;
    std::int32_t m_Port;

public:
    Impl(boost::asio::io_context &Context, const char *Host, const std::int32_t Port) : m_Context(Context), m_Host(Host), m_Port(Port)
    {
    }

    ~Impl()
    {
        Disconnect();
    }

    void Connect(const boost::function<void(const char *)> &Callback)
    {
        if (m_Service = std::make_shared<Service>(m_Context, m_Host.c_str(), m_Port); m_Service)
        {
            m_Service->Connect(Callback);
        }
    }

    void Disconnect() const
    {
        if (m_Service)
        {
            m_Service->Disconnect();
        }
    }

    void Post(const char *Data)
    {
        if (m_Service)
        {
            m_Service->Post(Data);
        }
    }
};

Client::Client(boost::asio::io_context &Context, const char *Host, const std::int32_t Port)
    : Service(Context, Host, Port)
    , m_Impl(std::make_unique<Impl>(Context, Host, Port))
{
}

Client::~Client()
{
    Disconnect();
}

void Client::Connect(const boost::function<void(const char *)> &Callback)
{
    if (m_Impl)
    {
        m_Impl->Connect(Callback);
    }
}

void Client::Disconnect()
{
    if (m_Impl)
    {
        m_Impl->Disconnect();
    }
}

void Client::Post(const char *Data)
{
    if (m_Impl)
    {
        m_Impl->Post(Data);
    }
}
