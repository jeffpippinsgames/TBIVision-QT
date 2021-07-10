#ifndef TBICLASS_THREEPOINTTRACKINGCONTAINER_H
#define TBICLASS_THREEPOINTTRACKINGCONTAINER_H

#include "tbiclass_pointint.h"
#include "tbiclass_line.h"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"

class TBIThreePointTrackingContainer
{
public:
    TBIThreePointTrackingContainer();
    TBIThreePointTrackingContainer(TBIThreePointTrackingContainer &_tpc);
    void clear();
    bool isValid(){return m_isvalid;}
    void validate(){m_isvalid = true;}
    void buildFrom4Lines(TBILine &_ltsl, TBILine &_rtsl, TBILine &_lbwl, TBILine &_rbwl);
    void drawToMat(cv::Mat &_dst, cv::Scalar _color);
    void updateCentroid();
    TBIPoint_Int getCentroid(){return m_centroid;}
    TBIPoint_Int getVertex1() {return m_vertex1;}
    TBIPoint_Int getVertex2() {return m_vertex2;}
    TBIPoint_Int getVertex3() {return m_vertex3;}


    void operator = (const TBIThreePointTrackingContainer &_tpc)
    {

        m_isvalid = _tpc.m_isvalid;
        m_vertex1 = _tpc.m_vertex1;
        m_vertex2 = _tpc.m_vertex2;
        m_vertex3 = _tpc.m_vertex3;
        m_centroid = _tpc.m_centroid;

    }

    TBIThreePointTrackingContainer operator + (TBIThreePointTrackingContainer &_tpc)
    {
        TBIThreePointTrackingContainer __tc;
        if(_tpc.m_isvalid)
        {
            __tc.m_isvalid = true;
            __tc.m_vertex1.m_x = m_vertex1.m_x + _tpc.m_vertex1.m_x;
            __tc.m_vertex2.m_x = m_vertex2.m_x +_tpc.m_vertex2.m_x;
            __tc.m_vertex3.m_x = m_vertex3.m_x +_tpc.m_vertex3.m_x;
            __tc.m_vertex1.m_y = m_vertex1.m_y + _tpc.m_vertex1.m_y;
            __tc.m_vertex2.m_y = m_vertex2.m_y + _tpc.m_vertex2.m_y;
            __tc.m_vertex3.m_y = m_vertex3.m_y + _tpc.m_vertex3.m_y;
            __tc.m_centroid.m_x = m_centroid.m_x + _tpc.m_centroid.m_x;
            __tc.m_centroid.m_y = m_centroid.m_y + _tpc.m_centroid.m_y;
        }
        return __tc;
    }

    void operator / (int _div)
    {

             if(_div == 0) return;
             m_vertex1.m_x = m_vertex1.m_x / _div;
             m_vertex2.m_x = m_vertex2.m_x / _div;
             m_vertex3.m_x = m_vertex3.m_x / _div;
             m_vertex1.m_y = m_vertex1.m_y / _div;
             m_vertex2.m_y = m_vertex2.m_y / _div;
             m_vertex3.m_y = m_vertex3.m_y / _div;
             m_centroid.m_x = m_centroid.m_x / _div;
             m_centroid.m_y = m_centroid.m_y / _div;


    }

    void operator += (TBIThreePointTrackingContainer _tpc)
    {


            m_vertex1.m_x += _tpc.m_vertex1.m_x;
            m_vertex2.m_x += _tpc.m_vertex2.m_x;
            m_vertex3.m_x += _tpc.m_vertex3.m_x;
            m_vertex1.m_y += _tpc.m_vertex1.m_y;
            m_vertex2.m_y += _tpc.m_vertex2.m_y;
            m_vertex3.m_y += _tpc.m_vertex3.m_y;
            m_centroid.m_x += _tpc.m_centroid.m_x;
            m_centroid.m_y += _tpc.m_centroid.m_y;

    }

private:


private:
    TBIPoint_Int m_vertex1;
    TBIPoint_Int m_vertex2;
    TBIPoint_Int m_vertex3;

    TBIPoint_Int m_centroid;
    bool m_isvalid;

};

#endif // TBICLASS_THREEPOINTTRACKINGCONTAINER_H
