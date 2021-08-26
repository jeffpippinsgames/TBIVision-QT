#ifndef TBIWELDTYPE_VGROOVE_H
#define TBIWELDTYPE_VGROOVE_H

#include "tbiweldtype_baseclass.h"

class tbiweldtype_vgroove : public tbiweldtype_baseclass
{
public:
    tbiweldtype_vgroove();

private:
    TBIDataSet *m_left_ransac_tsg_ds;
    TBIDataSet *m_left_inlier_tsg_ds;
    TBIDataSet *m_right_ransac_tsg_ds;
    TBIDataSet *m_right_inlier_tsg_ds;
    TBIDataSet *m_left_ransac1_bwl_ds;
    TBIDataSet *m_left_ransac2_bwl_ds;
    TBIDataSet *m_left_inlier_bwl_ds;
    TBIDataSet *m_right_ransac1_bwl_ds;
    TBIDataSet *m_right_ransac2_bwl_ds;
    TBIDataSet *m_right_inlier_bwl_ds;
    TBIDataSet *m_joint_ds;

    TBIDataSet *m_dummy_set1; //Used For Static Storage. So Other Functions Dont Have To Recreate Them in the Loops.
    TBIDataSet *m_dummy_set2; //Used For Static Storage. So Other Functions Dont Have To Recreate Them in the Loops.
};

#endif // TBIWELDTYPE_VGROOVE_H
