#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <skqtauth.h>
#include <skqtrealtimedatabase.h>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<SKQtAuth>("SKQtAuth", 1, 0, "SKQtAuth");
    qmlRegisterType<SKQtRealTimeDatabase>("SKQtRealTimeDatabase", 1, 0, "SKQtRealTimeDatabase");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
