#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include <chrono>
#include <thread>
#include "backend.h"
#include "remotecontrol.h"
#include <iostream>
int main(int argc, char *argv[])

{

    qmlRegisterType<BackEnd>("io.qt.examples.backend", 1, 0, "BackEnd");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();

    int retVal = 0;
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("cid")) {
        std::cerr << argv[0] << " sends (not just) steering instructions to the BeagleBone."
                  << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OD4Session toBeagleBone>"
                  << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=111" << std::endl;
        retVal = 1;
    }
    else{
        std::cout << "help" << std::endl;
        uint16_t const CID = (uint16_t) std::stoi(commandlineArguments["cid"]);

        remote(CID);
    }
}
