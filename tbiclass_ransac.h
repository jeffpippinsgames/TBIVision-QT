#ifndef TBICLASS_RANSAC_H
#define TBICLASS_RANSAC_H

#include "tbiclass_dataset.h"
#include "tbiparameterclass_ransacvotingstruct.h"
#include "tbiclass_line.h"
#include "tbiparameterclass_ranscaparms.h"
#include "tbiclass_pointfloat.h"
#include "tbiclass_pointint.h"

class TBIRansac
{
public:
    TBIRansac();

    static bool doRansac(TBILine &_line, const TBIRansacParameter &_ransac, const TBIDataSet &_dataset);
    static bool doRansac(TBILine &_line, const TBIRansacParameter &_ransac, const TBIDataSet &_dataset, const int _startdsindex, const int _enddsindex);
    static int randomInt(int _min, int _max);
};

#endif // TBICLASS_RANSAC_H
