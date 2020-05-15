#include <QtQml>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Backend/BackendBase.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    
    QGuiApplication app(argc, argv);
    
    qmlRegisterType<BackendBase>("nsp.DS.Backend", 1, 14, "Backend");
    
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl
    ) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    
    return app.exec();
}
