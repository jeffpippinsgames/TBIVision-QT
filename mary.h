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
 int m_cvthresholdMax
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
    Q_PROPERTY(int sk_max_discontinuity READ getMaxDiscontinuity WRITE setMaxDiscontinuity NOTIFY skMaxDiscontuityChanged)
    //Ransac Processing Properties-------------------------------------------------------------
    Q_PROPERTY(float rn_left_tsl_min_angle READ getLeftTSLMinAngle WRITE setLeftTSLMinAngle NOTIFY rnLeftTSLMinAngleChanged)
    Q_PROPERTY(float rn_left_tsl_max_angle READ getLeftTSLMaxAngle WRITE setLeftTSLMaxAngle NOTIFY rnLeftTSLMaxAngleChanged)
    Q_PROPERTY(int rn_left_tsl_votes READ getLeftTSLMinVotes WRITE setLeftTSLMinVotes NOTIFY rnLeftTSLMinVotesChanged)
    Q_PROPERTY(float rn_left_tsl_distance_threshold READ getLeftTSLDistanceThreshold WRITE setLeftTSLDistanceThreshold NOTIFY rnLeftTSLDistanceThresholdChanged)
    Q_PROPERTY(int rn_left_tsl_iterations READ getLeftTSLIterations WRITE setLeftTSLIterations NOTIFY rnLeftTSLIterationsChanged)

    Q_PROPERTY(float rn_right_tsl_min_angle READ getRightTSLMinAngle WRITE setRightTSLMinAngle NOTIFY rnRightTSLMinAngleChanged)
    Q_PROPERTY(float rn_right_tsl_max_angle READ getRightTSLMaxAngle WRITE setRightTSLMaxAngle NOTIFY rnRightTSLMaxAngleChanged)
    Q_PROPERTY(int rn_right_tsl_votes READ getRightTSLMinVotes WRITE setRightTSLMinVotes NOTIFY rnRightTSLMinVotesChanged)
    Q_PROPERTY(float rn_right_tsl_distance_threshold READ getRightTSLDistanceThreshold WRITE setRightTSLDistanceThreshold NOTIFY rnRightTSLDistanceThresholdChanged)
    Q_PROPERTY(int rn_right_tsl_iterations READ getRightTSLIterations WRITE setRightTSLIterations NOTIFY rnRightTSLIterationsChanged)

    Q_PROPERTY(float rn_left_bwl_min_angle READ getLeftBWLMinAngle WRITE setLeftBWLMinAngle NOTIFY rnLeftBWLMinAngleChanged)
    Q_PROPERTY(float rn_left_bwl_max_angle READ getLeftBWLMaxAngle WRITE setLeftBWLMaxAngle NOTIFY rnLeftBWLMaxAngleChanged)
    Q_PROPERTY(int rn_left_bwl_votes READ getLeftBWLMinVotes WRITE setLeftBWLMinVotes NOTIFY rnLeftBWLMinVotesChanged)
    Q_PROPERTY(float rn_left_bwl_distance_threshold READ getLeftBWLDistanceThreshold WRITE setLeftBWLDistanceThreshold NOTIFY rnLeftBWLDistanceThresholdChanged)
    Q_PROPERTY(int rn_left_bwl_iterations READ getLeftBWLIterations WRITE setLeftBWLIterations NOTIFY rnLeftBWLIterationsChanged)

    Q_PROPERTY(float rn_right_bwl_min_angle READ getRightBWLMinAngle WRITE setRightBWLMinAngle NOTIFY rnRightBWLMinAngleChanged)
    Q_PROPERTY(float rn_right_bwl_max_angle READ getRightBWLMaxAngle WRITE setRightBWLMaxAngle NOTIFY rnRightBWLMaxAngleChanged)
    Q_PROPERTY(int rn_right_bwl_votes READ getRightBWLMinVotes WRITE setRightBWLMinVotes NOTIFY rnRightBWLMinVotesChanged)
    Q_PROPERTY(float rn_right_bwl_distance_threshold READ getRightBWLDistanceThreshold WRITE setRightBWLDistanceThreshold NOTIFY rnRightBWLDistanceThresholdChanged)
    Q_PROPERTY(int rn_right_bwl_iterations READ getRightBWLIterations WRITE setRightBWLIterations NOTIFY rnRightBWLIterationsChanged)

    //Split Merge Processing Properties-------------------------------------------------------------
    Q_PROPERTY(float sm_split_distance READ getSplitDistance WRITE setSplitDistance NOTIFY smSplitDistanceChanged)
    Q_PROPERTY(float sm_split_length READ getSplitLength WRITE setSplitLength NOTIFY smSplitLengthChanged)


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
    //Skeletal Processing Get Functions
    Q_INVOKABLE int getMaxDiscontinuity(){return m_sk_max_discontinuity;}
    //Ransac Processing Get Functions
    Q_INVOKABLE float getLeftTSLMinAngle(){return m_left_tsl_min_angle;}
    Q_INVOKABLE float getLeftTSLMaxAngle(){return m_left_tsl_max_angle;}
    Q_INVOKABLE int getLeftTSLMinVotes(){return m_left_tsl_min_votes;}
    Q_INVOKABLE float getLeftTSLDistanceThreshold(){return m_left_tsl_distance_threshold;}
    Q_INVOKABLE float getLeftTSLIterations(){return m_left_tsl_iterations;}

    Q_INVOKABLE float getRightTSLMinAngle(){return m_right_tsl_min_angle;}
    Q_INVOKABLE float getRightTSLMaxAngle(){return m_right_tsl_max_angle;}
    Q_INVOKABLE int getRightTSLMinVotes(){return m_right_tsl_min_votes;}
    Q_INVOKABLE float getRightTSLDistanceThreshold(){return m_right_tsl_distance_threshold;}
    Q_INVOKABLE int getRightTSLIterations(){return m_right_tsl_iterations;}

    Q_INVOKABLE float getLeftBWLMinAngle(){return m_left_bwl_min_angle;}
    Q_INVOKABLE float getLeftBWLMaxAngle(){return m_left_bwl_max_angle;}
    Q_INVOKABLE int getLeftBWLMinVotes(){return m_left_bwl_min_votes;}
    Q_INVOKABLE float getLeftBWLDistanceThreshold(){return m_left_bwl_distance_threshold;}
    Q_INVOKABLE int getLeftBWLIterations(){return m_left_bwl_iterations;}

    Q_INVOKABLE float getRightBWLMinAngle(){return m_right_bwl_min_angle;}
    Q_INVOKABLE float getRightBWLMaxAngle(){return m_right_bwl_max_angle;}
    Q_INVOKABLE int getRightBWLMinVotes(){return m_right_bwl_min_votes;}
    Q_INVOKABLE float getRightBWLDistanceThreshold(){return m_right_bwl_distance_threshold;}
    Q_INVOKABLE int getRightBWLIterations(){return m_right_bwl_iterations;}

    //SplitMerge Processing Get Functions
    Q_INVOKABLE float getSplitDistance(){return m_split_distance;}
    Q_INVOKABLE float getSplitLength(){return m_split_length;}

    //GUI Property Get Functions-------------------------------------------------------------
    Q_INVOKABLE bool getShowDebugInfo(){return m_gui_showdebuginfo;}

    //Pylon Property Set Functions-----------------------------------------------------------
    void setCameraAOIWidth(int _width);
    void setCameraAOIHeight(int _height);
    void setCameraExposure(double _exposure);
    void setCameraGain(int _gain);
    //OpenC V Property Set Functions---------------------------------------------------------
    void setCVBlurValue(int _value);
    void setCVThresholdMaxValue(int _value);
    void setCVThresholdMinValue(int _value);
    //Pixel Column Set Functions-------------------------------------------------------------
    void setMaxTII(quint64 _maxtii);
    void setMinTII(quint64 _mintii);
    void setMaxClusterSize(int _cs);
    void setMinClusterSize(int _cs);
    void setMaxClusterInCol(int _csincol);
    //Skeletal Set Functions-----------------------------------------------------------------
    void setMaxDiscontinuity(int _disc);
    //Ransac Set Functions-------------------------------------------------------------------
    void setLeftTSLMinAngle(float _minangle);
    void setLeftTSLMaxAngle(float _maxangle);
    void setLeftTSLMinVotes(int _minvotes);
    void setLeftTSLDistanceThreshold(float _distthreshold);
    void setLeftTSLIterations(int _iterations);

    void setRightTSLMinAngle(float _minangle);
    void setRightTSLMaxAngle(float _maxangle);
    void setRightTSLMinVotes(int _minvotes);
    void setRightTSLDistanceThreshold(float _distthreshold);
    void setRightTSLIterations(int _iterations);

    void setLeftBWLMinAngle(float _minangle);
    void setLeftBWLMaxAngle(float _maxangle);
    void setLeftBWLMinVotes(int _minvotes);
    void setLeftBWLDistanceThreshold(float _distthreshold);
    void setLeftBWLIterations(int _iterations);

    void setRightBWLMinAngle(float _minangle);
    void setRightBWLMaxAngle(float _maxangle);
    void setRightBWLMinVotes(int _minvotes);
    void setRightBWLDistanceThreshold(float _distthreshold);
    void setRightBWLIterations(int _iterations);

    //Split Merge Set Functions---------------------------------------------------------------
    void setSplitDistance(float _distance);
    void setSplitLength(float _length);

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
    //OpenCV
    void signalChangeBlur(int _blur);
    void signalChangeThresholdMin(int _min);
    void signalChangeThresholdMax(int _max);
    //Pixel Cluster
    void signalChangeMaxTII(quint64 _maxtii);
    void signalChangeMinTII(quint64 _mintii);
    void signalChangeMaxClusterSize(int _size);
    void signalChangeMaxClustersInColumn(int _numofclusters);
    void signalChangeMinClusterSize(int _size);
    //Skeletal
    void signalChangeMaxDiscontinuity(int _value);
    //RANSAC
    void signalLeftTSLMinAngle(float _minangle);
    void signalLeftTSLMaxAngle(float _maxangle);
    void signalLeftTSLMinVotes(int _minvotes);
    void signalLeftTSLDistanceThreshold(float _distthreshold);
    void signalLeftTSLIterations(int _iterations);

    void signalRightTSLMinAngle(float _minangle);
    void signalRightTSLMaxAngle(float _maxangle);
    void signalRightTSLMinVotes(int _minvotes);
    void signalRightTSLDistanceThreshold(float _distthreshold);
    void signalRightTSLIterations(int _iterations);

    void signalLeftBWLMinAngle(float _minangle);
    void signalLeftBWLMaxAngle(float _maxangle);
    void signalLeftBWLMinVotes(int _minvotes);
    void signalLeftBWLDistanceThreshold(float _distthreshold);
    void signalLeftBWLIterations(int _iterations);

    void signalRightBWLMinAngle(float _minangle);
    void signalRightBWLMaxAngle(float _maxangle);
    void signalRightBWLMinVotes(int _minvotes);
    void signalRightBWLDistanceThreshold(float _distthreshold);
    void signalRightBWLIterations(int _iterations);

    //Split Merge
    void signalSplitDistance(float _distance);
    void signalSplitLength(float _length);

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
    void cvThresholdMaxValueChanged();
    void cvThresholdMinValueChanged();
    //Pixel Column Related Signals------------------------------------------------------------
    void pcMaxTIIChanged();
    void pcMinTIIChanged();
    void pcMaxClusterSizeChanged();
    void pcMinClusterSizeChanged();
    void pcMaxClusterInColChanged();
    //Skeletal Related Signals----------------------------------------------------------------
    void skMaxDiscontuityChanged();
    //Ransac Related Signals------------------------------------------------------------------
    void rnLeftTSLMinAngleChanged();
    void rnLeftTSLMaxAngleChanged();
    void rnLeftTSLMinVotesChanged();
    void rnLeftTSLDistanceThresholdChanged();
    void rnLeftTSLIterationsChanged();

    void rnRightTSLMinAngleChanged();
    void rnRightTSLMaxAngleChanged();
    void rnRightTSLMinVotesChanged();
    void rnRightTSLDistanceThresholdChanged();
    void rnRightTSLIterationsChanged();

    void rnLeftBWLMinAngleChanged();
    void rnLeftBWLMaxAngleChanged();
    void rnLeftBWLMinVotesChanged();
    void rnLeftBWLDistanceThresholdChanged();
    void rnLeftBWLIterationsChanged();

    void rnRightBWLMinAngleChanged();
    void rnRightBWLMaxAngleChanged();
    void rnRightBWLMinVotesChanged();
    void rnRightBWLDistanceThresholdChanged();
    void rnRightBWLIterationsChanged();

    void smSplitDistanceChanged();
    void smSplitLengthChanged();
    //GUI Related Signals---------------------------------------------------------------------
    void showDebugInfoChanged();

public slots:
    Q_INVOKABLE void broadcastQMLSignals();
    Q_INVOKABLE void broadcastSingletonSignals();
private:
    //Pylon Settings and Stuff
    const int m_pylon_maxcamerawidth = 724;
    const int m_pylon_maxcameraheight = 542;
    const int m_pylon_Maxoffsetx = 0;
    const int m_pylon_Maxoffsety = 0;
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

    //Skeletal Processing Settings and Stuff
    int m_sk_max_discontinuity;

    //Ransac Processing Settings and Stuff
    float m_left_tsl_min_angle;
    float m_left_tsl_max_angle;
    int m_left_tsl_min_votes;
    int m_left_tsl_iterations;
    float m_left_tsl_distance_threshold;

    float m_right_tsl_min_angle;
    float m_right_tsl_max_angle;
    int m_right_tsl_min_votes;
    int m_right_tsl_iterations;
    float m_right_tsl_distance_threshold;

    float m_left_bwl_min_angle;
    float m_left_bwl_max_angle;
    int m_left_bwl_min_votes;
    int m_left_bwl_iterations;
    float m_left_bwl_distance_threshold;

    float m_right_bwl_min_angle;
    float m_right_bwl_max_angle;
    int m_right_bwl_min_votes;
    int m_right_bwl_iterations;
    float m_right_bwl_distance_threshold;

    //Split Merge Processing Settings and Stuff
    float m_split_distance;
    float m_split_length;

    //GUI UI Settings and Stuff
    bool m_gui_showdebuginfo;




};

#endif // MARY_H
