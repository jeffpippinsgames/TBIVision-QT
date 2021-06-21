#include "max.h"
#include <QDebug>
#include "pixelcolumnclass.h"
#include "pixelclusterclass.h"
#include "pixelfundamental.h"
#include "tbilinearransacvotingstructure.h"
#include <QRandomGenerator>
#include <QThread>
#include <vector>
#include <random>
#include "tbipoint_int.h"
#include "tbiransac.h"

//Constructors and Destructor--------------------------------------------------------
Max::Max(QObject *parent) : QObject(parent)
{

    m_scan_ds = new TBIDataSet();
    m_scan_distro = new TBIDataDistributionSet();
    m_skeletal_ds = new TBIDataSet();
    m_roughing_left_tsl_ds = new TBIDataSet();
    m_roughing_right_tsl_ds = new TBIDataSet();
    m_left_tsl_inliers_ds = new TBIDataSet();
    m_left_tsl_distro = new TBIDataDistributionSet();
    m_right_tsl_inliers_ds = new TBIDataSet();
    m_right_tsl_distro = new TBIDataDistributionSet();
    m_joint_ds = new TBIDataSet();
    m_left_bwl_inlier_ds = new TBIDataSet();
    m_right_bwl_inlier_ds = new TBIDataSet();
    m_dummy_set1 = new TBIDataSet();
    m_dummy_set2 = new TBIDataSet();




    m_emitextramats = false;

    m_in_proccesing_loop = false;
    m_timeinloop = "Error:";
    m_blur = 3;
    m_thresholdmin = 100;
    m_thresholdmax = 255;

    m_max_image_intensity = 720*540*255*.3;
    m_min_image_intensity = 720*255;
    m_min_cluster_size = 6;
    m_max_cluster_size = 75;

    m_allowable_discontinuities = 20;



    m_left_tsl_ransac.setIdealAngle(0);
    m_left_tsl_ransac.setAllowedAngleVariance(10);
    m_left_tsl_ransac.setDistanceThreshold(2.0);
    m_left_tsl_ransac.setIterations(50);
    m_left_tsl_ransac.setMinVotes(20);

    m_right_tsl_ransac.setIdealAngle(0);
    m_right_tsl_ransac.setAllowedAngleVariance(10);
    m_right_tsl_ransac.setDistanceThreshold(2.0);
    m_right_tsl_ransac.setIterations(50);
    m_right_tsl_ransac.setMinVotes(20);

    m_left_bwl_ransac.setIdealAngle(-30);
    m_left_bwl_ransac.setAllowedAngleVariance(10);
    m_left_bwl_ransac.setDistanceThreshold(2.0);
    m_left_bwl_ransac.setIterations(50);
    m_left_bwl_ransac.setMinVotes(20);

    m_right_bwl_ransac.setIdealAngle(30);
    m_right_bwl_ransac.setAllowedAngleVariance(10);
    m_right_bwl_ransac.setDistanceThreshold(2.0);
    m_right_bwl_ransac.setIterations(50);
    m_right_bwl_ransac.setMinVotes(20);



    qDebug()<<"Max::Max() Max Object Created.";
}

Max::~Max()
{
    delete m_scan_ds;
    delete m_scan_distro;
    delete m_skeletal_ds;
    delete m_roughing_left_tsl_ds;
    delete m_roughing_right_tsl_ds;
    delete m_left_tsl_inliers_ds;
    delete m_left_tsl_distro;
    delete m_right_tsl_inliers_ds;
    delete m_right_tsl_distro;
    delete m_joint_ds;
    delete m_left_bwl_inlier_ds;
    delete  m_right_bwl_inlier_ds;
    delete m_dummy_set1;
    delete m_dummy_set2;
    emit this->aboutToDestroy();
    qDebug()<<"Max::~Max() Max Object Destroyed";
}

//Processing Methods------------------------------------------------------------------


//PreProcessing Methods--------------------------------------------------
void Max::blankProcessingArrays()
{


    m_left_tsl.clear();
    m_right_tsl.clear();
    m_right_bwl.clear();
    m_left_bwl.clear();
    m_flattened_iohrv = 0;
    m_total_image_intensity = 0;

    m_scan_ds->clear();

}

int Max::randomInt(int _min, int _max)
{
    //C++ 11 Random Number Engine. #include <random>
    std::random_device seeder; //the random device that will seed the generator
    std::mt19937 engine(seeder()); //then make a mersenne twister engine
    std::uniform_int_distribution<int> dist(_min, _max); //then the easy part... the distribution
    return dist(engine); //Return the Number From The Distrobution
}

bool Max::clusterProcessThresholdMat(Mat &_src, Mat &_dst, quint64 *_tii, const quint64 _max_tii,
                                     const quint64 _min_tii, const int _min_cluster_size,
                                     const int _max_cluster_size, const int _max_clusters_in_column, TBIDataSet &_scandataset)
{

    if(_src.type() != CV_8UC1)
    {
        emit failedSkeletonizeMat();
        return false;
    }

    _scandataset.clear();
    *_tii = 0;

    int _x = 0;
    int _y = 0;
    uint8_t _value;
    int _clusterindex;
    PixelClusterClass _cluster;

    //Mat_Index = ((Y) * Mat.cols) + X
    uint8_t *_srcdata = _src.data;
    uint8_t *_dstdata = _dst.data;

    int _srcindex = 0;
    int _dstindex = 0;

    //Iterate Thru The Src Mat And Find Clusters
    do
    {
        _srcindex = (_y * _src.cols) + _x;
        _value = _srcdata[_srcindex];
        if(_value > 0)
        {
            *_tii += _value;
            if(*_tii > _max_tii)
            {
                emit failedTIICheck();
                return false;
            }
            PixelFundamental_t _pixel(_y, _x, _value);
            _cluster.pushPixelToBack(_pixel);
        }
        else if(_value == 0)
        {
            int _cluster_size = _cluster.size();
            if(_cluster_size > 0) //The Cluster Has Pixels in it.
            {
                if((_cluster_size >= _min_cluster_size) && (_cluster_size <= _max_cluster_size)) //The Cluster Needs a Centroid
                {
                    _cluster.setRowCentroid();
                    _dstindex = ((int)_cluster.getRowCentroid() * _dst.cols) + _x;
                    _dstdata[_dstindex] = 255;
                    TBIPoint_Int ___pnt(_x, (int)_cluster.getRowCentroid());
                    _scandataset.insert(___pnt);
                }
                else //The Cluster Is Not Centroid Put Them all in the _dst mat
                {
                    _clusterindex = 0;
                    do
                    {
                        _dstindex = (_cluster[_clusterindex].row * _dst.cols) + _cluster[_clusterindex].col;
                        _dstdata[_dstindex] = 255;
                        TBIPoint_Int ___pnt(_cluster[_clusterindex].col, _cluster[_clusterindex].row);
                        _scandataset.insert(___pnt);
                        ++_clusterindex;
                    }while(_clusterindex < _cluster.size());
                }
            }
            _cluster.clear();
        }
        ++_y;
        if(_y == _src.rows)
        {

            int _cluster_size = _cluster.size();
            if(_cluster_size > 0) //The Cluster Has Pixels in it.
            {
                if((_cluster_size >= _min_cluster_size) && (_cluster_size <= _max_cluster_size)) //The Cluster Needs a Centroid
                {
                    _cluster.setRowCentroid();
                    _dstindex = ((int)_cluster.getRowCentroid() * _dst.cols) + _x;
                    _dstdata[_dstindex] = 255;
                    TBIPoint_Int ___pnt(_x, (int)_cluster.getRowCentroid());
                    _scandataset.insert(___pnt);
                }
                else //The Cluster Is Not Centroid Put Them all in the _dst mat
                {
                    _clusterindex = 0;
                    do
                    {
                        _dstindex = (_cluster[_clusterindex].row * _dst.cols) + _cluster[_clusterindex].col;
                        _dstdata[_dstindex] = 255;
                        TBIPoint_Int ___pnt(_cluster[_clusterindex].col, _cluster[_clusterindex].row);
                        _scandataset.insert(___pnt);
                        ++_clusterindex;
                    }while(_clusterindex < _cluster.size());
                }
            }
            _cluster.clear();
            _y = 0;
            ++_x;
        }

    }while(_x < _src.cols);

    if(*_tii < _min_tii)
    {
        emit failedTIICheck();
        return false;
    }



    return true;
}


//The Recieve New CV::Mat Method
//This Slot Handles A New Camera Frame
void Max::recieveNewCVMat(const Mat &_mat)
{


    if(m_in_proccesing_loop) return;
    m_in_proccesing_loop = true;

    //Start The Time in Loop Timer
    m_timer.start();

    //Make Sure Mat is Ok.
    if(_mat.channels() != 1)
    {
        qDebug()<<"Max::recieveNewCVMat() did not recieve a single Channel Mat";
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }
    if(!_mat.isContinuous())
    {
        qDebug()<<"Max::recieveNewCVMat() did not recieve a continuous Mat";
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }

    //Blank Internals
    blankProcessingArrays();

    //Declare Mats
    cv::Mat _raw_mat = _mat.clone();
    cv::Mat _blurr_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _threshold_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _pixelcolumn_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _skel_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _ransac_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3); //Mats From Here Out are Color.
    cv::Mat _dataset_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);
    cv::Mat _operation_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);

    //Do OpenCV Proccesses
    cv::GaussianBlur(_raw_mat, _blurr_mat, Size(m_blur,m_blur), 0);
    cv::threshold(_blurr_mat, _threshold_mat, m_thresholdmin, m_thresholdmax, THRESH_TOZERO);
    cv::cvtColor(_raw_mat, _operation_mat, cv::COLOR_GRAY2BGR);
    blankProcessingArrays();


    //Create DataSets and Perfom Geometric Construction.
    if(clusterProcessThresholdMat(_threshold_mat, _pixelcolumn_mat, &m_total_image_intensity, m_max_image_intensity,
                                  m_min_image_intensity, m_min_cluster_size, m_max_cluster_size, m_max_clusterincol, *m_scan_ds))
    {


        //Build The Main Scan's Data Distribution Set
        m_scan_ds->buildDistributionSet(*m_scan_distro);
        //-----------------------------------------------------------------------------------
        //Build Skeletal Data Set
        m_scan_ds->buildSkeletalDataSet(*m_skeletal_ds, *m_scan_distro);
        m_skeletal_ds->drawToMat(_skel_mat);
        cv::cvtColor(_skel_mat, _ransac_mat, cv::COLOR_GRAY2BGR);
        //-----------------------------------------------------------------------------------
        //Build The Roughing TSL's DataSets and Do Roughing Ransacs
        int _breakingindex = m_skeletal_ds->getIndexofHighestY();
        m_skeletal_ds->buildDataSetForRoughingLeftTSL(*m_roughing_left_tsl_ds, _breakingindex);
        m_skeletal_ds->buildDataSetForRoughingRightTSL(*m_roughing_right_tsl_ds, _breakingindex);
        TBIRansac::doRansac(m_left_tsl, m_left_tsl_ransac, *m_roughing_left_tsl_ds);
        TBIRansac::doRansac(m_right_tsl, m_right_tsl_ransac, *m_roughing_right_tsl_ds);
        if(m_left_tsl.isValid())
        {
            m_left_tsl.remakeLine(0, _raw_mat.cols);
            m_left_tsl.drawOnMat(_ransac_mat, CV_RGB(0,255,0));
        }
        if(m_right_tsl.isValid())
        {
            m_right_tsl.remakeLine(0, _raw_mat.cols);
            m_right_tsl.drawOnMat(_ransac_mat, CV_RGB(255,0,0));
        }
        //-----------------------------------------------------------------------------------
        //Build The Inlier TSL Data Sets
        //The Data Set is Refined First. Then The Least Squares Method is used.
        if(m_left_tsl.isValid())
        {
            m_roughing_left_tsl_ds->buildDataSetForInliers(*m_left_tsl_inliers_ds, m_left_tsl, 1.0);
            m_left_tsl_inliers_ds->buildDistributionSet(*m_left_tsl_distro);
            m_left_tsl_inliers_ds->buildLeastSquareLine(m_left_tsl);
            if(m_left_tsl.isValid())
            {
                m_left_tsl.remakeLine(0, _raw_mat.cols);
                //m_left_tsl.drawOnMat(_dataset_mat, CV_RGB(0,255,0));
                m_left_tsl_inliers_ds->drawToMat(_dataset_mat, CV_RGB(125,255,125));
            }
        }
        if(m_right_tsl.isValid())
        {
            m_roughing_right_tsl_ds->buildDataSetForInliers(*m_right_tsl_inliers_ds, m_right_tsl, 1.0);
            m_right_tsl_inliers_ds->buildDistributionSet(*m_right_tsl_distro);
            m_right_tsl_inliers_ds->buildLeastSquareLine(m_right_tsl);
            if(m_right_tsl.isValid())
            {

                m_right_tsl.remakeLine(0, _raw_mat.cols);
                //m_right_tsl.drawOnMat(_dataset_mat, CV_RGB(255, 0, 0));
                m_right_tsl_inliers_ds->drawToMat(_dataset_mat, CV_RGB(255, 125, 125));
            }
        }


        //-----------------------------------------------------------------------------------
        //Having The Refined TSL's Build the Joint Datasets From the Skeletal DataSet
        if(m_left_tsl.isValid() && m_right_tsl.isValid())
        {
            m_skeletal_ds->buildDataSetForJoint(*m_joint_ds, m_right_tsl, m_left_tsl, 3.0);
            m_joint_ds->drawToMat(_dataset_mat, CV_RGB(50,50,255));

            //Find Tracking Point By The Masking Method

            TBIPoint_Int _pnt1lefttsl(m_left_tsl_inliers_ds->getPoint(m_left_tsl_inliers_ds->getIndexofHighestX()));
            TBIPoint_Int _pnt2righttsl(m_right_tsl_inliers_ds->getPoint(m_right_tsl_inliers_ds->getIndexofLowestX()));
            TBIPoint_Int _pnt1joint(m_joint_ds->getLowestX(), m_left_tsl.getYatX(m_joint_ds->getLowestX()));
            TBIPoint_Int _pnt2joint(m_joint_ds->getHighestX(), m_right_tsl.getYatX(m_joint_ds->getHighestX()));


            cv::drawMarker(_operation_mat, _pnt1joint.toCVPoint(), CV_RGB(0,0,255), MARKER_CROSS, 12, 1, LINE_AA);
            cv::drawMarker(_operation_mat, _pnt2joint.toCVPoint(), CV_RGB(0,0,255), MARKER_CROSS, 12, 1, LINE_AA);

            cv::drawMarker(_operation_mat, _pnt1lefttsl.toCVPoint(), CV_RGB(255,0,0), MARKER_CROSS, 12, 1, LINE_AA);
            cv::drawMarker(_operation_mat, _pnt2righttsl.toCVPoint(), CV_RGB(255,0,0), MARKER_CROSS, 12, 1, LINE_AA);

            /*
            //Ransac The Joint DataSet
            TBIRansac::doRansac(m_left_bwl, m_left_bwl_ransac, *m_joint_ds);
            TBIRansac::doRansac(m_right_bwl, m_right_bwl_ransac, *m_joint_ds);

            if(m_left_bwl.isValid())
            {
                m_left_bwl.remakeLine(0, _raw_mat.cols);
                m_left_bwl.drawOnMat(_ransac_mat, CV_RGB(0,255,0));
            }
            if(m_right_bwl.isValid())
            {
                m_right_bwl.remakeLine(0, _raw_mat.cols);
                m_right_bwl.drawOnMat(_ransac_mat, CV_RGB(255,0,0));
            }*/
        }

       /*

        //-----------------------------------------------------------------------------------
        //Refine the Left and Right BWL
        if(m_left_bwl.isValid())
        {
            m_joint_ds->buildDataSetForInliers(*m_left_bwl_inlier_ds, m_left_bwl, 1.0);
            m_left_bwl_inlier_ds->buildLeastSquareLine(m_left_bwl);
            m_left_bwl_inlier_ds->drawToMat(_dataset_mat, CV_RGB(255,155,255));
            if(m_left_bwl.isValid())
            {
                m_left_bwl.drawOnMat(_dataset_mat, CV_RGB(255,155,255));
            }

        }
        if(m_right_bwl.isValid())
        {
             m_joint_ds->buildDataSetForInliers(*m_right_bwl_inlier_ds, m_right_bwl,1.0);
             m_right_bwl_inlier_ds->buildLeastSquareLine(m_right_bwl);
             m_right_bwl_inlier_ds->drawToMat(_dataset_mat, CV_RGB(255,255,100));
             if(m_right_bwl.isValid())
             {
                 m_right_bwl.drawOnMat(_dataset_mat, CV_RGB(100,255,255));
             }
        }
        //-----------------------------------------------------------------------------------
        //Build Tracking Points If Refined TSL's and BWL's Have Been Found
        if(m_left_tsl.isValid() && m_right_tsl.isValid() && m_left_bwl.isValid() && m_right_bwl.isValid())
        {
            TBIPoint_Float _intersectleftpnt;
            TBIPoint_Float _intersectjointpnt;
            TBIPoint_Float _intersectrightpnt;
            m_left_tsl.findPointofIntersection(m_left_bwl, _intersectleftpnt);
            m_left_bwl.findPointofIntersection(m_right_bwl, _intersectjointpnt);
            m_right_tsl.findPointofIntersection(m_right_bwl, _intersectrightpnt);
            cv::drawMarker(_operation_mat, _intersectjointpnt.toCVPoint(), CV_RGB(0,0,255), MARKER_CROSS, 12, 1, LINE_AA);
            cv::drawMarker(_operation_mat, _intersectleftpnt.toCVPoint(), CV_RGB(0,0,255), MARKER_CROSS, 12, 1, LINE_AA);
            cv::drawMarker(_operation_mat, _intersectrightpnt.toCVPoint(), CV_RGB(0,0,255), MARKER_CROSS, 12, 1, LINE_AA);

        }
*/


    }

    //Emit Mat Signals If Turned On
    if(m_emitextramats)
    {
        emit newRawMatProcessed(_raw_mat);
        emit newBlurMatProcessed(_blurr_mat);
        emit newThresholdMatProcessed(_threshold_mat);
        emit newPixelColumnMatProcessed(_pixelcolumn_mat);
        emit newSkeletalMatProcessed(_skel_mat);
        emit newRansacMatProcessed(_ransac_mat);
        emit newDataSetMatProcessed(_dataset_mat);
    }

    //Emit The New Operations Screen Mat
    emit newOperationMatProcessed(_operation_mat);

    m_timeinloop = m_timer.elapsed();
    qDebug() << "Time in Loop = " << m_timer.elapsed();
    emit timeInLoopChanged(m_timeinloop);
    emit totalImageIntensityChanged(m_total_image_intensity);
    m_in_proccesing_loop = false;

    emit processingComplete(); //Must Be Last Signal Sent

}


// Slot Handling Methods To Update Settings-------------------------------------------
void Max::onBlurChange(int _blur)
{
    m_blur = _blur;
}

void Max::onThresholdMinChange(int _min)
{
    m_thresholdmin = _min;
}

void Max::onThresholdMaxChange(int _max)
{
    m_thresholdmax = _max;
}

void Max::onMaxTIIChange(quint64 _tii)
{
    m_max_image_intensity = _tii;
}

void Max::onMinTIIChange(quint64 _tii)
{
    m_min_image_intensity = _tii;
}

void Max::onMaxClusterSizeChange(int _size)
{
    m_max_cluster_size = _size;
}

void Max::onMinClusterSizeChange(int _size)
{
    m_min_cluster_size = _size;
}

void Max::onMaxClustersInColChange(int _size)
{
    m_max_clusterincol = _size;
}

void Max::onMaxDiscontinuityChange(int _value)
{
    m_allowable_discontinuities = _value;
}









