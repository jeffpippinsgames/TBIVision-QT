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
#include "tbiclass_garybasetypes.h"
#include "tbiclass_serialportcontrollerreturntype.h"
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
    //Register The QmlTBIDisplay QML Type
    QmlTBIDisplay::declareQML();
    //Register TBI Weld Types
    TBIWeldType_Enumerator::declareQML();
    TBIWeld_ProcessingPipeLineReturnType::declareQML();
    //General Class Types
    SerialPortControllerReturnType::declareQML();

    //The QML Application engine----------------------------------
    QQmlApplicationEngine engine;

    //instantiated Objects of Application-------------------------
    Gary _gary; //MicroController Singleton
    Toby _toby; //Basler Camera Singleton
    Max _max; //Seam Tracking Singleton
    Mary _mary; //Settings Singleton

    //Singleton Hooks
    _mary.setTBICorePointers(&_max, &_toby, &_gary);
    _max.setGary(&_gary);

    //Connect The  Frame Grab Loop
    QObject::connect(&_toby, SIGNAL(newCVMatFrameGrabbed(const cv::Mat&)), &_max, SLOT(recieveNewCVMat(const cv::Mat&)));

    //Adding Property Contextes From the Backend to QML--------------------------------
    engine.rootContext()->setContextProperty("Gary", &_gary);
    engine.rootContext()->setContextProperty("Toby", &_toby);
    engine.rootContext()->setContextProperty("Max", &_max);
    engine.rootContext()->setContextProperty("Mary", &_mary);
    _max.setRootQMLContextProperties(engine);
    _toby.setRootQMLContextProperties(engine);
    _gary.setRootQMLContextProperties(engine);

    //Set Mary's Default Values... The TBI Core Pointers Must Be Set Prior To Calling a Load File.
    //The QML Property Contexts Must Be Set First.
    //REMEMBER TO RESET THE SAVE FILE WHEN YOU ADD SOMETHING THAT IS SAVED
    _mary.loadSettingsFromDefaultFile();

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
