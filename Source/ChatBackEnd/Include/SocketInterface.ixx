// Copyright Notices: [...]

module;

#include "ChatBackEndModule.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/function.hpp>
#include <string>

export module ChatBackEnd.SocketInterface;

namespace ChatBackEnd
{
    export class CHATBACKENDMODULE_API SocketInterface
    {
    protected:
        std::string m_ReadData;
        std::string m_WriteData;

        mutable std::mutex m_Mutex;

        boost::asio::io_context &m_Context;
        boost::asio::ip::tcp::socket m_Socket;

        boost::function<void(const char *)> m_Callback;

    public:
        explicit SocketInterface(boost::asio::io_context &Context);
        SocketInterface(boost::asio::io_context &Context, boost::asio::ip::tcp::socket Socket);

        virtual ~SocketInterface();

        virtual void Connect(const boost::function<void(const char *)> &Callback);
        virtual void Disconnect();

        virtual bool IsConnected() const;

        virtual void Post(const char *Data);

    protected:
        virtual void DoRead();
        virtual void DoClose();

        virtual void ConnectionCallback(const boost::system::error_code &Error);

        virtual void ReadCallback(const boost::system::error_code &Error, std::size_t BytesTransferred);

        virtual void PostCallback();
        virtual void WriteCallback(const boost::system::error_code &, std::size_t BytesTransferred);
    };
} // namespace ChatBackEnd
