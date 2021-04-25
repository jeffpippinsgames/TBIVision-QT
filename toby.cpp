#include "toby.h"
#include <QDebug>

/**************************************************************
Toby()
Public
Description:
  Toby Constructor
 **************************************************************/
Toby::Toby(QObject *parent) : QObject(parent)
{
    //Grab The First Camera
    m_camera1 = nullptr;
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
    if(m_camera1 != nullptr)
    {     
        if(m_camera1->IsOpen()) m_camera1->Close();
        m_camera1->DestroyDevice();
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
    if(m_camera1 == nullptr) return;
    if(!m_camera1->IsOpen()) return;
    if(m_camera1->CanWaitForFrameTriggerReady())
    {
        m_camera1->WaitForFrameTriggerReady(200);
        m_camera1->ExecuteSoftwareTrigger();
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
        m_camera1 = new Pylon::CInstantCamera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
        m_camera1->RegisterConfiguration(new Pylon::CSoftwareTriggerConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_None);
        m_camera1->Open();

        //Set Camera Settings
        //m_camera1->GetNodeMap().GetNode()
        m_camera1->RegisterImageEventHandler(this, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);
        m_camera1->StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByInstantCamera);
        this->triggerCamera();
        emit cameraOpenedChanged(m_camera1->IsOpen());

    }
    catch(const Pylon::GenericException e)
    {
        qDebug() << "Toby: Pylon Error - " << e.GetDescription();
        m_camera1 = nullptr;
        return;
    }
    qDebug() << "Toby: Pylon Camera Device Created and Opened.";
}


/**************************************************************
getCameraInfo()
Public, Q_INVOKABLE
Description:
  Returns The Model and Manufacture of the Camera
 **************************************************************/
QString Toby::getCameraInfo()
{
    if(m_camera1 != nullptr)
    {
        if(m_camera1->IsOpen())
        {
            CDeviceInfo _deviceinfo = m_camera1->GetDeviceInfo();
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


