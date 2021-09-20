#ifndef TBICLASS_DATASET_H
#define TBICLASS_DATASET_H

#include "tbicore_constants.h"
#include "tbiclass_pointint.h"
#include "tbiclass_line.h"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include "tbiclass_datadistributionset.h"
#include "tbiparameterclass_gausiandecluster.h"



//Uses Lot of Memory. We need the Speed.
//Using Dynamic Data Containers are too Slow.
//Memory is Cheap.

enum class TBIDataSetType {NoType, LeftTSLForRansacType, RightTSLForRansacType, LeftBWLForRansacType, RightBWLForRansacType,
                           LeftTSLInlierType, RightTSLForInlierType, LeftBWLForInlierType, RightBWLForInlierType, GausianDeclusterType};

enum class TBIDataSetReturnType {Ok, FailedFunctionPassedInWrongDataSetType, FailedFunctionProducedInvalidResult, FailedWrongDataSetTypeForFunction,
                                FailedFunctionPassedInWrongMatType, FailedPassedInDataSetWrongSize, FailedPassedInParameterMakesNoSense, FailedTotalImageIntensityTooHigh, FailedTotalImageIntensityTooLow};

class TBIDataSet
{
public:
    TBIDataSet(){m_dataset_size = 0; m_dataset_type = TBIDataSetType::NoType;}
    TBIDataSet(TBIDataSet &_dataset){*this = _dataset;}

    //Uitility Methods
    int size() const {return m_dataset_size;}
    void insert(TBIPoint_Int &_pnt)
    {
        if(m_dataset_size < Largest_DataSet_Size)
        {
            ++m_dataset_size;
            m_pnts[m_dataset_size-1]=_pnt;
        }
    }
    void clear(){m_dataset_size = 0; m_dataset_type = TBIDataSetType::NoType;}
    TBIDataSetType getDataSetType(){return m_dataset_type;}
    void drawToMat(cv::Mat &_dst);
    void drawToMat(cv::Mat &_dst, cv::Scalar _color);
    void eraseFromMat(cv::Mat &_dst);
    int getIndexofHighestY();
    int getIndexofHighestY(const TBIDataDistributionSet &_distroset);
    int getIndexofLowestX();
    int getIndexofHighestX();
    int getHighestX();
    int getLowestX();
    TBIPoint_Int getPoint(int _index){ return m_pnts[_index];}

    //Build Methods
    //Build Methods Create New Dataset types from the Source Dataset.
    //For Gausian Clustering
    TBIDataSetReturnType buildGausianClusterDataSet(cv::Mat &_thresholdmat, TBIGausianDeclusteringParameters &_declusterparameter);
    //For The TSL Ransac SubSets
    TBIDataSetReturnType buildLeftTSLRansacDataSet(const TBIDataSet & _gausiansrcds, const int _endingsrcindex);
    TBIDataSetReturnType buildRightTSLRansacDataSet(const TBIDataSet & _gausiansrcds, const int _startingsrcindex);
    TBIDataSetReturnType buildLeftBWLRansacDataSet(const TBIDataSet & _gausiansrcds, const int _endingsrcindex);
    TBIDataSetReturnType buildRightBWLRansacDataSet(const TBIDataSet & _gausiansrcds, const int _startingsrcindex);
    //For The Inlier SubSets
    TBIDataSetReturnType buildLeftBWLInlierDataSet(const TBIDataSet & _gausiansrcds, const TBILine &_leftbwlransacline);
    TBIDataSetReturnType buildRightBWLInlierDataSet(const TBIDataSet &_gausiansrcds, const TBILine &_rightbwlransacline);
    TBIDataSetReturnType buildLeftTSLInlierDataSet(const TBIDataSet &_gausiansrcds, const TBILine &_lefttslransacline);
    TBIDataSetReturnType buildRightTSLInlierDataSet(const TBIDataSet &_gausiansrcds, const TBILine &_righttslransacline);

    //Extract Methods
    //Extract Methods Create New Entities By Extracting From This DataSet
    TBIDataSetReturnType extractDataSetForLeftTSL(TBIDataSet &_dst, unsigned int _uniquexvalues);
    TBIDataSetReturnType extractDataSetForRightTSL(TBIDataSet &_dst, unsigned int _uniquexvalues);
    TBIDataSetReturnType extractDataSetForRansacLeftTSL(TBIDataSet &_dst, int _breakindex);
    TBIDataSetReturnType extractDataSetForRansacRightTSL(TBIDataSet &_dst, int _startindex);
    TBIDataSetReturnType extractDataSetForJoint(TBIDataSet &_dst, const TBILine &_lefttsl, const TBILine &_righttsl, const float _inlierdistancethreshold);
    TBIDataSetReturnType extractVGrooveJointDataSet(TBIDataSet &_dst, const TBILine &_lefttsl, const TBILine &_righttsl, const float _inlierdistancethreshold);
    TBIDataSetReturnType extractDataSetForInliers(TBIDataSet &_dst, const TBILine &_line, const float _distancethreshold);
    TBIDataSetReturnType extractDataSetForInliers(TBIDataSet &_dst, const TBILine &_ransacline, const float _distancethreshold, const int _start_index, const int _end_index);
    TBIDataSetReturnType extractInlierDataSet(TBIDataSet &_dst, const TBIDataDistributionSet &_srcdistro);
    TBIDataSetReturnType extractDistributionSet(TBIDataDistributionSet &_distroset);
    TBIDataSetReturnType extractLeastSquareLine(TBILine &_line);
    TBIDataSetReturnType extractFilteredGausianSet(TBIDataSet &_dst, int _breakindex);
    TBIDataSetReturnType extractDataSubSet(TBIDataSet &_dst, int _startindex, int _endindex);

    //Filter Functions

    //Overloaded Operators

    TBIPoint_Int operator [] (int _index) const
    {
        TBIPoint_Int _pnt(m_pnts[_index].m_x, m_pnts[_index].m_y);
        return _pnt;
    }
    void operator = (TBIDataSet &_dataset)
    {
        m_dataset_size = _dataset.size();
        m_dataset_type = _dataset.getDataSetType();
        int _index = 0;
        if(m_dataset_size > 0)
        {
            do
            {
                m_pnts[_index].m_x = _dataset[_index].m_x;
                m_pnts[_index].m_y = _dataset[_index].m_y;

            }while(_index < m_dataset_size);
        }
    }


private:
    const bool m_showdebug = false;
    static const int Mat_Max_Width = TBIConstants::Max_Camera_Width;
    static const int Mat_Max_Height = TBIConstants::Max_Camera_Height;
    static const int Largest_DataSet_Size = Mat_Max_Width*Mat_Max_Height;
    TBIPoint_Int m_pnts[Largest_DataSet_Size];
    int m_dataset_size;
    TBIDataSetType m_dataset_type;
};

#endif // TBICLASS_DATASET_H
