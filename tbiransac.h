#ifndef TBIRANSAC_H
#define TBIRANSAC_H

#include "tbidataset.h"
#include "tbilinearransacvotingstructure.h"
#include "tbiline.h"
#include "tbilinearransac.h"
#include "tbipoint_float.h"
#include "tbipoint_int.h"

class TBIRansac
{
public:
    TBIRansac();

    static bool doRansac(TBILine &_line, const TBILinearRansac &_ransac, const TBIDataSet &_dataset);
    static bool doRansacWithInliers(TBILine &_line, TBIDataSet &_inlierset, TBIDataSet &_dummyset1, TBIDataSet &_dummyset2,
                                    const TBILinearRansac &_ransac, const TBIDataSet &_dataset);
    static int randomInt(int _min, int _max);
};

#endif // TBIRANSAC_H
