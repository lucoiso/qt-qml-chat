// Copyright Notices: [...]

module;

#include "ChatBackEndModule.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/function.hpp>

export module ChatBackEnd.Client;

import ChatBackEnd.Service;

namespace ChatBackEnd
{
    export class CHATBACKENDMODULE_API Client final : public Service
    {
        class Impl;
        std::unique_ptr<Impl> m_Impl;

    public:
        Client(boost::asio::io_context &, std::string_view, std::uint16_t);
        ~Client();

        void Connect(const boost::function<void(std::string)> &) override;
        void Disconnect() override;

        void Post(std::string_view) override;
    };
} // namespace ChatBackEnd
