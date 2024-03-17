// Copyright Notices: [...]

#pragma once

#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace UserInterface
{
    class LoginSetup : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString User WRITE SetUser)
        QML_ELEMENT

    public:
        explicit LoginSetup(QObject* Parent = nullptr);

        void SetUser(const QString &);
    };
} // namespace UserInterface
