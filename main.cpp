#include <pylon/PylonIncludes.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gary.h"
#include "toby.h"
#include "max.h"
#include "mary.h"
#include "QmlTBIDisplay.h"
#include "opencv4/opencv2/core.hpp"

using namespace Pylon;
using namespace cv;

Q_DECLARE_METATYPE(cv::Mat) //So Mat Can Be Passed Thru Signals

int main(int argc, char *argv[])
{

    //Init the Pylon Run Time.
    //Required For Use of the Pylon Function Calls
    Pylon::PylonAutoInitTerm pylonAutoInitTerm;


    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif

    QGuiApplication app(argc, argv);

    //Register the QML Meta Types So That QML Can Use Them
    qRegisterMetaType< Mat >("Mat"); //OpenCV Mat Type. So Mats can be passed thru Signals.


    //------QML Component Registration ---------------------------
    //Register the Custom Gary QML Data Types.
    GaryHomingStatus::declareQML();
    GaryControlErrorCode::declareQML();
    GaryOperationStatus::declareQML();
    GaryLimitSwitch::declareQML();
    GaryControlMode::declareQML();
    GaryCommands::declareQML();
    GaryMotionStatus::declareQML();
    //Register The QmlTBIDisplay QML Type
    QmlTBIDisplay::declareQML();

    //The QML Application engine----------------------------------
    QQmlApplicationEngine engine;

    //instantiated Objects of Application-------------------------
    Gary _gary; //MicroController Singleton
    Toby _toby; //Basler Camera Singleton
    Max _max; //Seam Tracking Singleton
    Mary _mary; //Settings Singleton

    //Connect The Singletons Signals and Slots---------------------
    QObject::connect(&_toby, SIGNAL(newCVMatFrameGrabbed(const cv::Mat&)), &_max, SLOT(recieveNewCVMat(const cv::Mat&)));
    QObject::connect(&_mary, SIGNAL(signalChangeCameraAOI(int,int)), &_toby, SLOT(onChangeCameraAOI(int,int)));

    //Set Mary's Default Values... This must Be Done After the Signals and Slot of the Singletons are connected.
    _mary.SetMaryDefaultValues();
    _mary.loadMaryFromFile();

    //Adding Singletons as Root Properties to QML--------------------------------
    engine.rootContext()->setContextProperty("Gary", &_gary);
    engine.rootContext()->setContextProperty("Toby", &_toby);
    engine.rootContext()->setContextProperty("Max", &_max);
    engine.rootContext()->setContextProperty("Mary", &_mary);

    //QML Code-----------------------------------------------------
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    //QML App Is Running Now--------------------------------------------
    //Exit App.
    return app.exec();
}
