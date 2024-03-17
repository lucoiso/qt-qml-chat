// Copyright Notices: [...]

#include "ServiceProvider.hpp"

#include <execution>
#include <future>

#include "Singleton.hpp"

using namespace UserInterface;

ServiceProvider::ServiceProvider(QObject *Parent) : QObject(Parent), m_Timer(this)
{
    m_Timer.setInterval(500);

    QObject::connect(&m_Timer,
                     &QTimer::timeout,
                     [this]
                     {
                         static bool ServiceRunning = false;

                         if (const bool IsRunning = Singleton::Get().IsRunning(); ServiceRunning != IsRunning)
                         {
                             ServiceRunning = IsRunning;
                             OnIsConnectedChanged();
                         }
                     });

    m_Timer.start();
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

        m_Type = Value;
        OnTypeChanged();
    }
}
