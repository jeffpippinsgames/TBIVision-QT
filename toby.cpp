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
    m_timer.start();
    m_camera_fps = "Error";
    emit cameraFPSChanged();
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
        //m_camera->WaitForFrameTriggerReady(20);
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
        m_camera = new Pylon::CInstantCamera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
        //m_camera = new Pylon::CInstantCamera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
        m_camera->RegisterConfiguration(new Pylon::CSoftwareTriggerConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);
        m_camera->Open();
        m_camera->RegisterImageEventHandler(this, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);

        if(!SetDefaultCameraSettings())
        {
            turnOffCamera();
            return;
        }

        m_camera->StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByInstantCamera);
        this->triggerCamera();
        emit cameraOpenedChanged(m_camera->IsOpen());
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

void Toby::turnOffCamera()
{
    while(m_camera->IsGrabbing())
    {
        m_camera->StopGrabbing();
    }
    while(m_camera->IsOpen())
    {
        m_camera->Close();
    }
    while(m_camera->IsPylonDeviceAttached())
    {
        m_camera->DetachDevice();
    }
    m_camera->DestroyDevice();
    qDebug() << "Toby: Pylon Device Closed and Destroyed.";
}

/**************************************************************
OnImageGrabbed(CInstantCamera &camera, const CGrabResultPtr &ptrGrab)
Private
Description:
  Pylon Event Handler For The Camera Object
 **************************************************************/
void Toby::OnImageGrabbed(CInstantCamera &camera, const CGrabResultPtr &ptrGrab)
{
    m_camera_fps = QString("Camera FPS = " + QString::number(1000/m_timer.elapsed()));
    if(ptrGrab->GrabSucceeded())
    {
        m_timer.start();
        cv::Mat _mat = cv::Mat(ptrGrab->GetHeight(), ptrGrab->GetWidth(), CV_8UC1, (uint8_t *) ptrGrab->GetBuffer());
        emit cameraFPSChanged();
        emit newCVMatFrameGrabbed(_mat);
        return;
    }
    emit cameraFPSChanged();

}

/**************************************************************
CameraSettings()
Private
Description:
  There are a lot of camera settings.
 This Function Puts them all in one spot for ease of reading
 and Changing. It mirrors the Pylon Viewer App.
 **************************************************************/
bool Toby::SetDefaultCameraSettings()
{
    try
    {
        //Get Camera NodeMap
        GenApi::INodeMap& _nodemap = m_camera->GetNodeMap();
        //Analog Controls
        CEnumParameter(_nodemap, "GainAuto").SetValue("Off");
        CEnumParameter(_nodemap, "GainSelector").SetValue("All");
        CIntegerParameter(_nodemap, "GainRaw").SetValue(0); //Range 0 to 360
        CEnumParameter(_nodemap, "BlackLevelSelector").SetValue("All");
        CIntegerParameter(_nodemap, "BlackLevelRaw").SetValue(0); //Range 0 to 511
        CBooleanParameter(_nodemap, "GammaEnable").SetValue(false);
        CEnumParameter(_nodemap, "GammaSelector").SetValue("User");
        CFloatParameter(_nodemap, "Gamma").SetValue(0.0);
        CIntegerParameter(_nodemap, "DigitalShift").SetValue(0); //Range 0 to 4

        //Image Format Controls
        CEnumParameter(_nodemap, "PixelFormat").SetValue("Mono8");
        CBooleanParameter(_nodemap, "ReverseX").SetValue(false);
        CBooleanParameter(_nodemap, "ReverseY").SetValue(false);

        //AOI Controls
        CIntegerParameter(_nodemap, "Width").SetValue(720); //Range 0 to 728
        CIntegerParameter(_nodemap, "Height").SetValue(540); //Range 0 to 544
        CBooleanParameter(_nodemap, "CenterX").SetValue(true);
        CBooleanParameter(_nodemap, "CenterY").SetValue(true);
        CIntegerParameter(_nodemap, "BinningHorizontal").SetValue(1); //Set To 1
        CIntegerParameter(_nodemap, "BinningVertical").SetValue(1); //Set to 1
        CEnumParameter(_nodemap, "BinningHorizontalMode").SetValue("Average");
        CEnumParameter(_nodemap, "BinningVerticalMode").SetValue("Average");

        //Image Quality Control
        CEnumParameter(_nodemap, "PgiMode").SetValue("Off");

        //Acquisition Controls
        CIntegerParameter(_nodemap, "AcquisitionFrameCount").SetValue(1); //Set to 1
        CEnumParameter(_nodemap, "TriggerMode").SetValue("On");
        CEnumParameter(_nodemap, "TriggerSource").SetValue("Software");
        CEnumParameter(_nodemap, "TriggerActivation").SetValue("RisingEdge");
        //CFloatParameter(_nodemap, "TriggerDelay").SetValue(0.0); //Our Camera Does Not Have This Node.
        CEnumParameter(_nodemap, "ExposureMode").SetValue("Timed");
        CEnumParameter(_nodemap, "ExposureAuto").SetValue("Off");
        CEnumParameter(_nodemap, "ExposureTimeMode").SetValue("Standard");
        CFloatParameter(_nodemap, "ExposureTimeAbs").SetValue(3000.0);
        //CEnumParameter(_nodemap, "SensorShutterMode").SetValue("Global"); //Our Camera Does Not Have This Node.
        CBooleanParameter(_nodemap, "AcquisitionFrameRateEnable").SetValue(false);
        CFloatParameter(_nodemap, "AcquisitionFrameRateAbs").SetValue(100.0);
        CEnumParameter(_nodemap, "AcquisitionStatusSelector").SetValue("FrameTriggerWait");
        CBooleanParameter(_nodemap, "SyncFreeRunTimerEnable").SetValue(false);
        CIntegerParameter(_nodemap, "SyncFreeRunTimerStartTimeLow").SetValue(0); //Set to 0
        CIntegerParameter(_nodemap, "SyncFreeRunTimerStartTimeHigh").SetValue(0); //Set to 0
        CFloatParameter(_nodemap, "SyncFreeRunTimerTriggerRateAbs").SetValue(0.03);
    }
    catch(const GenericException &e)
    {
        QString _error = e.GetDescription();
        qDebug() << "Toby:: Error in SetDefaultCameraSettings: " + _error;
        return false;
    }
    return true;
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
        if(_oneshot)
        {
            _oneshot = false;
            m_camera->StopGrabbing();
        }
        QThread::msleep(100);
    }
    try
    {
        //Get Camera NodeMap
        GenApi::INodeMap& _nodemap = m_camera->GetNodeMap();
        CIntegerParameter(_nodemap, "Width").SetValue(_width); //Range 0 to 728
        CIntegerParameter(_nodemap, "Height").SetValue(_height); //Range 0 to 544
    }
    catch(const GenericException &e)
    {
        QString _error = e.GetDescription();
        qDebug() << "Toby:: Error in onChangeCameraGain: " + _error;
    }
    _oneshot = true;
    while(!m_camera->IsGrabbing())
    {
        if(_oneshot)
        {
            m_camera->StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByInstantCamera);
            _oneshot = false;
        }
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
    try
    {
        //Get Camera NodeMap
        GenApi::INodeMap& _nodemap = m_camera->GetNodeMap();
        CFloatParameter(_nodemap, "ExposureTimeAbs").SetValue((float)_exposure);
    }
    catch(const GenericException &e)
    {
        QString _error = e.GetDescription();
        qDebug() << "Toby:: Error in onChangeCameraExposure: " + _error;
    }

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
    try
    {
        //Get Camera NodeMap
        GenApi::INodeMap& _nodemap = m_camera->GetNodeMap();
        CIntegerParameter(_nodemap, "GainRaw").SetValue(_gain); //Range 0 to 360
    }
    catch(const GenericException &e)
    {
        QString _error = e.GetDescription();
        qDebug() << "Toby:: Error in onChangeCameraGain: " + _error;
    }

}


