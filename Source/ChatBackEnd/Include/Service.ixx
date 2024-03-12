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
        std::string m_Host;
        std::int32_t m_Port;

    public:
        Service(boost::asio::io_context &Context, const char *Host, std::int32_t Port);
        ~Service() override;

        void Connect(const boost::function<void(const char *)> &Callback) override;
    };
} // namespace ChatBackEnd
