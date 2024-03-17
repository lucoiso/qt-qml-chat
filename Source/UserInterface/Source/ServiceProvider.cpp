// Copyright Notices: [...]

#include "ServiceProvider.hpp"

#include "Singleton.hpp"

using namespace UserInterface;

ServiceProvider::ServiceProvider(QObject *Parent) : QObject(Parent)
{
}

ServiceProvider::ServiceType ServiceProvider::GetType() const
{
    return m_Type;
}

bool ServiceProvider::GetIsConnected() const
{
    return Singleton::Get().IsRunning();
}

void ServiceProvider::TryConnect(const ServiceType Value)
{
    if (const bool IsRunning = Singleton::Get().IsRunning(); !IsRunning || m_Type != Value)
    {
        switch (Value)
        {
            case ServiceType::CLIENT:
            {
                Singleton::Get().StartClient();
                break;
            }
            case ServiceType::SERVER:
            {
                Singleton::Get().StartServer();
                break;
            }
            case ServiceType::NONE:
            {
                Singleton::Get().StopService();
                break;
            }
            default:
                return;
        }

        if (const bool Result = Singleton::Get().IsRunning(); Result != IsRunning)
        {
            m_Type = Value;
            OnTypeChanged();
            OnIsConnectedChanged();
        }
    }
}
