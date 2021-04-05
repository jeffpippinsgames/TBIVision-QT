#include "baslercamera.h"
#include <opencv2/core/hal/interface.h>
#include <QDebug>

//------------------------------------------------------------------------------------------------------------
BaslerCamera::BaslerCamera(QObject *parent) : QObject(parent)
{
    m_camera = nullptr;
}
//------------------------------------------------------------------------------------------------------------
BaslerCamera::~BaslerCamera()
{
    Dispose();
}
//------------------------------------------------------------------------------------------------------------
void BaslerCamera::Start()
{
    try
    {
        m_camera = new Pylon::CInstantCamera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
        m_camera->RegisterConfiguration(new Pylon::CAcquireContinuousConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_None);
        m_camera->Open();
        m_camera->RegisterImageEventHandler(this, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);
        m_camera->StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByInstantCamera);
        emit openedChanged(m_camera->IsOpen());
    }
    catch(const Pylon::GenericException e)
    {
        qDebug() << "Pylon Camera Error:" << e.GetDescription() << Qt::endl;
        m_camera = nullptr;
        return;
    }
}
//------------------------------------------------------------------------------------------------------------
void BaslerCamera::Stop()
{
    Dispose();
}
//------------------------------------------------------------------------------------------------------------
void BaslerCamera::Dispose()
{
    if(m_camera == nullptr) return;

    if(m_camera->IsGrabbing())
    {
        m_camera->StopGrabbing();
        m_camera->DeregisterImageEventHandler(this);
    }
    if(m_camera->IsOpen())
    {
        m_camera->Close();
        emit openedChanged(m_camera->IsOpen());
    }
    m_camera = nullptr;
}
//------------------------------------------------------------------------------------------------------------
bool BaslerCamera::isOpen()
{
    if(m_camera == nullptr) return false;
    return m_camera->IsOpen();
}
//------------------------------------------------------------------------------------------------------------
void BaslerCamera::OnImageGrabbed(Pylon::CInstantCamera &_camera, const Pylon::CGrabResultPtr &_ptrGrab)
{
    if(_ptrGrab->GrabSucceeded())
    {
        Pylon::CPylonImage pylonimage;
        Pylon::CImageFormatConverter fc;
        fc.Convert(pylonimage, _ptrGrab);
        //cv::Mat _matframe = cv::Mat(_ptrGrab->GetHeight(), _ptrGrab->GetWidth(), cv::CV_8UC1, (uint8_t *)pylonimage.GetBuffer());
       // emit NewMatFrameGrabbed(_matframe);
    }
}

