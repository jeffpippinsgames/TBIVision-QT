#include "tbidataset.h"
#include "random"
#include "tbilinearransacvotingstructure.h"



bool TBIDataSet::doRansac(TBILine &_line, const TBILinearRansac &_ransac)
{
    _line.clear();
    if(m_dataset_size < _ransac.getMinVotes()) return false;
    if(m_dataset_size == 0) return false;

    int _cnt;
    const int _iterations = _ransac.getIteration();
    TBILine _lines[_iterations];
    TBILinearRansacVotingStructure _ransacresults[_iterations];
    TBIPoint_Int _pnt1;
    TBIPoint_Int _pnt2;


    //generate the Lines
    do
    {

        _cnt;
    }while(_cnt < _iterations);

    //generate the results
    _cnt = 0;
    do
    {
        ++_cnt;
    }while(_cnt < _iterations);
}
