#ifndef TBIDATASET_H
#define TBIDATASET_H

#include "tbipoint_int.h"
#include "tbiline.h"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"



//Uses Lot of Memory. We need the Speed.


class TBIDataSet
{
public:
    TBIDataSet(){m_dataset_size = 0;}
    TBIDataSet(TBIDataSet &_dataset){*this = _dataset;}


    int size() const {return m_dataset_size;}
    void insert(TBIPoint_Int &_pnt)
    {
        if(m_dataset_size < Largest_DataSet_Size)
        {
            ++m_dataset_size;
            m_pnts[m_dataset_size-1]=_pnt;
        }
    }
    void clear(){m_dataset_size = 0;}
    void drawToMat(cv::Mat &_dst);
    void drawToMat(cv::Mat &_dst, cv::Scalar _color);
    void eraseFromMat(cv::Mat &_dst);
    void buildDataSetForLeftTSL(TBIDataSet &_dst, unsigned int _uniquexvalues);
    void buildDataSetForRightTSL(TBIDataSet &_dst, unsigned int _uniquexvalues);
    void buildDataSetForInliers(TBIDataSet &_dst, const TBILine &_line, const float _distancethreshold);
    void buildLeastSquareLine(TBILine &_line);

    const TBIPoint_Int& operator [] (std::size_t _index) const
    {
        return m_pnts[_index];
    }

    TBIPoint_Int& operator [] (std::size_t _index)
    {
        return m_pnts[_index];
    }

    void operator = (TBIDataSet &_dataset)
    {
        m_dataset_size = _dataset.size();
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
    static const int Mat_Max_Width = 728;
    static const int Mat_Max_Height = 544;
    static const int Largest_DataSet_Size = Mat_Max_Width*Mat_Max_Height;
    TBIPoint_Int m_pnts[Largest_DataSet_Size];
    int m_dataset_size;
};

#endif // TBIDATASET_H
