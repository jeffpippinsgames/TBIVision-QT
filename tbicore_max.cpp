#include "tbicore_max.h"
#include <QDebug>
#include "tbiclass_pixelfundamental.h"
#include "tbiparameterclass_ransacvotingstruct.h"
#include <QRandomGenerator>
#include <QThread>
#include <vector>
#include <random>
#include "tbiclass_pointint.h"
#include "tbiclass_ransac.h"
#include "tbiclass_threepointtrackingcontainer.h"


//Constructors and Destructor--------------------------------------------------------
Max::Max(QObject *parent) : QObject(parent)
{

    m_control_mode = GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE;
    m_gausian_decluster_ds = new TBIDataSet();
    m_gausian_decluster_distro = new TBIDataDistributionSet();
    m_left_ransac_tsg_ds = new TBIDataSet();
    m_left_inlier_tsg_ds = new TBIDataSet();
    m_right_ransac_tsg_ds = new TBIDataSet();
    m_right_inlier_tsg_ds = new TBIDataSet();
    m_left_ransac1_bwl_ds = new TBIDataSet();
    m_left_ransac2_bwl_ds = new TBIDataSet();
    m_left_inlier_bwl_ds = new TBIDataSet();
    m_right_ransac1_bwl_ds = new TBIDataSet();
    m_right_ransac2_bwl_ds = new TBIDataSet();
    m_right_inlier_bwl_ds = new TBIDataSet();
    m_joint_ds = new TBIDataSet();
    m_dummy_set1 = new TBIDataSet();
    m_dummy_set2 = new TBIDataSet();

    m_emitextramats = false;

    m_in_proccesing_loop = false;
    m_timeinloop = "Error:";
    m_blur = 3;
    m_thresholdmin = 100;
    m_thresholdmax = 255;

    m_image_intensity_parameters.m_max_tii = 720*540*255*.3;
    m_image_intensity_parameters.m_min_tii = 720*255;

    m_gausian_decluster_parameters.m_min_cluster_size = 1;
    m_gausian_decluster_parameters.m_max_cluster_size = 75;

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
    delete m_gausian_decluster_ds;
    delete m_gausian_decluster_distro;
    delete m_left_ransac_tsg_ds;
    delete m_left_inlier_tsg_ds;
    delete m_right_ransac_tsg_ds;
    delete m_right_inlier_tsg_ds;
    delete m_left_ransac1_bwl_ds;
    delete m_left_ransac2_bwl_ds;
    delete m_left_inlier_bwl_ds;
    delete m_right_ransac1_bwl_ds;
    delete m_right_ransac2_bwl_ds;
    delete m_right_inlier_bwl_ds;
    delete m_joint_ds;
    delete m_dummy_set1;
    delete m_dummy_set2;
    emit this->aboutToDestroy();
    qDebug()<<"Max::~Max() Max Object Destroyed";
}



//The Recieve New CV::Mat Method
//This Slot Handles A New Camera Frame


void Max::recieveNewCVMat(const Mat &_mat)
{
    if(m_in_proccesing_loop) return;
    m_in_proccesing_loop = true;
    m_timer.start();

    processVGrooveTracking(_mat);

    m_timeinloop = m_timer.elapsed();
    emit timeInLoopChanged(m_timeinloop);
    emit totalImageIntensityChanged(m_total_image_intensity);
    m_in_proccesing_loop = false;

    emit processingComplete(); //Must Be Last Signal Sent

}

void Max::processVGrooveTracking(const cv::Mat _mat)
{

    if(_mat.channels() != 1)
    {
        qDebug()<<"Max::processVGrooveTracking did not recieve a single Channel Mat";
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }
    if(!_mat.isContinuous())
    {
        qDebug()<<"Max::processVGrooveTracking() did not recieve a continuous Mat";
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }

    //Blank Internals
    //Clear Ransac Lines And Processing DataSets
    bool _dataconstructionok = true;
    m_ransac_left_tsl.clear();
    m_ransac_left_bwl.clear();
    m_ransac_right_tsl.clear();
    m_ransac_right_bwl.clear();
    m_gausian_decluster_ds->clear();
    m_gausian_decluster_distro->clear();
    m_left_ransac_tsg_ds->clear();
    m_left_inlier_tsg_ds->clear();
    m_left_ransac1_bwl_ds->clear();
    m_left_ransac2_bwl_ds->clear();
    m_left_inlier_bwl_ds->clear();
    m_right_ransac_tsg_ds->clear();
    m_right_inlier_tsg_ds->clear();
    m_right_ransac1_bwl_ds->clear();
    m_right_ransac2_bwl_ds->clear();
    m_right_inlier_bwl_ds->clear();
    m_joint_ds->clear();

    //Declare Mats
    cv::Mat _raw_mat = _mat.clone();
    cv::Mat _blurr_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _threshold_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _gausiandecluster_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _inliers_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3); //Mats From Here Out are Color.
    cv::Mat _ransac_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);
    cv::Mat _geometricconstruction_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);
    cv::Mat _operation_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);

    //Declare Ransac Parameters For Adusted Bevel Wall Lines
    TBIRansacParameter _tmpransacparm;
    TBIRansacParameter _leftbwl_adjustedransac;
    TBIRansacParameter _rightbwl_adjustedransac;

    //Declare DataSet Construction Variables
    int _breakindex = -1;
    TBILine _left_ts_inlierline;
    TBILine _right_ts_inlierline;

    //Do OpenCV Proccesses
    cv::GaussianBlur(_raw_mat, _blurr_mat, Size(m_blur,m_blur), 0);
    cv::threshold(_blurr_mat, _threshold_mat, m_thresholdmin, m_thresholdmax, THRESH_TOZERO);
    cv::cvtColor(_raw_mat, _operation_mat, cv::COLOR_GRAY2BGR);


    //Data Constuction Section-------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------

    //Build Gausian Decluster DataSet
    if(m_gausian_decluster_ds->buildGausianClusterDataSet(_threshold_mat, m_gausian_decluster_parameters) == TBIDataSetReturnType::Ok)
    {
        m_gausian_decluster_ds->drawToMat(_gausiandecluster_mat);
        cv::cvtColor(_gausiandecluster_mat, _ransac_mat, COLOR_GRAY2BGR);

        //Extract The Gausian Decluster DistrobutionSet
        if(m_gausian_decluster_ds->extractDistributionSet(*m_gausian_decluster_distro) == TBIDataSetReturnType::Ok)
        {
            //Find The BreakIndex
            _breakindex = m_gausian_decluster_ds->getIndexofHighestY(*m_gausian_decluster_distro);
            if(_breakindex != -1) //A Good Break Index Was Found. Continue Construction
            {
                cv::drawMarker(_ransac_mat, m_gausian_decluster_ds->getPoint(_breakindex).toCVPoint(), CV_RGB(255,125,125), MARKER_CROSS, 20);

                //Left TSG Ransac and Inlier DataSet Construction
                TBIRansac::doRansac(m_ransac_left_tsl, m_left_tsl_ransac, *m_gausian_decluster_ds, 0, _breakindex);
                if(m_ransac_left_tsl.isValid())
                {
                    m_ransac_left_tsl.remakeLine(0, _raw_mat.cols);
                    m_ransac_left_tsl.drawOnMat(_ransac_mat, CV_RGB(0,0,255));
                    //Build Inliers Set
                    if(m_gausian_decluster_ds->extractDataSetForInliers(*m_left_inlier_tsg_ds, m_ransac_left_tsl, 2.0, 0, _breakindex) == TBIDataSetReturnType::Ok)
                    {
                        m_left_inlier_tsg_ds->drawToMat(_inliers_mat, CV_RGB(125,125,255));
                    }
                    else
                    {
                        _dataconstructionok = false;
                    }
                }
                else
                {
                    _dataconstructionok = false;
                }

                //Right TSG Ransac and Inlier DataSet Construction
                TBIRansac::doRansac(m_ransac_right_tsl, m_right_tsl_ransac, *m_gausian_decluster_ds, _breakindex, m_gausian_decluster_ds->size()-1);
                if(m_ransac_right_tsl.isValid())
                {
                    m_ransac_right_tsl.remakeLine(0, _raw_mat.cols);
                    m_ransac_right_tsl.drawOnMat(_ransac_mat, CV_RGB(255,0,0));
                    //Build Inliers Set
                    if(m_gausian_decluster_ds->extractDataSetForInliers(*m_right_inlier_tsg_ds, m_ransac_right_tsl, 2.0, _breakindex, m_gausian_decluster_ds->size()-1) == TBIDataSetReturnType::Ok)
                    {
                        m_right_inlier_tsg_ds->drawToMat(_inliers_mat, CV_RGB(255,125,125));
                    }
                    else
                    {
                        _dataconstructionok = false;
                    }
                }
                else
                {
                    _dataconstructionok = false;
                }

                //Build Joint DS For Reduced Processsing Overhead For BWL Ransacs
                if(m_ransac_left_tsl.isValid() && m_ransac_right_tsl.isValid())
                {
                    if(m_gausian_decluster_ds->extractDataSetForJoint(*m_joint_ds, m_ransac_left_tsl, m_ransac_right_tsl, 5) == TBIDataSetReturnType::Ok)
                    {
                        m_joint_ds->drawToMat(_inliers_mat, CV_RGB(50,50,50));
                    }
                    else
                    {
                        _dataconstructionok = false;
                    }

                }
                else
                {
                    _dataconstructionok = false;
                }


                //Make Sure The Point in BreakIndex is inside the Joint Dataset
                //

                //Left BWL
                _leftbwl_adjustedransac.makeEqual(m_left_bwl_ransac);
                if(m_ransac_left_tsl.isValid())_leftbwl_adjustedransac.setIdealAngle(m_left_bwl_ransac.getIdealAngle() - m_ransac_left_tsl.angleFromHorizontal());
                TBIRansac::doRansac(m_ransac_left_bwl, _leftbwl_adjustedransac, *m_joint_ds);
                if(m_ransac_left_bwl.isValid())
                {
                    m_ransac_left_bwl.remakeLine(0, _raw_mat.cols);
                    m_ransac_left_bwl.drawOnMat(_ransac_mat, CV_RGB(75,75,255));
                    //Build Inliers Set
                    if(m_gausian_decluster_ds->extractDataSetForInliers(*m_left_inlier_bwl_ds, m_ransac_left_bwl, 5.0, 0, _breakindex) == TBIDataSetReturnType::Ok)
                    {
                         m_left_inlier_bwl_ds->drawToMat(_inliers_mat, CV_RGB(175,175,255));
                    }
                    else
                    {
                        _dataconstructionok = false;
                    }
                }
                else
                {
                    _dataconstructionok = false;
                }

                //Right BWL
                _rightbwl_adjustedransac.makeEqual(m_right_bwl_ransac);
                if(m_ransac_right_tsl.isValid())_rightbwl_adjustedransac.setIdealAngle(m_right_bwl_ransac.getIdealAngle() - m_ransac_right_tsl.angleFromHorizontal());
                TBIRansac::doRansac(m_ransac_right_bwl, _rightbwl_adjustedransac, *m_joint_ds);
                if(m_ransac_right_bwl.isValid())
                {
                    m_ransac_right_bwl.remakeLine(0, _raw_mat.cols);
                    m_ransac_right_bwl.drawOnMat(_ransac_mat, CV_RGB(75,255,75));
                    //Build Inliers Set
                    if(m_gausian_decluster_ds->extractDataSetForInliers(*m_right_inlier_bwl_ds, m_ransac_right_bwl, 5.0, _breakindex, m_gausian_decluster_ds->size()-1) == TBIDataSetReturnType::Ok)
                    {

                        m_right_inlier_bwl_ds->drawToMat(_inliers_mat, CV_RGB(175,255,175));
                    }
                    else
                    {
                        _dataconstructionok = false;
                    }
                }
                else
                {
                    _dataconstructionok = false;
                }
            }
            else
            {
                _dataconstructionok = false;
            }
        }
        else
        {
            _dataconstructionok = false;
        }
    }
    else
    {
        _dataconstructionok = false;
    }

    //Validate The Geometric Construction
    if(_dataconstructionok)
    {
        if(_breakindex == -1) _dataconstructionok = false;
        if(m_gausian_decluster_distro->standardDeviation() > 1.0) _dataconstructionok = false;

    }


    float _stddev = m_gausian_decluster_distro->standardDeviation();
    //qDebug() << "Gausian Decluster Distribution Standard Deviation = " << _stddev;

    //Geometric Construction For Joint Construction.
    if(_dataconstructionok)
    {
        m_left_inlier_tsg_ds->extractLeastSquareLine(m_geo_left_tsl);
        m_right_inlier_tsg_ds->extractLeastSquareLine(m_geo_right_tsl);
        m_left_inlier_bwl_ds->extractLeastSquareLine(m_geo_left_bwl);
        m_right_inlier_bwl_ds->extractLeastSquareLine(m_geo_right_bwl);

        if(m_geo_left_tsl.isValid())
        {
            m_geo_left_tsl.remakeLine(0, _raw_mat.cols);
            m_geo_left_tsl.drawOnMat(_geometricconstruction_mat, CV_RGB(0,255,0));
        }
        if(m_geo_right_tsl.isValid())
        {
            m_geo_right_tsl.remakeLine(0, _raw_mat.cols);
            m_geo_right_tsl.drawOnMat(_geometricconstruction_mat, CV_RGB(0,0,255));
        }
        if(m_geo_left_bwl.isValid())
        {
            m_geo_left_bwl.remakeLine(0, _raw_mat.cols);
            m_geo_left_bwl.drawOnMat(_geometricconstruction_mat, CV_RGB(125,255,125));
        }
        if(m_geo_right_bwl.isValid())
        {
            m_geo_right_bwl.remakeLine(0, _raw_mat.cols);
            m_geo_right_bwl.drawOnMat(_geometricconstruction_mat, CV_RGB(125,125,255));
        }

        //Draw Tracking Point
        TBIThreePointTrackingContainer _trackingpoint;
        _trackingpoint.buildFrom4Lines(m_geo_left_tsl, m_geo_right_tsl, m_geo_left_bwl, m_geo_right_bwl);
        if(_trackingpoint.isValid())
        {
            m_three_point_tracking_manager.insert(_trackingpoint);       
        }

        if(m_three_point_tracking_manager.m_tracking_point.isValid())
        {
            m_three_point_tracking_manager.m_tracking_point.drawToMat(_operation_mat, CV_RGB(0,255,255));
        }

    }



    //Emit Mat Signals If Turned On
    if(m_emitextramats)
    {
        emit newRawMatProcessed(_raw_mat);
        emit newBlurMatProcessed(_blurr_mat);
        emit newThresholdMatProcessed(_threshold_mat);
        emit newPixelColumnMatProcessed(_gausiandecluster_mat);
        emit newInlierDataSetMatProcessed(_inliers_mat);
        emit newRansacMatProcessed(_ransac_mat);
        emit newGeoConstructionMatProcessed(_geometricconstruction_mat);
    }

    //Emit The New Operations Screen Mat
    emit newOperationMatProcessed(_operation_mat);



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
    m_image_intensity_parameters.m_max_tii = _tii;
}

void Max::onMinTIIChange(quint64 _tii)
{
    m_image_intensity_parameters.m_min_tii = _tii;
}

void Max::onMaxClusterSizeChange(int _size)
{
    m_gausian_decluster_parameters.m_max_cluster_size = _size;
}

void Max::onMinClusterSizeChange(int _size)
{
    m_gausian_decluster_parameters.m_min_cluster_size = _size;
}

void Max::onMaxClustersInColChange(int _size)
{
    m_gausian_decluster_parameters.m_max_clusters_in_col = _size;
}

void Max::onMaxDiscontinuityChange(int _value)
{
    m_allowable_discontinuities = _value;
}









