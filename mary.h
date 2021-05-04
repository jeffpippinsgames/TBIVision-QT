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
    Q_PROPERTY(double pylon_exposure READ getCameraExposure WRITE setCameraExposure NOTIFY pylonCameraExposureChanged)
    Q_PROPERTY(int pylon_gain READ getCameraGain WRITE setCameraGain NOTIFY pylonCameraGainChanged)
    //Open CV Properties------------------------------------------------------------------------
    Q_PROPERTY(int cv_blur READ getCVBlurValue WRITE setCVBlurValue NOTIFY cvBlurValueChanged)
    Q_PROPERTY(int cv_thresholdmin READ getCVThresholdMinValue WRITE setCVThresholdMinValue NOTIFY cvThresholdMinValueChanged)
    Q_PROPERTY(int cv_thresholdmax READ getCVThresholdMaxValue WRITE setCVThresholdMaxValue NOTIFY cvThresholdMaxValueChanged)
    //Pixel Column Processing Properties---------------------------------------------------------
    Q_PROPERTY(quint64 pc_max_tii READ getMaxTII WRITE setMaxTII NOTIFY pcMaxTIIChanged)
    Q_PROPERTY(quint64 pc_min_tii READ getMinTII WRITE setMinTII NOTIFY pcMinTIIChanged)
    Q_PROPERTY(int pc_min_cluster_size READ getMinClusterSize WRITE setMinClusterSize NOTIFY pcMinClusterSizeChanged)
    Q_PROPERTY(int pc_max_cluster_size READ getMaxClusterSize WRITE setMaxClusterSize NOTIFY pcMaxClusterSizeChanged)
    Q_PROPERTY(int pc_max_clusters_in_column READ getMaxClusterInCol WRITE setMaxClusterInCol NOTIFY pcMaxClusterInColChanged)
    //Skeletal Processing Properties-------------------------------------------------------------


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
    Q_INVOKABLE int getCameraMaxHeight(){return m_pylon_maxcameraheight;}
    Q_INVOKABLE int getCameraMaxWidth(){return m_pylon_maxcamerawidth;}
    Q_INVOKABLE QString getCameraDeviceName(){return m_pylon_cameraname;}
    Q_INVOKABLE int getCameraAOIWidth(){return m_pylon_aoiwidth;}
    Q_INVOKABLE int getCameraAOIHeight(){return m_pylon_aoiheight;}
    Q_INVOKABLE double getCameraExposure(){return m_pylon_exposuretime;}
    Q_INVOKABLE int getCameraGain(){return m_pylon_gain;}
    //Open CV Property Get Functions-----------------------------------------------------------
    Q_INVOKABLE int getCVBlurValue(){return m_cv_blur;}
    Q_INVOKABLE int getCVThresholdMinValue(){return m_cv_thresholdmin;}
    Q_INVOKABLE int getCVThresholdMaxValue(){return m_cv_thresholdmax;}
    //Pixel Column Processing Get Functions
    Q_INVOKABLE quint64 getMaxTII(){return m_pc_max_tii;}
    Q_INVOKABLE quint64 getMinTII(){return m_pc_min_tii;}
    Q_INVOKABLE int getMinClusterSize(){return m_pc_min_clustersize;}
    Q_INVOKABLE int getMaxClusterSize(){return m_pc_max_clustersize;}
    Q_INVOKABLE int getMaxClusterInCol(){return m_pc_max_clustersincolumn;}
    //GUI Property Get Functions--------------------------------------------------------------
    Q_INVOKABLE bool getShowDebugInfo(){return m_gui_showdebuginfo;}

    //Pylon Property Set Functions------------------------------------------------------------
    void setCameraAOIWidth(int _width);
    void setCameraAOIHeight(int _height);
    void setCameraExposure(double _exposure);
    void setCameraGain(int _gain);
    //OpenC V Property Set Functions----------------------------------------------------------
    void setCVBlurValue(int _value);
    void setCVThresholdMinValue(int _value);
    void setCVThresholdMaxValue(int _value);
    //Pixel Column Set Functions-------------------------------------------------------------
    void setMaxTII(quint64 _maxtii);
    void setMinTII(quint64 _mintii);
    void setMinClusterSize(int _cs);
    void setMaxClusterSize(int _cs);
    void setMaxClusterInCol(int _csincol);
    //GUI Property Set Functions--------------------------------------------------------------
    void setShowDebugInfo(bool _value);


signals:
    //General Signals-------------------------------------------------------------------------
    void aboutToDestroy();
    //Toby Related Signals;
    void signalChangeCameraAOI(int _width, int _height);
    void signalChangeCameraExposure(double _exposure);
    void signalChangeCameraGain(int _gain);
    //Max Related Signals---------------------------------------------------------------------
    void signalChangeBlur(int _blur);
    void signalChangeThresholdMin(int _min);
    void signalChangeThresholdMax(int _max);
    void signalChangeMaxTII(quint64 _maxtii);
    void signalChangeMinTII(quint64 _mintii);
    void signalChangeMaxClusterSize(int _size);
    void signalChangeMaxClustersInColumn(int _numofclusters);
    void signalChangeMinClusterSize(int _size);
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
    //Pixel Column Related Signals------------------------------------------------------------
    void pcMaxTIIChanged();
    void pcMinTIIChanged();
    void pcMinClusterSizeChanged();
    void pcMaxClusterSizeChanged();
    void pcMaxClusterInColChanged();
    //GUI Related Signals---------------------------------------------------------------------
    void showDebugInfoChanged();

public slots:
    Q_INVOKABLE void broadcastQMLSignals();
    Q_INVOKABLE void broadcastSingletonSignals();
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

    //Pixel Column Processing Settings and Stuff
    quint64 m_pc_max_tii;
    quint64 m_pc_min_tii;
    int m_pc_max_clustersize;
    int m_pc_min_clustersize;
    int m_pc_max_clustersincolumn;

    //GUI UI Settings and Stuff
    bool m_gui_showdebuginfo;



};

#endif // MARY_H
