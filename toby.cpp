#include "toby.h"
#include <QDebug>
#include <QThread>


/**************************************************************
Toby()
Public
Description:
  Toby Constructor
 **************************************************************/
Toby::Toby(QObject *parent) : QObject(parent)
{
    //Grab The First Camera
    m_camera = nullptr;
    qDebug() << "Toby: Toby Object Created.";
}

/**************************************************************
~Toby()
Public
Description:
  Toby Destructor
 **************************************************************/
Toby::~Toby()
{
    emit this->aboutToDestroy();
    if(m_camera != nullptr)
    {
        if(m_camera->IsOpen()) m_camera->Close();
        m_camera->DestroyDevice();
    }
    qDebug() << "Toby: Toby Object Destroyed.";
}

/**************************************************************
triggerCamera()
Public, Q_INVOKABLE
Description:
  Instructs Toby to Send a Software Trigger to the Camera
 **************************************************************/
void Toby::triggerCamera()
{
    if(m_camera == nullptr) return;
    if(!m_camera->IsOpen()) return;
    if(!m_camera->IsGrabbing()) return;
    if(m_camera->CanWaitForFrameTriggerReady())
    {
        m_camera->WaitForFrameTriggerReady(2000);
        m_camera->ExecuteSoftwareTrigger();
    }
}

/**************************************************************
startCamera()
Public, Q_INVOKABLE
Description:
  Sets up the camera object within the Pylon Runtime
  and starts grabbing.
 **************************************************************/
void Toby::startCamera()
{
    try
    {
        m_camera = new Pylon::CBaslerUniversalInstantCamera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
        //m_camera = new Pylon::CInstantCamera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
        m_camera->RegisterConfiguration(new Pylon::CSoftwareTriggerConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_None);
        m_camera->Open();
        m_camera->RegisterImageEventHandler(this, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);
        SetDefaultCameraSettings();
        m_camera->StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByInstantCamera);
        this->triggerCamera();
        emit cameraOpenedChanged(m_camera->IsOpen());
        qDebug() << "Toby: Pylon Camera Device Created and Opened.";
        qDebug() << "Toby: Max Camera Height = " << m_camera->Height.GetMax();
        qDebug() << "Toby: Min Camera Height = " << m_camera->Height.GetMin();
        qDebug() << "Toby: Max Camera Width = " << m_camera->Width.GetMax();
        qDebug() << "Toby: Min Camera Width = " << m_camera->Width.GetMin();
        qDebug() << "Toby: Camera CenterX/Y = " << m_camera->CenterX.GetValue();
    }
    catch(const Pylon::GenericException e)
    {
        qDebug() << "Toby: Pylon Error - " << e.GetDescription();
        m_camera = nullptr;
        return;
    }


}

/**************************************************************
getCameraInfo()
Public, Q_INVOKABLE
Description:
  Returns The Model and Manufacture of the Camera
 **************************************************************/
QString Toby::getCameraInfo()
{
    if(m_camera != nullptr)
    {
        if(m_camera->IsOpen())
        {
            CDeviceInfo _deviceinfo = m_camera->GetDeviceInfo();
            QString _model = QString(_deviceinfo.GetFriendlyName().c_str());
            return QString("Current Camera: " + _model);
        }
        else
        {
            return QString("There is No Camera Running");
        }
    }
    return QString("There is No Camera Running");
}

/**************************************************************
setCameraAOIToMax()
Public, Q_INVOKABLE
Description:
  Sets The Camera AOI to Maximum and the Offsets to 0
 **************************************************************/
void Toby::setCameraAOIToMax()
{
    onChangeCameraAOI(720, 540);
}

/**************************************************************
OnImageGrabbed(CInstantCamera &camera, const CGrabResultPtr &ptrGrab)
Private
Description:
  Pylon Event Handler For The Camera Object
 **************************************************************/
void Toby::OnImageGrabbed(CInstantCamera &camera, const CGrabResultPtr &ptrGrab)
{

    if(ptrGrab->GrabSucceeded())
    {

        cv::Mat _mat = cv::Mat(ptrGrab->GetHeight(), ptrGrab->GetWidth(), CV_8UC1, (uint8_t *) ptrGrab->GetBuffer());
        emit newCVMatFrameGrabbed(_mat);
    }

}

/**************************************************************
CameraSettings()
Private
Description:
  There are a lot of camera settings.
 This Function Puts them all in one spot for ease of reading
 and Changing. It mirrors the Pylon Viewer App.
 **************************************************************/
void Toby::SetDefaultCameraSettings()
{
    //Analog Controls
    m_camera->GainAuto.SetValue("Off");
    m_camera->GainSelector.SetValue("All");
    m_camera->GainRaw.SetValue(0); //Range 0 to 360
    m_camera->BlackLevelSelector.SetValue("All");
    m_camera->BlackLevelRaw.SetValue(0); //Range 0 to 511
    m_camera->GammaEnable.SetValue(false);
    m_camera->GammaSelector.SetValue("User");
    m_camera->Gamma.SetValue(0.0); //Range 0.0 to 4.0
    m_camera->DigitalShift.SetValue(0); //Range 0 to 4

    //Image Format Controls
    m_camera->PixelFormat.SetValue("Mono8");
    m_camera->ReverseX.SetValue(false);
    m_camera->ReverseY.SetValue(false);

    //AOI Controls
    m_camera->Width.SetValue(720);
    m_camera->Height.SetValue(540);
    m_camera->CenterX.SetValue(true);
    m_camera->CenterY.SetValue(true);
    m_camera->BinningHorizontal.SetValue(1);
    m_camera->BinningVertical.SetValue(1);
    m_camera->BinningHorizontalMode.SetValue("Average");
    m_camera->BinningVerticalMode.SetValue("Average");

    //Image Quality Control
    m_camera->PgiMode.SetValue("Off");

    //Acquisition Controls
    m_camera->AcquisitionFrameCount.SetValue(1);
    m_camera->TriggerMode.SetValue("On");
    m_camera->TriggerSource.SetValue("Software");
    m_camera->TriggerActivation.SetValue("RisingEdge");
    m_camera->TriggerDelayAbs.SetValue(0.0);
    m_camera->ExposureMode.SetValue("Timed");
    m_camera->ExposureAuto.SetValue("Off");
    m_camera->ExposureTimeMode.SetValue("Standard");
    m_camera->ExposureTimeAbs.SetValue(3000.0); //Range From 22 to 100,000,000 in micro seconds.
    m_camera->ShutterMode.SetValue("Global");
    m_camera->AcquisitionFrameRateEnable.SetValue(false);
    m_camera->AcquisitionFrameRateAbs.SetValue(100.0);
    m_camera->AcquisitionStatusSelector.SetValue("FrameTriggerWait");
    m_camera->SyncFreeRunTimerEnable.SetValue(false);
    m_camera->SyncFreeRunTimerStartTimeLow.SetValue(0);
    m_camera->SyncFreeRunTimerStartTimeHigh.SetValue(0);
    m_camera->SyncFreeRunTimerTriggerRateAbs.SetValue(0.03);
}

/**************************************************************
onChangeCameraAOI(int _width, int _height, int _offsetx, int _offsety)
Slot
Description:
  Signal Handler For Changing the Camera AOI
 **************************************************************/
void Toby::onChangeCameraAOI(int _width, int _height)
{
    bool _oneshot = true;
    if(m_camera == nullptr) return;
    if(!m_camera->IsOpen()) return;
    while(m_camera->IsGrabbing())
    {
        if(_oneshot) m_camera->StopGrabbing();
        QThread::msleep(100);
    }
    _oneshot = true;
    m_camera->Width.SetValue(_width);
    m_camera->Height.SetValue(_height);

    while(!m_camera->IsGrabbing())
    {
        if(_oneshot) m_camera->StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByInstantCamera);
        QThread::msleep(100);
    }

    triggerCamera();
}

/**************************************************************
onChangeCameraExposure(double _exposure)
Slot
Description:
  Signal Handler For Changing the Camera Exposure
 **************************************************************/
void Toby::onChangeCameraExposure(double _exposure)
{
    if(m_camera == nullptr) return;
    if(!m_camera->IsOpen()) return;
    m_camera->ExposureTimeAbs.SetValue(_exposure);
}


/**************************************************************
onChangeCameraGain(int _gain)
Slot
Description:
  Signal Handler For Changing the Camera Gain
 **************************************************************/
void Toby::onChangeCameraGain(int _gain)
{
    if(m_camera == nullptr) return;
    if(!m_camera->IsOpen()) return;
    m_camera->GainRaw.SetValue(_gain);
}


