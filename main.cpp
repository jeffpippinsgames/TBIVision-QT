#include <pylon/PylonIncludes.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tbicore_gary.h"
#include "tbicore_toby.h"
#include "tbicore_max.h"
#include "tbicore_mary.h"
#include "tbiqml_display.h"
#include "opencv4/opencv2/core.hpp"
#include "tbiweld_enumerator.h"
#include "tbiweld_pipelineprocessingreturntype.h"
//#include "tbilinearransac.h"

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
    GaryLaserStatus::declareQML();
    GaryMotorCalibrationCycleStatus::declareQML();
    GaryControllerStatus::declareQML();
    GaryControllerQMLSignals::declareQML();
    //Register The QmlTBIDisplay QML Type
    QmlTBIDisplay::declareQML();
    //Register TBI Weld Types
    TBIWeldType_Enumerator::declareQML();
    TBIWeld_ProcessingPipeLineReturnType::declareQML();



    //The QML Application engine----------------------------------
    QQmlApplicationEngine engine;

    //instantiated Objects of Application-------------------------
    Gary _gary; //MicroController Singleton
    Toby _toby; //Basler Camera Singleton
    Max _max; //Seam Tracking Singleton
    Mary _mary; //Settings Singleton

    //Singleton Hooks
    _max.setGary(&_gary);
    _max.setMary(&_mary);

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

    QObject::connect(&_mary, SIGNAL(signalLeftTSLIdealAngle(float)), &_max, SLOT(onLeftTSLIdealAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftTSLAllowedAngleVariance(float)), &_max, SLOT(onLeftTSLAllowedAngleVariance(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftTSLMinVotes(int)), &_max, SLOT(onLeftTSLMinVotes(int)));
    QObject::connect(&_mary, SIGNAL(signalLeftTSLDistanceThreshold(float)), &_max, SLOT(onLeftTSLDistanceThreshold(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftTSLIterations(int)), &_max, SLOT(onLeftTSLIterations(int)));

    QObject::connect(&_mary, SIGNAL(signalRightTSLIdealAngle(float)), &_max, SLOT(onRightTSLIdealAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalRightTSLAllowedAngleVariance(float)), &_max, SLOT(onRightTSLAllowedAngleVariance(float)));
    QObject::connect(&_mary, SIGNAL(signalRightTSLMinVotes(int)), &_max, SLOT(onRightTSLMinVotes(int)));
    QObject::connect(&_mary, SIGNAL(signalRightTSLDistanceThreshold(float)), &_max, SLOT(onRightTSLDistanceThreshold(float)));
    QObject::connect(&_mary, SIGNAL(signalRightTSLIterations(int)), &_max, SLOT(onRightTSLIterations(int)));

    QObject::connect(&_mary, SIGNAL(signalLeftBWLIdealAngle(float)), &_max, SLOT(onLeftBWLIdealAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftBWLAllowedAngleVariance(float)), &_max, SLOT(onLeftBWLAllowedAngleVariance(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftBWLMinVotes(int)), &_max, SLOT(onLeftBWLMinVotes(int)));
    QObject::connect(&_mary, SIGNAL(signalLeftBWLDistanceThreshold(float)), &_max, SLOT(onLeftBWLDistanceThreshold(float)));
    QObject::connect(&_mary, SIGNAL(signalLeftBWLIterations(int)), &_max, SLOT(onLeftBWLIterations(int)));

    QObject::connect(&_mary, SIGNAL(signalRightBWLIdealAngle(float)), &_max, SLOT(onRightBWLIdealAngle(float)));
    QObject::connect(&_mary, SIGNAL(signalRightBWLAllowedAngleVariance(float)), &_max, SLOT(onRightBWLAllowedAngleVariance(float)));
    QObject::connect(&_mary, SIGNAL(signalRightBWLMinVotes(int)), &_max, SLOT(onRightBWLMinVotes(int)));
    QObject::connect(&_mary, SIGNAL(signalRightBWLDistanceThreshold(float)), &_max, SLOT(onRightBWLDistanceThreshold(float)));
    QObject::connect(&_mary, SIGNAL(signalRightBWLIterations(int)), &_max, SLOT(onRightBWLIterations(int)));

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
