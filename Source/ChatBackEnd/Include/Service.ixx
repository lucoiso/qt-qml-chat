// Copyright Notices: [...]

module;

#include "ChatBackEndModule.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/function.hpp>

export module ChatBackEnd.Service;

import ChatBackEnd.SocketInterface;

namespace ChatBackEnd
{
    export class CHATBACKENDMODULE_API Service : public SocketInterface
    {
        std::string m_Host {};
        std::uint16_t m_Port {0U};

    public:
        Service(boost::asio::io_context &, std::string_view, std::uint16_t);

        void Connect(const boost::function<void(std::string)> &) override;
    };
} // namespace ChatBackEnd
