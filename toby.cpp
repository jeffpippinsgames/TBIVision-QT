#include "toby.h"
#include <QDebug>

Toby::Toby(QObject *parent) : QObject(parent)
{
    //Grab The First Camera
    m_camera1 = nullptr;
    qDebug() << "Toby Object Created." << Qt::endl;
}

Toby::~Toby()
{
    if(m_camera1 != nullptr)
    {
        if(m_camera1->IsOpen()) m_camera1->Close();
        m_camera1->DestroyDevice();
    }
    qDebug() << "Toby Onject Destroyed." << Qt::endl;
}

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

void Toby::startCamera()
{
    try
    {
        m_camera1 = new Pylon::CInstantCamera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
        m_camera1->RegisterConfiguration(new Pylon::CSoftwareTriggerConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_None);
        m_camera1->Open();
        m_camera1->RegisterImageEventHandler(this, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);
        m_camera1->StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByInstantCamera);
        this->triggerCamera();
        emit cameraOpenedChanged(m_camera1->IsOpen());

    }
    catch(const Pylon::GenericException e)
    {
        qDebug() << "Pylon Camera Error:" << e.GetDescription() << Qt::endl;
        m_camera1 = nullptr;
        return;
    }
    qDebug() << "Toby Initialized Pylon Successfully. Pylon Camera Device Created and Opened." << Qt::endl;
}

void Toby::OnImageGrabbed(CInstantCamera &camera, const CGrabResultPtr &ptrGrab)
{


    if(ptrGrab->GrabSucceeded())
    {
        /*
        CImageFormatConverter fc;
        fc.OutputPixelFormat = PixelType_Mono8;
        CPylonImage pylonImage;
        fc.Convert(pylonImage, ptrGrab);
        if (!pylonImage.IsValid()) {
           qDebug()<<"Error in Toby::OnImageGrabbed(). pylonImage Could Not Be Converted.";
            return;
        }
        QImage img = this->toQImage(&pylonImage);
        emit frameGrabbed(img);
        */

        QImage _img = QImage(static_cast<uchar*>(ptrGrab->GetBuffer()), ptrGrab->GetWidth(), ptrGrab->GetHeight(), QImage::Format_Grayscale8);
        emit newFrameGrabbed(_img);
    }

}

/*
QImage Toby::toQImage(CPylonImage *pylonImage)
{
    int width = pylonImage->GetWidth();
    int height = pylonImage->GetHeight();
    void* buffer = (uchar*)pylonImage->GetBuffer();
    //int step = pylonImage->GetAllocatedBufferSize() / height;
    QImage img(static_cast<uchar*>(buffer), width, height, QImage::Format_Grayscale8);
    return img;


}
*/

