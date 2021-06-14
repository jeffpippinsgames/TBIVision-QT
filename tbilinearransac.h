#ifndef TBILINEARRANSAC_H
#define TBILINEARRANSAC_H

#include <QObject>
#include <QImage>
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include <QImage>
#include <QQuickItem>
#include <QString>
#include <QElapsedTimer>
#include <QList>
#include <vector>
#include "pixelcolumnclass.h"
#include "tbiline.h"
#include <QRandomGenerator>



class TBILinearRansac : public QObject
{
    Q_OBJECT

public:
    explicit TBILinearRansac(QObject *parent = nullptr);


    //Processing Functions
    bool doRansacProcessingLinearLines(TBILine& _dstline, float *_skel_data, int _startindex, int _endindex);

    //get Functions
    Q_INVOKABLE float getIdealAngle() const {return m_ideal_angle;}
    Q_INVOKABLE float getAllowedAngleVariance() const {return m_allowed_angle_variance;}
    Q_INVOKABLE int getMinVotes() const {return m_min_votes;}
    Q_INVOKABLE int getIteration() const {return m_iterations;}
    Q_INVOKABLE float getDistanceThreshold() const {return m_distance_threshold;}

    //set Functions
    Q_INVOKABLE void setIdealAngle(float _idealangle);
    Q_INVOKABLE void setAllowedAngleVariance(float _allowedanglevariance);
    Q_INVOKABLE void setMinVotes(int _minvotes);
    Q_INVOKABLE void setIterations(int _iterations);
    Q_INVOKABLE void setDistanceThreshold(float _distancethreshold);

    void makeEqual(TBILinearRansac &_ransac);

private:
    float m_ideal_angle; //Ideal Angle From Horizontal
    float m_allowed_angle_variance; // The +/- amount the angle can be
    int m_min_votes;
    int m_iterations;
    float m_distance_threshold;

public slots:


signals:

};

#endif // TBILINEARRANSAC_H
