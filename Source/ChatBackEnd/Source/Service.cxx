// Copyright Notices: [...]

module;

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind/bind.hpp>
#include <boost/log/trivial.hpp>

module ChatBackEnd.Service;

using namespace ChatBackEnd;

Service::Service(boost::asio::io_context &Context, const std::string_view Host, const std::uint16_t Port) : SocketInterface(Context), m_Host(Host), m_Port(Port)
{
}

void Service::Connect(const boost::function<void(std::string)> &Callback)
{
    SocketInterface::Connect(Callback);

    try
    {
        boost::asio::ip::tcp::resolver Resolver(m_Context);
        const boost::asio::ip::tcp::resolver::results_type Results = Resolver.resolve(m_Host, std::to_string(m_Port));

        const auto AsyncCallback = boost::bind(&Service::ConnectionCallback, this, boost::placeholders::_1);
        boost::asio::async_connect(m_Socket, Results, AsyncCallback);
    }
    catch (const std::exception &Exception)
    {
        BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Exception.what();
    }
}
