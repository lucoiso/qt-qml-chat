// Copyright Notices: [...]

#include "LoginSetup.hpp"
#include "Singleton.hpp"

using namespace UserInterface;

LoginSetup::LoginSetup(QObject *Parent) : QObject(Parent)
{
}

void LoginSetup::SetUser(const QString &Value)
{
    Singleton::Get().SetUser(Value);
}
