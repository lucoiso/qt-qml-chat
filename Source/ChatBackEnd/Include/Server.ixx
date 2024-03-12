// Copyright Notices: [...]

module;

#include "ChatBackEndModule.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/function.hpp>

export module ChatBackEnd.Server;

import ChatBackEnd.Service;

namespace ChatBackEnd
{
    export class CHATBACKENDMODULE_API Server final : public Service
    {
        class Impl;
        std::unique_ptr<Impl> m_Impl;

    public:
        Server(boost::asio::io_context &Context, const char *Host, std::int32_t Port);
        ~Server() override;

        void Connect(const boost::function<void(const char *)> &Callback) override;
        void Disconnect() override;

        void Post(const char *Data) override;
    };
} // namespace ChatBackEnd
