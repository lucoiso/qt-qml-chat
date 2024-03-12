// Copyright Notices: [...]

module;

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/log/trivial.hpp>

module ChatBackEnd.SocketInterface;

using namespace ChatBackEnd;

SocketInterface::SocketInterface(boost::asio::io_context &Context) : m_Context(Context), m_Socket(Context)
{
}

SocketInterface::SocketInterface(boost::asio::io_context &Context, boost::asio::ip::tcp::socket Socket) : m_Context(Context), m_Socket(std::move(Socket))
{
}

SocketInterface::~SocketInterface()
{
    SocketInterface::Disconnect();
}

void SocketInterface::DoRead()
{
    try
    {
        const auto Callback = [this]<typename ErrorTy, typename BytesTy>(ErrorTy &&Error, BytesTy &&Bytes)
        {
            ReadCallback(std::forward<ErrorTy>(Error), std::forward<BytesTy>(Bytes));
        };
        async_read_until(m_Socket, boost::asio::dynamic_buffer(m_ReadData), '\n', Callback);
    }
    catch (const std::exception &Exception)
    {
        BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Exception.what();
    }
}

void SocketInterface::Connect(const boost::function<void(const char *)> &Callback)
{
    m_Callback = Callback;
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                             << " - Starting Connection...";
}

void SocketInterface::Disconnect()
{
    try
    {
        post(m_Context,
             [this]
             {
                 DoClose();
             });
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
    return m_Socket.is_open();
}

void SocketInterface::Post(const char *Data)
{
    m_WriteData = Data;
    m_WriteData += '\n';

    try
    {
        post(m_Context,
             [this]
             {
                 PostCallback();
             });
    }
    catch (const std::exception &Exception)
    {
        BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Exception.what();
    }
}

void SocketInterface::DoClose()
{
    if (m_Socket.is_open())
    {
        boost::system::error_code Error;
        [[maybe_unused]] auto Dicard = m_Socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, Error);
        m_Socket.close();
    }
}

void SocketInterface::ConnectionCallback(const boost::system::error_code &Error)
{
    if (!Error)
    {
        DoRead();
    }
    else
    {
        DoClose();
    }
}

void SocketInterface::ReadCallback(const boost::system::error_code &Error, [[maybe_unused]] std::size_t BytesTransferred)
{
    std::lock_guard Lock(m_Mutex);

    if (Error && Error != boost::asio::error::eof && Error != boost::asio::error::operation_aborted && Error.category() != boost::asio::error::system_category)
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Error.message() << " (" << Error << ")";
    }
    else
    {
        if (!m_ReadData.empty())
        {
            m_ReadData.erase(std::remove(m_ReadData.begin(), m_ReadData.end(), '\n'), m_ReadData.cend());
            m_Callback(m_ReadData.c_str());

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
    std::lock_guard Lock(m_Mutex);

    try
    {
        const auto Callback = [this]<typename ErrorTy, typename BytesTy>(ErrorTy &&Error, BytesTy &&Bytes)
        {
            WriteCallback(std::forward<ErrorTy>(Error), std::forward<BytesTy>(Bytes));
        };
        async_write(m_Socket, boost::asio::buffer(m_WriteData.data(), m_WriteData.length()), Callback);
    }
    catch (const std::exception &Exception)
    {
        BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: "
                                 << " - An error has occurred: " << Exception.what();
    }
}

void SocketInterface::WriteCallback(const boost::system::error_code &Error, std::size_t BytesTransferred)
{
    std::lock_guard Lock(m_Mutex);

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
