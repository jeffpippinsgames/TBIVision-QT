#include "tbiclass_threepointtrackingmanager.h"

TBIThreePointTrackingManager::TBIThreePointTrackingManager()
{
    m_current_index = 0;
}

void TBIThreePointTrackingManager::insert(TBIThreePointTrackingContainer &_pnt)
{
    if(_pnt.isValid())
    {
        ++m_current_index;
        if(m_current_index == TrackingListSize) m_current_index = 0;
        m_trackingpoints[m_current_index] = _pnt;
        updateAverage();
    }

}

void TBIThreePointTrackingManager::updateAverage()
{
    m_tracking_point.clear();
    m_tracking_point.validate();
    int _index = 0;
    do
    {
        m_tracking_point += m_trackingpoints[_index];
        ++_index;
    }while(_index < TrackingListSize);
    m_tracking_point/TrackingListSize;
    m_tracking_point.updateCentroid();
}

