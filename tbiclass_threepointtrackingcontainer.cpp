#include "tbiclass_threepointtrackingcontainer.h"

TBIThreePointTrackingContainer::TBIThreePointTrackingContainer()
{
    this->clear();
}

TBIThreePointTrackingContainer::TBIThreePointTrackingContainer(TBIThreePointTrackingContainer &_tpc)
{
    m_isvalid = _tpc.m_isvalid;
    m_vertex1.m_x = _tpc.m_vertex1.m_x;
    m_vertex2.m_x = _tpc.m_vertex2.m_x;
    m_vertex3.m_x = _tpc.m_vertex3.m_x;
    m_vertex1.m_y = _tpc.m_vertex1.m_y;
    m_vertex2.m_y = _tpc.m_vertex2.m_y;
    m_vertex3.m_y = _tpc.m_vertex3.m_y;
    m_centroid.m_x = _tpc.m_centroid.m_x;
    m_centroid.m_y = _tpc.m_centroid.m_y;
}

void TBIThreePointTrackingContainer::clear()
{
    m_vertex1.m_x = 0;
    m_vertex1.m_y = 0;
    m_vertex2.m_x = 0;
    m_vertex2.m_y = 0;
    m_vertex3.m_x = 0;
    m_vertex3.m_y = 0;
    m_isvalid = false;

}

void TBIThreePointTrackingContainer::buildFrom4Lines(TBILine &_ltsl, TBILine &_rtsl, TBILine &_lbwl, TBILine &_rbwl)
{

        //Find The 3 Points of Intersection
        if(_ltsl.isValid() && _rtsl.isValid() && _lbwl.isValid() && _rbwl.isValid())
        {
            if((_ltsl.findPointofIntersection(_lbwl, m_vertex1)) && (_rtsl.findPointofIntersection(_rbwl, m_vertex2)) && (_lbwl.findPointofIntersection(_rbwl, m_vertex3)))
            {
                m_isvalid = true;
                updateCentroid();

            }
            else
            {
                this->clear();
                return;
            }

        }
        else
        {
            this->clear();
        }
}

void TBIThreePointTrackingContainer::drawToMat(cv::Mat &_dst, cv::Scalar _color)
{
    if(m_isvalid)
    {
        cv::drawMarker(_dst, m_vertex1.toCVPoint(), _color, cv::MARKER_CROSS, 15, 1, cv::LINE_AA);
        cv::drawMarker(_dst, m_vertex2.toCVPoint(), _color, cv::MARKER_CROSS, 15, 1, cv::LINE_AA);
        cv::drawMarker(_dst, m_vertex3.toCVPoint(), _color, cv::MARKER_CROSS, 15, 1, cv::LINE_AA);
        cv::drawMarker(_dst, m_centroid.toCVPoint(), CV_RGB(0,0,255), cv::MARKER_TILTED_CROSS, 10, 1, cv::LINE_AA);
    }
}

void TBIThreePointTrackingContainer::updateCentroid()
{
    m_centroid.m_x = (m_vertex1.m_x+m_vertex2.m_x+m_vertex3.m_x)/3;
    m_centroid.m_y = (m_vertex1.m_y+m_vertex2.m_y+m_vertex3.m_y)/3;
}


