#ifndef TOBY_H
#define TOBY_H

#include <QObject>
#include <QQuickItem>
#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>
#include <QImage>
#include "opencv4/opencv2/core.hpp"
#include <QString>
#include <QElapsedTimer>


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

    Q_PROPERTY(double pylon_exposure READ getCameraExposure NOTIFY pylonCameraExposureChanged)
    Q_PROPERTY(QString camera_fps READ getCameraFPS NOTIFY cameraFPSChanged)
public:
    explicit Toby(QObject *parent = nullptr);
    ~Toby();
    Q_INVOKABLE void triggerCamera();
    Q_INVOKABLE void startCamera();

    //Property Get Methods
    Q_INVOKABLE QString getCameraInfo();
    Q_INVOKABLE void setCameraAOIToMax();
    Q_INVOKABLE double getCameraExposure();
    Q_INVOKABLE void turnOffCamera();
    QString getCameraFPS(){return m_camera_fps;}


private:
    //Pylon Members
    CInstantCamera *m_camera;
    QElapsedTimer m_timer;
    QString m_camera_fps;

    //Methods
    virtual void OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrab); //Pylon Event Handler
    bool SetDefaultCameraSettings();

public slots:
    void onChangeCameraAOI(int _width, int _height);
    void onChangeCameraExposure(double _exposure);
    void onChangeCameraGain(int _gain);

signals:

    void signalMaryForCameraSettings();
    void newQImageFrameGrabbed(const QImage &qimageframe);
    void newCVMatFrameGrabbed(const cv::Mat &matframe);
    void cameraOpenedChanged(bool _isopen);
    void aboutToDestroy();
    void pylonCameraExposureChanged();
    void cameraFPSChanged();

};

inline double Toby::getCameraExposure()
{
    if(m_camera == nullptr) return 0.0;
    try
    {
        //Get Camera NodeMap
        GenApi::INodeMap& _nodemap = m_camera->GetNodeMap();
        return CFloatParameter(_nodemap, "ExposureTimeAbs").GetValue();
    }
    catch(const GenericException &e)
    {
        QString _error = e.GetDescription();
        qDebug() << "Toby:: Error in onChangeCameraGain: " + _error;
    }
    return 0.0;
}

#endif // TOBY_H
