// Copyright Notices: [...]

module;

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind/bind.hpp>
#include <boost/log/trivial.hpp>

module ChatBackEnd.SocketInterface;

using namespace ChatBackEnd;

SocketInterface::SocketInterface(boost::asio::io_context &Context) : m_Context(Context), m_Socket(Context)
{
}

SocketInterface::SocketInterface(boost::asio::io_context &Context, boost::asio::ip::tcp::socket Socket) : m_Context(Context), m_Socket(std::move(Socket))
{
}

void SocketInterface::DoRead()
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                             << " - Read requested";

    try
    {
        auto Callback = boost::bind(&SocketInterface::ReadCallback, this, boost::placeholders::_1, boost::placeholders::_2);
        boost::asio::async_read_until(m_Socket, boost::asio::dynamic_buffer(m_ReadData), '\n', Callback);
    }
    catch (const std::exception &Exception)
    {
        BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Exception.what();
    }
}

void SocketInterface::Connect(const boost::function<void(std::string)> &Callback)
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                             << " - Connect requested";

    m_Callback = Callback;
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                             << " - Starting Connection";
}

void SocketInterface::Disconnect()
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                             << " - Disconnect requested";

    m_IsConnected = false;

    try
    {
        boost::asio::post(m_Context, boost::bind(&SocketInterface::DoClose, this));
    }
    catch ([[maybe_unused]] const std::bad_weak_ptr &BadWeakPtrException)
    {
        // Do nothing: The socket is being closed
    }
    catch (const std::exception &Exception)
    {
        BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Exception.what();
    }
}

bool SocketInterface::IsConnected() const
{
    return m_IsConnected;
}

void SocketInterface::Post(const std::string_view Data)
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                             << " - Post requested with data: " << Data;

    m_WriteData = std::format("{}\n", Data);

    try
    {
        boost::asio::post(m_Context, boost::bind(&SocketInterface::PostCallback, this));
    }
    catch (const std::exception &Exception)
    {
        BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Exception.what();
    }
}

void SocketInterface::DoClose()
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                             << " - Close requested";

    m_IsConnected = false;

    if (m_Socket.is_open())
    {
        boost::system::error_code Error;
        [[maybe_unused]] auto Discard = m_Socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, Error);
        m_Socket.close();
    }
}

void SocketInterface::ConnectionCallback(const boost::system::error_code &Error)
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                             << " - Connection callback reached";

    if (!Error)
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                                 << " - Connection established";

        m_IsConnected = true;
        DoRead();
    }
    else
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                                 << " - Connection Failed";

        DoClose();
    }
}

void SocketInterface::ReadCallback(const boost::system::error_code &Error, [[maybe_unused]] std::size_t BytesTransferred)
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                             << " - Read callback reached";

    if (Error && Error != boost::asio::error::eof && Error != boost::asio::error::operation_aborted && Error.category() != boost::asio::error::system_category)
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Error.message() << " (" << Error << ")";
    }
    else
    {
        if (!m_ReadData.empty())
        {
            std::_Erase_remove(m_ReadData, '\n');
            m_Callback(m_ReadData);

            BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                                     << " - Received Data: '" << m_ReadData << "'";
            m_ReadData.clear();
        }

        if (!Error)
        {
            DoRead();
        }
        else
        {
            DoClose();
        }
    }
}

void SocketInterface::PostCallback()
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                             << " - Post callback reached";

    try
    {
        const auto Callback = boost::bind(&SocketInterface::WriteCallback, this, boost::placeholders::_1, boost::placeholders::_2);
        boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteData.data(), m_WriteData.length()), Callback);
    }
    catch (const std::exception &Exception)
    {
        BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Exception.what();
    }
}

void SocketInterface::WriteCallback(const boost::system::error_code &Error, std::size_t BytesTransferred)
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Write callback reached";

    if (Error && Error != boost::asio::error::eof)
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Error.message() << " (" << Error << ")";
    }
    else
    {
        m_WriteData.erase(std::remove(m_WriteData.begin(), m_WriteData.end(), '\n'), m_WriteData.cend());
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                                 << " - Data sent: '" << m_WriteData << "' Bytes transferred: '" << BytesTransferred << "'";
        m_WriteData.clear();
    }
}
