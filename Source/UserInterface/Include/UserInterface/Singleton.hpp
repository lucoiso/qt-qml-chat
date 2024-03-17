// Copyright Notices: [...]

#pragma once

#include <QString>
#include <memory>
#include <unordered_map>

namespace UserInterface
{
    class Singleton
    {
        bool m_IsRunning {false};
        QString m_User {"User"};

        class Handler;
        std::unique_ptr<Handler> m_ServiceHandler;

        std::unordered_map<std::uint8_t, std::function<void(std::string)>> m_MessageReceivedCallbacks {};

        void DataReceived(const std::string &);

    public:
        Singleton();

        static Singleton &Get();

        [[nodiscard]] QString GetUser() const;
        void SetUser(const QString &);

        void StartServer();
        void StartClient();

        using MessageReceivedHandler = std::uint8_t;
        [[nodiscard]] MessageReceivedHandler BindMessageReceived(const std::function<void(std::string)> &);
        void UnbindMessageReceived(MessageReceivedHandler);

        void StopService();
        [[nodiscard]] bool IsRunning() const;

        void PostChatMessage(const QString &);

        void DispatchToMainThread(const std::function<void()> &);
    };
} // namespace UserInterface
