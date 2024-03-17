// Copyright Notices: [...]

#pragma once

#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace UserInterface
{
    class ServiceProvider : public QObject
    {
    public:
        enum class ServiceType
        {
            NONE,
            CLIENT,
            SERVER
        };
        Q_ENUM(ServiceType)

    private:
        Q_OBJECT
        Q_PROPERTY(ServiceType Type READ GetType NOTIFY OnTypeChanged)
        Q_PROPERTY(bool IsConnected READ GetIsConnected NOTIFY OnIsConnectedChanged)
        QML_ELEMENT

        ServiceType m_Type {ServiceType::NONE};

    public:
        explicit ServiceProvider(QObject *Parent = nullptr);

        [[nodiscard]] ServiceType GetType() const;
        [[nodiscard]] bool GetIsConnected() const;

        Q_INVOKABLE void TryConnect(ServiceType);

    signals:
        void OnTypeChanged();
        void OnIsConnectedChanged();
    };
} // namespace UserInterface
