#include <PreCompile.h>

#include "ClientBackend/ClientBackend.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Dungeon");
    
    QGuiApplication app(argc, argv);
    
    ClientBackend* backend = new ClientBackend("127.0.0.1");
    
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("backend", backend);
    
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    
    return app.exec();
}
