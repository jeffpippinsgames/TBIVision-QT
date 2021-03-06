#ifndef TBICORE_TOBY_H
#define TBICORE_TOBY_H

#include <QObject>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>
#include <QImage>
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include "opencv4/opencv2/imgcodecs/imgcodecs.hpp"
#include <QString>
#include <QElapsedTimer>
#include <QStringList>
#include "tbiparameterclass_pyloncameraparams.h"


using namespace Pylon;

/**************************************************************
Toby

Toby is the Photographer.
Toby is in charge of handling the Camera.

Description:
  toby is a class that facilitates all of the machine vision
  camera functionality. It encapulates from Basler's Pylon SDK
  toby is written for Pylon 6.2.0
  At this point toby will only manage 1 camera.
 **************************************************************/
class Toby : public QObject, public Pylon::CImageEventHandler, public Pylon::CConfigurationEventHandler
{

    Q_OBJECT

    Q_PROPERTY(QString camera_fps READ getCameraFPS NOTIFY cameraFPSChanged)
    Q_PROPERTY(bool processingStillImage READ getProcessingStillImage NOTIFY emitProcessingStillImageChanged)
    Q_PROPERTY(bool processingCamera READ getProcessingCamera NOTIFY emitProcessingCameraChanged)
    Q_PROPERTY(QStringList stillImageListViewModel MEMBER m_still_file_names NOTIFY onStillImageFileModelChanged)
    Q_PROPERTY(QString currentApplicationPath READ getCurrentApplicationPath)


public:
    explicit Toby(QObject *parent = nullptr);
    ~Toby();

    //Public Camera Control and Interagation Methods
    Q_INVOKABLE bool initializeCamera();
    Q_INVOKABLE void startCamera();
    Q_INVOKABLE void stopCamera();
    Q_INVOKABLE bool isCameraOn();
    Q_INVOKABLE void triggerNextFrame();
    Q_INVOKABLE bool isCameraInitialized();

    //Public Still Image Processing
    Q_INVOKABLE void openStillImagetoProcess(QString _filename);
    Q_INVOKABLE void closeStillImagetoProcess();
    Q_INVOKABLE void updateStillImageFileList();
    Q_INVOKABLE QString getCurrentApplicationPath(){return qApp->applicationDirPath();}
    bool getProcessingStillImage(){return m_processsing_debug_image;}
    bool getProcessingCamera(){return m_camera->IsGrabbing();}
    void setRootQMLContextProperties(QQmlApplicationEngine &_engine);


    //Property Get Methods
    Q_INVOKABLE QString getCameraInfo();
    Q_INVOKABLE void setCameraAOIToMax();
    QString getCameraFPS(){return m_camera_fps;}

    //Save File Methods
    void setDefautValues();
    void saveToFile(QDataStream &_filedatastream);
    void loadFromFile(QDataStream &_filedatastream);

private:
    //Still Image Data Member
    const bool m_showdebug = false;
    cv::Mat m_still_debug_image;
    QStringList m_still_file_names;
    bool m_processsing_debug_image;

    TBIPylonCameraParamers m_camera_params;

    //Pylon Members
    CInstantCamera *m_camera;
    QElapsedTimer m_timer;
    QString m_camera_fps;
    int m_framesgrabbed;
    static const int Camera_Frames_To_Average = 3;
    cv::Mat m_grabbedmats[Camera_Frames_To_Average];


    //Methods
    virtual void OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrab); //Pylon Event Handler
    bool SetCameraSettingsacA800();
    bool SetCameraSettingsacA720();

public slots:
    //Camera Settings Slots
    void onChangeCameraAOI();
    void onChangeCameraExposure();
    void onChangeCameraGain();


signals:
    void emitProcessingStillImageChanged();
    void emitProcessingCameraChanged();
    void onStillImageFileModelChanged();
    void newQImageFrameGrabbed(const QImage &qimageframe);
    void newCVMatFrameGrabbed(const cv::Mat &matframe);
    void aboutToDestroy();
    void cameraFPSChanged(QString _fps);
    void cameraInitialized();
    void cameraTurnedOff();
    void cameraTurnedOn();

};


#endif // TBICORE_TOBY_H
