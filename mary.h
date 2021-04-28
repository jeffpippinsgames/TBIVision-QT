#ifndef MARY_H
#define MARY_H

#include <QObject>
#include <QQuickItem>
#include <QFile>
#include <QString>
#include <QRect>
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include <QFile>


/*
 A Note About Settings File Layout:

 Settings are saved as bin files with the .tbi extension
 The Application will look in the /savefile subfolder
 The arrangment of the file is as follows.

 Mary Will Attempt to load the file default.tbi

 int m_cvblur
 int m_cvthresholdmin
 int m_cvthresholdmax
*/

/**************************************************************
Mary
Description:
  Mary is the Record Keeper.
  Mary Interacts with Everyone.


  A Class to Encapsulate the storage of settings and value.
  This class is in carge of file handling and
 **************************************************************/
class Mary : public QObject
{
    Q_OBJECT
    //Pylon Properties--------------------------------------------------------------------------
    Q_PROPERTY(int pylon_maxheight READ getCameraMaxHeight NOTIFY pylonCameraMaxHeightChanged)
    Q_PROPERTY(int pylon_maxwidth READ getCameraMaxWidth NOTIFY pylonCameraMaxWidthChanged)
    Q_PROPERTY(QString pylon_cameraname READ getCameraDeviceName NOTIFY pylonCameraNameChanged)
    Q_PROPERTY(int pylon_aoiwidth READ getCameraAOIWidth WRITE setCameraAOIWidth NOTIFY pylonCameraAOIWidthChanged)
    Q_PROPERTY(int pylon_aoiheight READ getCameraAOIHeight WRITE setCameraAOIHeight NOTIFY pylonCameraAOIHeightChanged)
    Q_PROPERTY(int pylon_aoiwidth READ getCameraAOIWidth WRITE setCameraAOIWidth NOTIFY pylonCameraAOIWidthChanged)
    Q_PROPERTY(double pylon_exposure READ getCameraExposure WRITE setCameraExposure NOTIFY pylonCameraExposureChanged)
    Q_PROPERTY(int pylon_gain READ getCameraGain WRITE setCameraGain NOTIFY pylonCameraGainChanged)
    //Open CV Properties------------------------------------------------------------------------
    Q_PROPERTY(int cv_blur READ getCVBlurValue WRITE setCVBlurValue NOTIFY cvBlurValueChanged)
    Q_PROPERTY(int cv_thresholdmin READ getCVThresholdMinValue WRITE setCVThresholdMinValue NOTIFY cvThresholdMinValueChanged)
    Q_PROPERTY(int cv_thresholdmax READ getCVThresholdMaxValue WRITE setCVThresholdMaxValue NOTIFY cvThresholdMaxValueChanged)
    //GUI Properties Changed--------------------------------------------------------------------
    Q_PROPERTY(bool showdebuginfo READ getShowDebugInfo WRITE setShowDebugInfo NOTIFY showDebugInfoChanged)


public:
    //Constructor and Destructor--------------------------------------------------------------
    explicit Mary(QObject *parent = nullptr);
    ~Mary();

    //General Useful Methohds-----------------------------------------------------------------
    void SetMaryDefaultValues();
    Q_INVOKABLE void updateCameraAOIToMarysSettings();
    Q_INVOKABLE void saveMaryToFile();
    Q_INVOKABLE void loadMaryFromFile();



    //Pylon Property Get Functions-------------------------------------------------------------
    int getCameraMaxHeight(){return m_pylon_maxcameraheight;}
    int getCameraMaxWidth(){return m_pylon_maxcamerawidth;}
    QString getCameraDeviceName(){return m_pylon_cameraname;}
    int getCameraAOIWidth(){return m_pylon_aoiwidth;}
    int getCameraAOIHeight(){return m_pylon_aoiheight;}
    double getCameraExposure(){return m_pylon_exposuretime;}
    int getCameraGain(){return m_pylon_gain;}
    //Open CV Property Get Functions-----------------------------------------------------------
    int getCVBlurValue(){return m_cv_blur;}
    int getCVThresholdMinValue(){return m_cv_thresholdmin;}
    int getCVThresholdMaxValue(){return m_cv_thresholdmax;}
    //GUI Property Get Functions--------------------------------------------------------------
    bool getShowDebugInfo(){return m_gui_showdebuginfo;}

    //Pylon Property Set Functions------------------------------------------------------------
    void setCameraAOIWidth(int _width);
    void setCameraAOIHeight(int _height);
    void setCameraExposure(double _exposure);
    void setCameraGain(int _gain);
    //OpenC V Property Set Functions----------------------------------------------------------
    void setCVBlurValue(int _value);
    void setCVThresholdMinValue(int _value);
    void setCVThresholdMaxValue(int _value);
    //GUI Property Set Functions--------------------------------------------------------------
    void setShowDebugInfo(bool _value);


signals:
    //General Signals-------------------------------------------------------------------------
    void aboutToDestroy();
    //Toby Related Signals;
    void signalChangeCameraAOI(int _width, int _height);
    void signalChangeCameraExposure(double _exposure);
    void signalChangeCameraGain(int _gain);
    //Pylon Related Signals-------------------------------------------------------------------
    void pylonCameraMaxHeightChanged();
    void pylonCameraMaxWidthChanged();
    void pylonCameraNameChanged();
    void pylonCameraAOIWidthChanged();
    void pylonCameraAOIHeightChanged();
    void pylonCameraExposureChanged();
    void pylonCameraGainChanged();
    //Open CV Related Signals-----------------------------------------------------------------
    void cvBlurValueChanged();
    void cvThresholdMinValueChanged();
    void cvThresholdMaxValueChanged();
    //GUI Related Signals---------------------------------------------------------------------
    void showDebugInfoChanged();

public slots:

private:
    //Pylon Settings and Stuff
    const int m_pylon_maxcamerawidth = 724;
    const int m_pylon_maxcameraheight = 542;
    const int m_pylon_minoffsetx = 0;
    const int m_pylon_minoffsety = 0;
    QString m_pylon_cameraname;
    int m_pylon_aoiwidth;
    int m_pylon_aoiheight;
    int m_pylon_aoioffsetx;
    int m_pylon_aoioffsety;
    double m_pylon_exposuretime; //Range From 22 to 100,000,000 in micro seconds.
    int m_pylon_gain; //Range 0 to 360

    //OpenCV Settings and Stuff
    int m_cv_blur;
    int m_cv_thresholdmin;
    int m_cv_thresholdmax;


    //GUI UI Settings and Stuff
    bool m_gui_showdebuginfo;
};

#endif // MARY_H
