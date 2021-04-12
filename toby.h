#ifndef TOBY_H
#define TOBY_H

#include <QObject>
#include <pylon/PylonIncludes.h>


using namespace Pylon;

/**************************************************************
toby
Description:
  toby is a class that facilitates all of the machine vision
  camera functionality. It encapulates from Basler's Pylon SDK
  toby is written for Pylon 6.2.0
  At this point toby will only manage 1 camera.
 **************************************************************/
class Toby : public QObject, public Pylon::CImageEventHandler, public Pylon::CConfigurationEventHandler
{
    Q_OBJECT
    //Q_PROPERTY(QImage* cameraframe READ getCameraFrame)

public:
    explicit Toby(QObject *parent = nullptr);
    ~Toby();
    Q_INVOKABLE void triggerCamera();
    QImage* getCameraFrame(){return m_camera_frame;}

private:

    //Pylon Members
    CInstantCamera* m_camera1;
    QImage* m_camera_frame;

    // from Pylon::CImageEventHandler
    virtual void OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrab);
     QImage toQImage(Pylon::CPylonImage* pylonImage);

signals:
    void frameGrabbed(QImage frame);

};

#endif // TOBY_H
