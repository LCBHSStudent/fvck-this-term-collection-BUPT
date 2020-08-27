#include <PreCompile.h>

#include "ClientBackend/ClientBackend.h"

int main(int argc, char *argv[]) {
    // 启用高像素密度缩放
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // 供lab.setting记录登录用户名与密码必要的数据库参数
    QCoreApplication::setOrganizationName("Dungeon");
    
    QGuiApplication app(argc, argv);
    // 建立客户端backend，指定服务器ip
    ClientBackend* backend = new ClientBackend("39.106.219.180");
    // 建立qml引擎实例
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("backend", backend);
    // 加载主qml，连接退出信号
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    // 进入事件循环
    return app.exec();
}
