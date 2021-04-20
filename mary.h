#ifndef MARY_H
#define MARY_H

#include <QObject>
#include <QQuickItem>
#include <QFile>
#include <QString>
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"


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
    Q_PROPERTY(QString pylon_cameraname READ getCameraDeviceName NOTIFY pylonCameraNameChanged)
    Q_PROPERTY(int cv_blur READ getCVBlurValue WRITE setCVBlurValue NOTIFY cvBlurValueChanged)
    Q_PROPERTY(int cv_thresholdmin READ getCVThresholdMinValue WRITE setCVThresholdMinValue NOTIFY cvThresholdMinValueChanged)
    Q_PROPERTY(int cv_thresholdmax READ getCVThresholdMaxValue WRITE setCVThresholdMaxValue NOTIFY cvThresholdMaxValueChanged)

public:
    explicit Mary(QObject *parent = nullptr);
    ~Mary();

    //Property get functions
    int getCVBlurValue(){return m_cvblur;}
    int getCVThresholdMinValue(){return m_cvthresholdmin;}
    int getCVThresholdMaxValue(){return m_cvthresholdmax;}
    QString getCameraDeviceName(){return m_pylon_cameraname;}

    //Property set functions
    void setCVBlurValue(int _value){m_cvblur = _value; emit cvBlurValueChanged();}
    void setCVThresholdMinValue(int _value){m_cvthresholdmin = _value; emit cvThresholdMinValueChanged();}
    void setCVThresholdMaxValue(int _value){m_cvthresholdmax = _value; emit cvThresholdMaxValueChanged();}


signals:
    void cvBlurValueChanged();
    void cvThresholdMinValueChanged();
    void cvThresholdMaxValueChanged();

public slots:

private:
    QString m_pylon_cameraname;
    int m_cvblur;
    int m_cvthresholdmin;
    int m_cvthresholdmax;

};

#endif // MARY_H
