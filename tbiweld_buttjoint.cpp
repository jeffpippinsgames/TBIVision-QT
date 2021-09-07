#include "tbiweld_buttjoint.h"

TBIWeld_ButtJoint::TBIWeld_ButtJoint()
{
    m_tsl_inlier_ds = new TBIDataSet();
}

TBIWeld_ButtJoint::~TBIWeld_ButtJoint()
{
    delete m_tsl_inlier_ds;
}
