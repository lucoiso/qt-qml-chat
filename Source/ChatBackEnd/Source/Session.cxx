// Copyright Notices: [...]

module;

#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>

module ChatBackEnd.Session;

using namespace ChatBackEnd;

Session::Session(boost::asio::io_context &Context, boost::asio::ip::tcp::socket Socket, const boost::function<void(const Session *)> &DisconnectCallback)
    : SocketInterface(Context, std::move(Socket))
    , m_DisconnectCallback(DisconnectCallback)
{
}

Session::~Session()
{
    Disconnect();
}

void Session::Connect(const boost::function<void(const char *)> &Callback)
{
    SocketInterface::Connect(Callback);

    try
    {
        DoRead();
    }
    catch (const std::exception &Exception)
    {
        BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Exception.what();
    }
}

void Session::ReadCallback(const boost::system::error_code &Error, std::size_t BytesTransferred)
{
    SocketInterface::ReadCallback(Error, BytesTransferred);

    if (Error && Error == boost::asio::error::eof || Error == boost::asio::error::connection_reset)
    {
        m_DisconnectCallback(this);
    }
}
