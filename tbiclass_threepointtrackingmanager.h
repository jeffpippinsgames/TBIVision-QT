#ifndef TBICLASS_THREEPOINTTRACKINGMANAGER_H
#define TBICLASS_THREEPOINTTRACKINGMANAGER_H

#include "tbiclass_threepointtrackingcontainer.h"

class TBIThreePointTrackingManager
{
public:
    TBIThreePointTrackingManager();
    void resetList(){m_current_index = 0;}
    void insert(TBIThreePointTrackingContainer &_pnt);



    TBIThreePointTrackingContainer m_tracking_point;

private:
    void updateAverage();

    static const int TrackingListSize = 5;
    TBIThreePointTrackingContainer m_trackingpoints[TrackingListSize];

    int m_current_index;
};

#endif // TBICLASS_THREEPOINTTRACKINGMANAGER_H
