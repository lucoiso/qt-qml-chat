// Copyright Notices: [...]

#pragma once

#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace UserInterface
{
    class ChatMessage : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString User READ GetUser WRITE SetUser NOTIFY OnUserChanged)
        Q_PROPERTY(QString Message READ GetMessage WRITE SetMessage NOTIFY OnMessageChanged)
        QML_ELEMENT

        QString m_User {"User"};
        QString m_Message {"Message"};

    public:
        explicit ChatMessage(QObject* Parent = nullptr);

        [[nodiscard]] QString GetUser() const;
        void SetUser(const QString &);

        [[nodiscard]] QString GetMessage() const;
        void SetMessage(const QString &);

        signals:
            void OnUserChanged();
            void OnMessageChanged();
    };
} // namespace UserInterface
