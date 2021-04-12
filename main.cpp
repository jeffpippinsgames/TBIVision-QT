#include <pylon/PylonIncludes.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gary.h"
#include "toby.h"

using namespace Pylon;

int main(int argc, char *argv[])
{

    //Init the Pylon Run Time.
    //Required For Use of the Pylon Function Calls
    Pylon::PylonInitialize();


    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif

    QGuiApplication app(argc, argv);

    //Register the Custom Gary QML Types.
    GaryHomingStatus::declareQML();
    GaryControlErrorCode::declareQML();
    GaryOperationStatus::declareQML();
    GaryLimitSwitch::declareQML();
    GaryControlMode::declareQML();
    GaryCommands::declareQML();
    GaryMotionStatus::declareQML();

    //The QML Application engine----------------------------------
    QQmlApplicationEngine engine;

    //instantiated Objects of Application-------------------------
    Gary _gary; //MicroController Singleton
    Toby _toby; //Basler Camera Singleton

    //Adding Root Properties to QML--------------------------------
    engine.rootContext()->setContextProperty("Gary", &_gary);
    engine.rootContext()->setContextProperty("Toby", &_toby);

    //QML Code-----------------------------------------------------
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
