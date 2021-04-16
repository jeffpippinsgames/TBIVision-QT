#ifndef TOBY_H
#define TOBY_H

#include <QObject>
#include <QQuickItem>
#include <pylon/PylonIncludes.h>
#include <QImage>
#include "opencv4/opencv2/core.hpp"


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


public:
    explicit Toby(QObject *parent = nullptr);
    ~Toby();
    Q_INVOKABLE void triggerCamera();
    Q_INVOKABLE void startCamera();
    Q_INVOKABLE Toby* getThisToby(){return this;}

private:

    //Pylon Members
    CInstantCamera* m_camera1;

    //Methods
    virtual void OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrab); //Pylon Event Handler

public slots:

signals:
    void newQImageFrameGrabbed(const QImage &qimageframe);
    void newCVMatFrameGrabbed(const cv::Mat &matframe);
    void cameraOpenedChanged(bool _isopen);
    void aboutToDestroy();

};

#endif // TOBY_H
