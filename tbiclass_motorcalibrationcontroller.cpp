#include "tbiclass_motorcalibrationcontroller.h"

TBIMotorCalibrationController::TBIMotorCalibrationController(QObject *parent) : QObject(parent)
{
    m_got_cal_pnt1 = false;
    m_got_cal_pnt2 = false;
}

void TBIMotorCalibrationController::processMotorCalibration(TBIPoint_Int &_trackedpoint, TBIMotionControlParameters &_motionparams)
{
    /*
    switch(m_gary->getMotorCalibrationCycle())
    {
    case GaryMotorCalibrationCycleStatus::TBI_MOTORCAL_GETPNT1PAUSE:
        if(!m_got_cal_pnt1)
        {
            m_cal_pnt1 = _trackedpoint;
            m_gary->sendProceedNextMotorPhase();
            m_got_cal_pnt1 = true;
        }
        break;
    case GaryMotorCalibrationCycleStatus::TBI_MOTORCAL_GETPNT2PAUSE:
        if(!m_got_cal_pnt2)
        {
            m_cal_pnt2 = _trackedpoint;
            qint32 _xspp = m_cal_pnt1.m_x - m_cal_pnt2.m_x;
            qint32 _zspp = m_cal_pnt1.m_y - m_cal_pnt2.m_y;
            _motionparams.setXStepsPerPixel(abs(10000.0/(float)_xspp));
            _motionparams.setZStepsPerPixel(abs(10000.0/(float)_zspp));
            m_gary->sendProceedNextMotorPhase();
            m_got_cal_pnt2 = true;
        }
        break;
     default:
        break;
    }
    */
}
