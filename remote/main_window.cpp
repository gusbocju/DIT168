#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include <chrono>
#include <thread>
#include "backend.h"
#include <iostream>
int main(int argc, char *argv[])

{

    qmlRegisterType<BackEnd>("io.qt.examples.backend", 1, 0, "BackEnd");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main_window.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();


}
