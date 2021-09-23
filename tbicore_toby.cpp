#include "tbicore_toby.h"
#include <QDebug>
#include <QThread>
#include <QDir>
#include "tbicore_constants.h"

//Constructors and Deconstructors
/**************************************************************
Toby()
Public
Description:
  Toby Constructor
 **************************************************************/
Toby::Toby(QObject *parent) : QObject(parent)
{
    //Grab The First Camera
    m_framesgrabbed = 0;
    m_camera = nullptr;
    m_timer.start();
    m_camera_fps = "Error";
    m_processsing_debug_image = false;

    emit cameraFPSChanged(m_camera_fps);
    QObject::connect(&m_camera_params, SIGNAL(aoiWidthChanged()), this, SLOT(onChangeCameraAOI()));
    QObject::connect(&m_camera_params, SIGNAL(aoiHeightChanged()), this, SLOT(onChangeCameraAOI()));
    QObject::connect(&m_camera_params, SIGNAL(exposureChanged()), this, SLOT(onChangeCameraExposure()));
    QObject::connect(&m_camera_params, SIGNAL(gainChanged()), this, SLOT(onChangeCameraGain()));
    this->setDefautValues();
    initializeCamera();
    if(m_showdebug) qDebug() << "Toby::Toby() Toby Object Created.";
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
        if(m_camera->IsOpen())
        {
            m_camera->DeregisterImageEventHandler(this);
            m_camera->Close();
        }
        m_camera->DestroyDevice();
    }
    if(m_showdebug) qDebug() << "Toby::~Toby() Toby Object Destroyed.";
}

//Pylon Overloaded onImageGrabbed Method
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
        cv::Mat _mat = cv::Mat(ptrGrab->GetHeight(), ptrGrab->GetWidth(), CV_8UC1, (uint8_t *) ptrGrab->GetBuffer()).clone();
        emit newCVMatFrameGrabbed(_mat);
    }
    emit cameraFPSChanged(m_camera_fps);
}

//Get Methods
/**************************************************************
getCameraInfo()
Public, Q_INVOKABLE
Description:
  Returns The Model and Manufacture of the Camera
 **************************************************************/
QString Toby::getCameraInfo()
{
    if(!isCameraInitialized())
    {
        if(m_showdebug) qDebug() << "Toby::getCameraInfo() called without camera being initialized.";
        return QString("No Camera Initialized.");
    }
    CDeviceInfo _deviceinfo = m_camera->GetDeviceInfo();
    QString _model = QString(_deviceinfo.GetFriendlyName().c_str());
    return QString("Current Camera: " + _model);
}

//Camera Settings Related Slots
/**************************************************************
CameraSettings()
Private
Description:
  There are a lot of camera settings.
 This Function Puts them all in one spot for ease of reading
 and Changing. It mirrors the Pylon Viewer App.
 **************************************************************/
bool Toby::SetCameraSettingsacA800()
{
    if(!m_camera) return false;
    if(!m_camera->IsOpen()) return false;

    if(!isCameraInitialized())
    {
        if(m_showdebug) qDebug() << "Toby::SetCameraSettings() called without a camera being initialized.";
        return false;
    }

    try
    {

        //Get Camera NodeMap
        GenApi::INodeMap& _nodemap = m_camera->GetNodeMap();
        //Analog Controls
        CEnumParameter(_nodemap, "GainAuto").SetValue("Off");
        CEnumParameter(_nodemap, "GainSelector").SetValue("All");
        CIntegerParameter(_nodemap, "GainRaw").SetValue(136); //Range 0 to 360 for aca720; Range 136 to 542 for aca800
        CEnumParameter(_nodemap, "BlackLevelSelector").SetValue("All");
        CIntegerParameter(_nodemap, "BlackLevelRaw").SetValue(0); //Range 0 to 511 for aca720; Range 0 to 255 for aca800
        CBooleanParameter(_nodemap, "GammaEnable").SetValue(false);
        CEnumParameter(_nodemap, "GammaSelector").SetValue("User");
        CFloatParameter(_nodemap, "Gamma").SetValue(0.0);

        //Image Format Controls
        CEnumParameter(_nodemap, "PixelFormat").SetValue("Mono8");
        CBooleanParameter(_nodemap, "ReverseX").SetValue(false);
        CBooleanParameter(_nodemap, "ReverseY").SetValue(false);

        //AOI Controls
        CIntegerParameter(_nodemap, "Width").SetValue(800); //Range 0 to 728 For aca720; Range 0 to 816 for aca800
        CIntegerParameter(_nodemap, "Height").SetValue(600); //Range 0 to 544 for aca720; Range 0 to 616 for aca800
        CBooleanParameter(_nodemap, "CenterX").SetValue(true);
        CBooleanParameter(_nodemap, "CenterY").SetValue(true);
        CIntegerParameter(_nodemap, "BinningHorizontal").SetValue(1); //Set To 1
        CIntegerParameter(_nodemap, "BinningVertical").SetValue(1); //Set to 1
        CEnumParameter(_nodemap, "BinningHorizontalMode").SetValue("Average");
        CEnumParameter(_nodemap, "BinningVerticalMode").SetValue("Average");

        //Image Quality Control
        CEnumParameter(_nodemap, "PgiMode").SetValue("Off");

        //Exposure Control
        CEnumParameter(_nodemap, "ExposureMode").SetValue("Timed");
        CEnumParameter(_nodemap, "ExposureAuto").SetValue("Off");
        //CEnumParameter(_nodemap, "ExposureTimeMode").SetValue("Standard"); //The aca800 has only 1 Mode "timed.
        CFloatParameter(_nodemap, "ExposureTimeAbs").SetValue(1000.0);

        //SyncFreeRun Controls
        CBooleanParameter(_nodemap, "SyncFreeRunTimerEnable").SetValue(false);
        CIntegerParameter(_nodemap, "SyncFreeRunTimerStartTimeLow").SetValue(0); //Set to 0 Not Available for aca800
        CIntegerParameter(_nodemap, "SyncFreeRunTimerStartTimeHigh").SetValue(0); //Set to 0 Not Available for aca800
        CFloatParameter(_nodemap, "SyncFreeRunTimerTriggerRateAbs").SetValue(0.03);

        //Acquisition Controls
        CEnumParameter(_nodemap, "AcquisitionMode").SetValue("Continuous");
        CEnumParameter(_nodemap, "AcquisitionStatusSelector").SetValue("FrameTriggerWait");
        CIntegerParameter(_nodemap, "AcquisitionFrameCount").SetValue(1); //Set to 1
        CBooleanParameter(_nodemap, "AcquisitionFrameRateEnable").SetValue(false);
        CFloatParameter(_nodemap, "AcquisitionFrameRateAbs").SetValue(30);

        //Trigger Controls
        CEnumParameter(_nodemap, "TriggerMode").SetValue("On");
        CEnumParameter(_nodemap, "TriggerSource").SetValue("Software");
        CEnumParameter(_nodemap, "TriggerActivation").SetValue("RisingEdge");
    }
    catch(const GenericException &e)
    {
        QString _error = e.GetDescription();
        if(m_showdebug) qDebug() << "Toby:: Error in SetCameraSettingsacA800(): " + _error;
        return false;
    }

    if(m_showdebug) qDebug() << "Toby:: SetCameraSettingsacA800() finished. Camera Settings Have Been Set.";
    return true;
}

bool Toby::SetCameraSettingsacA720()
{
    if(!m_camera) return false;
    if(!m_camera->IsOpen()) return false;

    if(!isCameraInitialized())
    {
        if(m_showdebug) qDebug() << "Toby::SetCameraSettings() called without a camera being initialized.";
        return false;
    }

    try
    {

        //Get Camera NodeMap
        GenApi::INodeMap& _nodemap = m_camera->GetNodeMap();
        //Analog Controls
        CEnumParameter(_nodemap, "GainAuto").SetValue("Off");
        CEnumParameter(_nodemap, "GainSelector").SetValue("All");
        CIntegerParameter(_nodemap, "GainRaw").SetValue(0); //Range 0 to 360 for aca720; Range 136 to 542 for aca800
        CEnumParameter(_nodemap, "BlackLevelSelector").SetValue("All");
        CIntegerParameter(_nodemap, "BlackLevelRaw").SetValue(0); //Range 0 to 511 for aca720; Range 0 to 255 for aca800
        CBooleanParameter(_nodemap, "GammaEnable").SetValue(false);
        CEnumParameter(_nodemap, "GammaSelector").SetValue("User");
        CFloatParameter(_nodemap, "Gamma").SetValue(0.0);

        //Image Format Controls
        CEnumParameter(_nodemap, "PixelFormat").SetValue("Mono8");
        CBooleanParameter(_nodemap, "ReverseX").SetValue(false);
        CBooleanParameter(_nodemap, "ReverseY").SetValue(false);

        //AOI Controls
        CIntegerParameter(_nodemap, "Width").SetValue(720); //Range 0 to 728 For aca720; Range 0 to 816 for aca800
        CIntegerParameter(_nodemap, "Height").SetValue(544); //Range 0 to 544 for aca720; Range 0 to 616 for aca800
        CBooleanParameter(_nodemap, "CenterX").SetValue(true);
        CBooleanParameter(_nodemap, "CenterY").SetValue(true);
        CIntegerParameter(_nodemap, "BinningHorizontal").SetValue(1); //Set To 1
        CIntegerParameter(_nodemap, "BinningVertical").SetValue(1); //Set to 1
        CEnumParameter(_nodemap, "BinningHorizontalMode").SetValue("Average");
        CEnumParameter(_nodemap, "BinningVerticalMode").SetValue("Average");

        //Image Quality Control
        CEnumParameter(_nodemap, "PgiMode").SetValue("Off");

        //Exposure Control
        CEnumParameter(_nodemap, "ExposureMode").SetValue("Timed");
        CEnumParameter(_nodemap, "ExposureAuto").SetValue("Off");
        //CEnumParameter(_nodemap, "ExposureTimeMode").SetValue("Standard"); //The aca800 has only 1 Mode "timed.
        CFloatParameter(_nodemap, "ExposureTimeAbs").SetValue(1000.0);

        //SyncFreeRun Controls
        CBooleanParameter(_nodemap, "SyncFreeRunTimerEnable").SetValue(false);
        CIntegerParameter(_nodemap, "SyncFreeRunTimerStartTimeLow").SetValue(0); //Set to 0 Not Available for aca800
        CIntegerParameter(_nodemap, "SyncFreeRunTimerStartTimeHigh").SetValue(0); //Set to 0 Not Available for aca800
        CFloatParameter(_nodemap, "SyncFreeRunTimerTriggerRateAbs").SetValue(0.03);

        //Acquisition Controls
        CEnumParameter(_nodemap, "AcquisitionMode").SetValue("Continuous");
        CEnumParameter(_nodemap, "AcquisitionStatusSelector").SetValue("FrameTriggerWait");
        CIntegerParameter(_nodemap, "AcquisitionFrameCount").SetValue(1); //Set to 1
        CBooleanParameter(_nodemap, "AcquisitionFrameRateEnable").SetValue(false);
        CFloatParameter(_nodemap, "AcquisitionFrameRateAbs").SetValue(30);

        //Trigger Controls
        CEnumParameter(_nodemap, "TriggerMode").SetValue("On");
        CEnumParameter(_nodemap, "TriggerSource").SetValue("Software");
        CEnumParameter(_nodemap, "TriggerActivation").SetValue("RisingEdge");
    }
    catch(const GenericException &e)
    {
        QString _error = e.GetDescription();
        if(m_showdebug) qDebug() << "Toby:: Error in SetCameraSettingsacA720(): " + _error;
        return false;
    }

    if(m_showdebug) qDebug() << "Toby:: SetCameraSettingsacA720() finished. Camera Settings Have Been Set.";
    return true;
}

/**************************************************************
onChangeCameraAOI(int _width, int _height, int _offsetx, int _offsety)
Slot
Description:
  Signal Handler For Changing the Camera AOI
 **************************************************************/
void Toby::onChangeCameraAOI()
{
    /*
    if(!isCameraInitialized())
    {
        qDebug() << "Toby::onChangeCameraAOI() called without camera being initialized.";
        return;
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

*/
}

/**************************************************************
onChangeCameraExposure(double _exposure)
Slot
Description:
  Signal Handler For Changing the Camera Exposure
 **************************************************************/
void Toby::onChangeCameraExposure()
{

    if(!isCameraInitialized())
    {
        if(m_showdebug) qDebug() << "Toby::onChangeCameraExposure() called without camera being initialized.";
        return;
    }

    try
    {
        //Get Camera NodeMap
        GenApi::INodeMap& _nodemap = m_camera->GetNodeMap();
        CFloatParameter(_nodemap, "ExposureTimeAbs").SetValue(m_camera_params.getExposure());
    }
    catch(const GenericException &e)
    {
        QString _error = e.GetDescription();
        if(m_showdebug) qDebug() << "Toby:: Error in onChangeCameraExposure: " + _error;
    }

}

/**************************************************************
onChangeCameraGain(int _gain)
Slot
Description:
  Signal Handler For Changing the Camera Gain
 **************************************************************/
void Toby::onChangeCameraGain()
{
    /*

    if(!isCameraInitialized())
    {
        if(m_showdebug) qDebug() << "Toby::onChangeCameraGain() called without camera being initialized.";
        return;
    }

    try
    {
        //Get Camera NodeMap
        //GenApi::INodeMap& _nodemap = m_camera->GetNodeMap();
//        /CIntegerParameter(_nodemap, "GainRaw").SetValue(_gain); //Range 0 to 360
        //CIntegerParameter(_nodemap, "GainRaw").SetValue(136);
    }
    catch(const GenericException &e)
    {
        //QString _error = e.GetDescription();
        //if(m_showdebug) qDebug() << "Toby:: Error in onChangeCameraGain: " + _error;
    }
*/
}

//Still Image Processing Functions
void Toby::openStillImagetoProcess(QString _filename)
{

    //Have to do a bunch of funky string conversions for cv::imread()
    QString __filename = getCurrentApplicationPath() + "/stills/" + _filename;
    QByteArray __filenamearray = __filename.toLocal8Bit();
    char *__filenamebuffer = __filenamearray.data();
    cv::String ___filename(__filenamebuffer);

    cv::Mat _mat = cv::imread(___filename);
    cv::Mat _newmat;

    int _width = _mat.cols;
    int _height  = _mat.rows;

    //Resize the Image if its too Big
    if((_width > 720) || (_height > 540))
    {
        float  _wscale = (float)720/_width;
        float _hscale = (float)540/_height;
        float _scale;
        if(_wscale < _hscale)
        {
            _scale = _wscale;
        }
        else
        {
            _scale = _hscale;
        }
        cv::resize(_mat, _newmat, cv::Size(_width*_scale,_height*_scale), 0.0,0.0, cv::INTER_AREA);
    }
    else
    {
        _newmat = _mat.clone();
    }
    //Make Sure Image is right size


    //Convert. The Processing Pipeline requires a CV_8UC1 Mat
    switch(_newmat.type())
    {
    case CV_8UC3:
        cv::cvtColor(_newmat, m_still_debug_image, cv::COLOR_BGR2GRAY, 1);
        m_processsing_debug_image = true;
        emit emitProcessingStillImageChanged();
        break;
    case CV_8UC1:
        m_still_debug_image = _newmat.clone();
        m_processsing_debug_image = true;
        emit emitProcessingStillImageChanged();
        break;
    default:
        if(m_showdebug) qDebug() << "Toby: openStillImagetoProcess() cannot process the image file " << _filename;
        return;
        break;

    }


   if(m_showdebug) qDebug() << "Toby: openStillImagetoProcess() Called. m_processing_debug_image = true. Now Processsing " << _filename;

}

void Toby::closeStillImagetoProcess()
{
    if(m_processsing_debug_image)
    {
        m_processsing_debug_image = false;
        emit emitProcessingStillImageChanged();
        if(m_showdebug) qDebug() << "Toby: closeStillImagetoProcess() Called. m_processing_debug_image = false";
    }
}

void Toby::updateStillImageFileList()
{

    QDir _dir(qApp->applicationDirPath() + "/stills");
    if(!_dir.exists())
    {
        if(m_showdebug) qDebug() << "Toby: Error in updateDtillImageFileList(). the /stills directory does not exsist.";
        return;
    }
    QStringList _filenamefilters;
    _filenamefilters << "*.png";
    _dir.setNameFilters(_filenamefilters);
    m_still_file_names.empty();
    m_still_file_names = _dir.entryList(_filenamefilters, QDir::Files);
    emit onStillImageFileModelChanged();
}

void Toby::setRootQMLContextProperties(QQmlApplicationEngine &_engine)
{
    _engine.rootContext()->setContextProperty("PylonCameraParameters", &m_camera_params);
}

//Camera Control and Interation Methods
/**************************************************************
initializeCamera()
Private
Description:
  Created an Instance of the CInstantCamera Object.
  Sets The Configuration and Opens the Camera.
 **************************************************************/
bool Toby::initializeCamera()
{
    if(!isCameraInitialized())
    {
        try
        {
            m_camera = new Pylon::CInstantCamera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
            m_camera->Open();
            CDeviceInfo _deviceinfo = m_camera->GetDeviceInfo();
            m_camera_params.setCameraName(QString(_deviceinfo.GetFriendlyName().c_str()));
            if(m_showdebug) qDebug() << m_camera_params.getCameraName() << " has been opened.";
            if(m_camera_params.getCameraName().contains("acA800", Qt::CaseInsensitive))
            {
                SetCameraSettingsacA800();
                if(m_showdebug) qDebug() << "Settings Have Been Applied to the Basler acA800-200gm GigE Camera.";
            }
            if(m_camera_params.getCameraName().contains("acA720", Qt::CaseInsensitive))
            {
                SetCameraSettingsacA720();
                if(m_showdebug) qDebug() << "Settings Have Been Applied to the Basler acA720-280gm GigE Camera.";
            }
            m_camera->RegisterImageEventHandler(this, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);
            //m_camera->RegisterConfiguration(new Pylon::CAcquireContinuousConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);
            //m_camera->RegisterConfiguration(new Pylon::CSoftwareTriggerConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);
            m_camera_params.setConnection(true);


            if(m_showdebug) qDebug()<<"Toby::initializeCamera(): Camera Opened and Configured: ";
        }
        catch(const Pylon::GenericException e)
        {
            m_camera_params.setConnection(false);
            if(m_showdebug) qDebug()<<"Toby::initializeCamera() Pylon Error: " << e.GetDescription();
            return false;
        }
        emit cameraInitialized();
        if(m_showdebug) qDebug()<<"Toby::initializeCamera(): Camera Initialization Complete: ";
        return true;
    }
    else
    {
        if(m_showdebug) qDebug() << "Toby::initializeCamera() called without camera being initialized.";
        return false;
    }
    return true;
}

/**************************************************************
startCamera()
Public, Q_INVOKABLE
Description:
  Starts the camera grabbing images
 **************************************************************/
void Toby::startCamera()
{
    if(m_showdebug) qDebug() << "startCamera() Called";
    if(!isCameraInitialized())
    {
        if(m_showdebug) qDebug() << "Toby::startCamera() called without camera being initialized";
        return;
    }


    if(m_camera->IsGrabbing())
    {
        if(m_showdebug) qDebug() << "Toby::startCamera() called with camera already grabbing";
        return;
    }
    try
    {
        m_camera->StartGrabbing(GrabStrategy_LatestImageOnly, GrabLoop_ProvidedByInstantCamera);
        this->triggerNextFrame();
    }
    catch(const Pylon::GenericException e)
    {
        if(m_showdebug) qDebug() << "Toby::startCamera(): Pylon Error: FFFF" << e.GetDescription();
        m_camera = nullptr;
        return;
    }
    if(m_showdebug) qDebug() << "Toby::startCamera(): Camera has been turned on.";
    emit cameraTurnedOn();
    emit emitProcessingCameraChanged();

}

/**************************************************************
stopCamera()
Public, Q_INVOKABLE
Description:
  Stops the Camera From Grabbing Images.
 **************************************************************/
void Toby::stopCamera()
{

    if(!isCameraInitialized())
    {
        if(m_showdebug) qDebug() << "Toby: turnOffCamera() called without the camera being initialized.";
        return;
    }

    if(!isCameraOn())
    {
        if(m_showdebug) qDebug() << "Toby: turnOffCamera() called. Camera is already off.";
        return;
    }


    while(m_camera->IsGrabbing())
    {
        m_camera->StopGrabbing();
    }
    if(m_showdebug) qDebug() << "Toby: turnOffCamera(). Camera has been turned off.";
    emit cameraTurnedOff();
    emit emitProcessingCameraChanged();
}

/**************************************************************
isCameraInitialized()
Private
Description:
  Checks if the Camera object is instantiated,
  connected to Pylon and open.
 **************************************************************/
bool Toby::isCameraInitialized()
{
    if(m_camera == nullptr) return false;
    if(!m_camera->IsPylonDeviceAttached()) return false;
    if(!m_camera->IsOpen()) return false;
    return true;
}

/**************************************************************
isCameraOn()
Private
Description:
  Determines if the Camera is Grabbing Images.
 **************************************************************/
bool Toby::isCameraOn()
{
    if(!isCameraInitialized()) return false;
    if(!m_camera->IsOpen()) return false;
    return true;
}

/**************************************************************
triggerCamera()
Public, Q_INVOKABLE
Description:
  Instructs Toby to Send a Software Trigger to the Camera
 **************************************************************/
void Toby::triggerNextFrame()
{

    //Process the Still Image if the debug image is not null
    if(m_processsing_debug_image)
    {

        m_camera_fps = QString("Still Image FPS = " + QString::number(1000/m_timer.elapsed()));
        m_timer.start();
        emit cameraFPSChanged(m_camera_fps);
        emit newCVMatFrameGrabbed(m_still_debug_image.clone());
        return;
    }
    //Process Camera Trigger.
    else
    {
        //We Are Not Processing Still Images. Go On To Camera Processing

        if(!m_camera->IsOpen())
        {
            if(m_showdebug) qDebug() << "Toby: triggerCamera() called without camera being opened.";
            return;
        }
        if(!m_camera->IsGrabbing())
        {
            if(m_showdebug) qDebug() << "Toby: triggerCamera() called while camera was not grabbing.";
            return;
        }
        if (m_camera->WaitForFrameTriggerReady( 1000, TimeoutHandling_ThrowException ))
        {

           m_camera->ExecuteSoftwareTrigger();
           if(m_showdebug) qDebug() << "Camera Software Trigger Tripped";
        }
    }
}

/**************************************************************
setCameraAOIToMax()
Public, Q_INVOKABLE
Description:
  Sets The Camera AOI to Maximum and the Offsets to 0
 **************************************************************/
void Toby::setCameraAOIToMax()
{
    m_camera_params.setAOIHeight(TBIConstants::Max_Camera_Height);
    m_camera_params.setAOIWidth(TBIConstants::Max_Camera_Width);
}

void Toby::setDefautValues()
{
    m_camera_params.setDefautValues();
}

void Toby::saveToFile(QDataStream &_filedatastream)
{
    m_camera_params.saveToFile(_filedatastream);
}

void Toby::loadFromFile(QDataStream &_filedatastream)
{
    m_camera_params.loadFromFile(_filedatastream);
}
