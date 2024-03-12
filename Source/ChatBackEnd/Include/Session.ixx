// Copyright Notices: [...]

module;

#include "ChatBackEndModule.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/function.hpp>

export module ChatBackEnd.Session;

import ChatBackEnd.SocketInterface;

namespace ChatBackEnd
{
    export class CHATBACKENDMODULE_API Session : public SocketInterface
    {
        boost::function<void(const Session *)> m_DisconnectCallback;

    public:
        Session(boost::asio::io_context &Context, boost::asio::ip::tcp::socket Socket, const boost::function<void(const Session *)> &DisconnectCallback);
        ~Session() override;

        void Connect(const boost::function<void(const char *)> &Callback) override;

    protected:
        void ReadCallback(const boost::system::error_code &Error, std::size_t BytesTransferred) override;
    };
} // namespace ChatBackEnd
