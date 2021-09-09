#ifndef TBIPARAMETERCLASS_RANSCAPARMS_H
#define TBIPARAMETERCLASS_RANSCAPARMS_H

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
#include "tbiclass_line.h"
#include <QRandomGenerator>
#include <QDataStream>



class TBIRansacParameter : public QObject
{


    Q_OBJECT

    Q_PROPERTY(float idealAngle WRITE setIdealAngle READ getIdealAngle NOTIFY idealAngleChanged)
    Q_PROPERTY(float allowedAngleVariance WRITE setAllowedAngleVariance READ getAllowedAngleVariance NOTIFY allowedAngleVarianceChanged)
    Q_PROPERTY(int minVotes WRITE setMinVotes READ getMinVotes NOTIFY minVotesChanged)
    Q_PROPERTY(int maxVotes WRITE setMaxVotes READ getMaxVotes NOTIFY maxVotesChanged)
    Q_PROPERTY(int iterations WRITE setIterations READ getIterations NOTIFY iterationsChanged)
    Q_PROPERTY(float distanceThreshold WRITE setDistanceThreshold READ getDistanceThreshold NOTIFY distanceThresholdChanged)

public:

    explicit TBIRansacParameter(QObject *parent = nullptr);

    //Processing Functions

    //Get Functions
    float getIdealAngle() const {return m_ideal_angle;}
    float getAllowedAngleVariance() const {return m_allowed_angle_variance;}
    int getMinVotes() const {return m_min_votes;}
    int getMaxVotes() const {return m_max_votes;}
    int getIterations() const {return m_iterations;}
    float getDistanceThreshold() const {return m_distance_threshold;}

    //Set Functions
    void setIdealAngle(float _idealangle);
    void setAllowedAngleVariance(float _allowedanglevariance);
    void setMinVotes(int _minvotes);
    void setMaxVotes(int _minvotes);
    void setIterations(int _iterations);
    void setDistanceThreshold(float _distancethreshold);

    void makeEqual(TBIRansacParameter &_srcransacparam);

    //Save File Methods
    void setDefautValues();
    void saveToFile(QDataStream &_filedatastream);
    void loadFromFile(QDataStream &_filedatastream);

signals:
    void idealAngleChanged();
    void allowedAngleVarianceChanged();
    void minVotesChanged();
    void maxVotesChanged();
    void iterationsChanged();
    void distanceThresholdChanged();

private:

    float m_ideal_angle; //Ideal Angle From Horizontal
    float m_allowed_angle_variance; // The +/- amount the angle can be
    int m_min_votes;
    int m_max_votes;
    int m_iterations;
    float m_distance_threshold;


};

#endif // TBIPARAMETERCLASS_RANSCAPARMS_H
