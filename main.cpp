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
    //REMEMBER TO RESET THE SAVE FILE WHEN YOU ADD A MARY SIGNAL!!!
    //Toby To Max Signals
    QObject::connect(&_toby, SIGNAL(newCVMatFrameGrabbed(const cv::Mat&)), &_max, SLOT(recieveNewCVMat(const cv::Mat&)));
    //Mary To Toby Signals
    QObject::connect(&_mary, SIGNAL(signalChangeCameraAOI(int,int)), &_toby, SLOT(onChangeCameraAOI(int,int)));
    QObject::connect(&_mary, SIGNAL(signalChangeCameraExposure(double)), &_toby, SLOT(onChangeCameraExposure(double)));
    QObject::connect(&_mary, SIGNAL(signalChangeCameraGain(int)), &_toby, SLOT(onChangeCameraGain(int)));
    //Mary To Max Signals
    QObject::connect(&_mary, SIGNAL(signalChangeBlur(int)), &_max, SLOT(onBlurChange(int)));
    QObject::connect(&_mary, SIGNAL(signalChangeThresholdMin(int)), &_max, SLOT(onThresholdMinChange(int)));
    QObject::connect(&_mary, SIGNAL(signalChangeThresholdMax(int)), &_max, SLOT(onThresholdMaxChange(int)));

    QObject::connect(&_mary, SIGNAL(signalChangeMaxTII(quint64)), &_max, SLOT(onMaxTIIChange(quint64)));
    QObject::connect(&_mary, SIGNAL(signalChangeMinTII(quint64)), &_max, SLOT(onMinTIIChange(quint64)));
    QObject::connect(&_mary, SIGNAL(signalChangeMaxClusterSize(int)), &_max, SLOT(onMaxClusterSizeChange(int)));
    QObject::connect(&_mary, SIGNAL(signalChangeMaxClustersInColumn(int)), &_max, SLOT(onMaxClustersInColChange(int)));
    QObject::connect(&_mary, SIGNAL(signalChangeMinClusterSize(int)), &_max, SLOT(onMinClusterSizeChange(int)));

    QObject::connect(&_mary, SIGNAL(signalChangeMaxDiscontinuity(int)), &_max, SLOT(onMaxDiscontinuityChange(int)));

    QObject::connect(&_mary, SIGNAL(signalLeftTSLMinAngle(float)), &_max, SLOT(onLeftTSLMinAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftTSLMaxAngle(float)), &_max, SLOT(onLeftTSLMaxAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftTSLMinVotes(int)), &_max, SLOT(onLeftTSLMinVotes(int)));
    QObject::connect(&_mary, SIGNAL(signalLeftTSLDistanceThreshold(float)), &_max, SLOT(onLeftTSLDistanceThreshold(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftTSLIterations(int)), &_max, SLOT(onLeftTSLIterations(int)));

    QObject::connect(&_mary, SIGNAL(signalRightTSLMinAngle(float)), &_max, SLOT(onRightTSLMinAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalRightTSLMaxAngle(float)), &_max, SLOT(onRightTSLMaxAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalRightTSLMinVotes(int)), &_max, SLOT(onRightTSLMinVotes(int)));
    QObject::connect(&_mary, SIGNAL(signalRightTSLDistanceThreshold(float)), &_max, SLOT(onRightTSLDistanceThreshold(float)));
    QObject::connect(&_mary, SIGNAL(signalRightTSLIterations(int)), &_max, SLOT(onRightTSLIterations(int)));

    QObject::connect(&_mary, SIGNAL(signalLeftBWLMinAngle(float)), &_max, SLOT(onLeftBWLMinAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftBWLMaxAngle(float)), &_max, SLOT(onLeftBWLMaxAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftBWLMinVotes(int)), &_max, SLOT(onLeftBWLMinVotes(int)));
    QObject::connect(&_mary, SIGNAL(signalLeftBWLDistanceThreshold(float)), &_max, SLOT(onLeftBWLDistanceThreshold(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftBWLIterations(int)), &_max, SLOT(onLeftBWLIterations(int)));

    QObject::connect(&_mary, SIGNAL(signalRightBWLMinAngle(float)), &_max, SLOT(onRightBWLMinAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalRightBWLMaxAngle(float)), &_max, SLOT(onRightBWLMaxAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalRightBWLMinVotes(int)), &_max, SLOT(onRightBWLMinVotes(int)));
    QObject::connect(&_mary, SIGNAL(signalRightBWLDistanceThreshold(float)), &_max, SLOT(onRightBWLDistanceThreshold(float)));
    QObject::connect(&_mary, SIGNAL(signalRightBWLIterations(int)), &_max, SLOT(onRightBWLIterations(int)));

    QObject::connect(&_mary, SIGNAL(signalSplitDistance(float)), &_max, SLOT(onSplitDistance(float)));
    QObject::connect(&_mary, SIGNAL(signalSplitLength(float)), &_max, SLOT(onSplitLength(float)));
    //Set Mary's Default Values... This must Be Done After the Signals and Slot of the Singletons are connected.
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
