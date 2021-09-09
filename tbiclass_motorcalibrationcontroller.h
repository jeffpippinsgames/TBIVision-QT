#ifndef TBICLASS_MOTORCALIBRATIONCONTROLLER_H
#define TBICLASS_MOTORCALIBRATIONCONTROLLER_H

#include <QObject>
#include "tbicore_gary.h"
#include "tbicore_max.h"
#include "tbiparameterclass_motioncontrolparams.h"

class TBIMotorCalibrationController : public QObject
{
    Q_OBJECT
public:
    explicit TBIMotorCalibrationController(QObject *parent = nullptr);

    void setTBICorePointers(Max *_max, Gary *_gary){m_gary=_gary; m_max=_max;}
    void processMotorCalibration(TBIPoint_Int &_trackedpoint, TBIMotionControlParameters &_motionparams);
    void resetControlParams(){m_got_cal_pnt1=false; m_got_cal_pnt2=false;}

signals:


private:
    Gary *m_gary;
    Max *m_max;

    bool m_got_cal_pnt1;
    bool m_got_cal_pnt2;
    TBIPoint_Int m_cal_pnt1;
    TBIPoint_Int m_cal_pnt2;

};

#endif // TBICLASS_MOTORCALIBRATIONCONTROLLER_H
