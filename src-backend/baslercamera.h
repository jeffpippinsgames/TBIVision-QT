#ifndef BASLERCAMERA_H
#define BASLERCAMERA_H

#include <QObject>
#include <QImage>
#include <pylon/ImageEventHandler.h>
#include <pylon/ConfigurationEventHandler.h>
#include <pylon/stdinclude.h>
#include <pylon/PylonImage.h>
#include <pylon/PylonIncludes.h>
#include <opencv2/opencv.hpp>

class BaslerCamera : public QObject, public Pylon::CImageEventHandler, public Pylon::CConfigurationEventHandler
{
    Q_OBJECT
public:
    explicit BaslerCamera(QObject *parent = nullptr);
    virtual ~BaslerCamera();

public:
    void Start();
    void Stop();
    void Dispose();
    bool isOpen();

signals:
    void NewMatFrameGrabbed(cv::Mat const& _matframe);
    void openedChanged(bool _state);

private:
    void OnImageGrabbed(Pylon::CInstantCamera& _camera, const Pylon::CGrabResultPtr& _ptrGrab);

private:
    Pylon::CInstantCamera *m_camera;
};

#endif // BASLERCAMERA_H
