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
        boost::function<void(const Session *)> m_DisconnectCallback {};

    public:
        Session(boost::asio::io_context &, boost::asio::ip::tcp::socket, const boost::function<void(const Session *)> &);

        void Connect(const boost::function<void(std::string)> &) override;

    protected:
        void ReadCallback(const boost::system::error_code &, std::size_t) override;
    };
} // namespace ChatBackEnd
