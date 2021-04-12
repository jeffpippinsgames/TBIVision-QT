#include "toby.h"
#include <QDebug>
#include <QImage>

Toby::Toby(QObject *parent) : QObject(parent)
{
    m_camera_frame = new QImage();
    //Grab The First Camera
    try
    {
        m_camera1 = new CInstantCamera(CTlFactory::GetInstance().CreateFirstDevice());
        m_camera1->RegisterImageEventHandler(this, RegistrationMode_ReplaceAll, Cleanup_None);
        m_camera1->Open();
    }
    catch (const GenericException & e)
    {
        qDebug()<<"Error Instantiating CInstantCamera" + QString(e.GetDescription());
        m_camera1 = nullptr;
    }

}

Toby::~Toby()
{
    if(m_camera1)
    {
        if(m_camera1->IsOpen()) m_camera1->Close();
        m_camera1->DestroyDevice();
    }
}

void Toby::triggerCamera()
{
    if(!m_camera1) return;
    if(!m_camera1->IsOpen()) return;
    if(m_camera1->CanWaitForFrameTriggerReady())
    {
        m_camera1->WaitForFrameTriggerReady(200);
        m_camera1->ExecuteSoftwareTrigger();
    }
}



void Toby::OnImageGrabbed(CInstantCamera &camera, const CGrabResultPtr &ptrGrab)
{
    if(ptrGrab->GrabSucceeded())
    {
        CImageFormatConverter fc;
        fc.OutputPixelFormat = PixelType_Mono8;
        CPylonImage pylonImage;
        fc.Convert(pylonImage, ptrGrab);
        if (!pylonImage.IsValid()) {
           qDebug()<<"Error in Toby::OnImageGrabbed(). pylonImage Could Not Be Converted.";
            return;
        }
        QImage img = this->toQImage(&pylonImage);
        *m_camera_frame = img.copy(QRect(0,0,0,0));
        emit frameGrabbed(img);
    }

}

QImage Toby::toQImage(CPylonImage *pylonImage)
{
    int width = pylonImage->GetWidth();
    int height = pylonImage->GetHeight();
    void* buffer = (uchar*)pylonImage->GetBuffer();
    //int step = pylonImage->GetAllocatedBufferSize() / height;
    QImage img(static_cast<uchar*>(buffer), width, height, QImage::Format_Grayscale8);
    return img;
}


