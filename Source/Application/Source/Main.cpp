// Copyright Notices: [...]

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include <QtQml/QQmlExtensionPlugin>
Q_IMPORT_QML_PLUGIN(UserInterfacePlugin)

int main(int Argc, char *Argv[])
{
    QGuiApplication const Application(Argc, Argv);
    QQmlApplicationEngine AppEngine;

    QQuickStyle::setStyle("Material");

    QObject::connect(
            &AppEngine,
            &QQmlApplicationEngine::objectCreationFailed,
            &Application,
            []
            {
                QCoreApplication::exit(EXIT_FAILURE);
            },
            Qt::QueuedConnection);

    AppEngine.load(QUrl(u"qrc:/UserInterface/MainWindow.qml"_qs));

    return QGuiApplication::exec();
}