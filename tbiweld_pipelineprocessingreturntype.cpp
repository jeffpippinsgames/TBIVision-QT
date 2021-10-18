#include "tbiweld_pipelineprocessingreturntype.h"


TBIWeld_ProcessingPipeLineReturnType::TBIWeld_ProcessingPipeLineReturnType(QObject *parent) : QObject(parent)
{
    this->setStatus(TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_GENERAL_FAILURE);
}



void TBIWeld_ProcessingPipeLineReturnType::drawStatusStringToMat(cv::Mat &_dst)
{
    if(_dst.channels() != 3) return;
    cv::Scalar _color(50, 50, 255);

    if(m_status == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK) _color = cv::Scalar(50,255,50);
    cv::putText(_dst, m_statusstring.toStdString(), cv::Point(20, _dst.rows - 30), cv::FONT_HERSHEY_PLAIN, 2, _color, 1, cv::LINE_AA, false);
}

void TBIWeld_ProcessingPipeLineReturnType::setStatusString()
{

    /*
TBI_PIPELINE_OK = 0x01,
TBI_PIPELINE_GENERAL_FAILURE = 0x02,
TBI_PIPELINE_RAWMATCHANNELNOTEQUALTOONE = 0x03,
TBI_PIPELINE_RAWMATCHANNELNOTCONTINOUS = 0x04,
TBI_PIPELINE_FAILEDTOBUILDGAUSIANDECLUSTERDATASET = 0x05,
TBI_PIPELINE_TOTALIMAGEINTENSITYTOHIGH = 0x06,
TBI_PIPELINE_TOTALIMAGEINTENSITYTOLOW = 0x07,
TBI_PIPELINE_FAILEDTOBUILDDECLUSTERDISTROBUTIONSET = 0x08,
TBI_PIPELINE_GAUSIANDISTROBUTIONDATASETEXCEEDSALLOWEDSTANDARDDEVIATION = 0x09,
TBI_PIPELINE_COULDNOTFINDVALIDBREAKINDEXFORVGROOVEDATASET = 0x10,
TBI_PIPELINE_FAILEDTOGETGAUSIANDECLUSTERSUBSET = 0x11,
TBI_PIPELINE_FAILEDTOBUILDVGROOVELEFTGAUSIANDECLUSTERDATASUBSET = 0x12,
TBI_PIPELINE_FAILEDTOBUILDVGROOVERIGHTGAUSIANDECLUSTERDATASUBSET = 0x13,
TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE = 0x14,
TBI_PIPELINE_FAILEDTOEXTRACTVGROOVEJOINTDATASET = 0x15,
TBI_PIPELINE_FAILEDTOEXTRACTLEFTTSLINLIERDATASET = 0x16,
TBI_PIPELINE_FAILEDTOEXTRACTRIGHTTSLINLIERDATASET = 0x17,
TBI_PIPELINE_FAILEDTOEXTRACTLEFTBWLINLIERDATASET = 0x18,
TBI_PIPELINE_FAILEDTOEXTRACTRIGHTBWLINLIERDATASET = 0x19,
TBI_PIPELINE_FAILEDTOBUILDLEFTTSLGEOMETRICENTITY = 0x20,
TBI_PIPELINE_FAILEDTOBUILDRIGHTTSLGEOMETRICENTITY = 0x21,
TBI_PIPELINE_FAILEDTOBUILDLEFTBWLGEOMETRICENTITY = 0x22,
TBI_PIPELINE_FAILEDTOBUILDRIGHTBWLGEOMETRICENTITY = 0x23,
TBI_PIPELINE_FAILEDTOEXTRACTTRACKINGPOINT = 0x24,
TBI_PIPELINE_FAILEDLASERPOWEROFF = 0x25,
TBI_PIPELINE_SELECTEDJOINTTYPENOTIMPLEMENTED = 0x26,
TBI_PIPELINE_SCANNEDVGROOVEJOINTFAILEDBOUNDRYCHECK = 0x27,
TBI_PIPELINE_FAILED_TO_EXTRACTINLIERDATASET = 0x29,
TBI_PIPELINE_FAILED_TO_EXTRACTOUTLIERDATASET = 0x30,
TBI_PIPELINE_FAILED_TO_EXTRACTVGROOVEBREAKINDEX = 0x31}

    case TBIWeld_ProcessingPipeLineReturnType::
        m_statusstring = "";
    break;

*/
    switch(m_status)
    {
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK: //0x01
        m_statusstring = "Pipeline - Ok";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_GENERAL_FAILURE: //0x02
        m_statusstring = "General Failure.";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTEQUALTOONE: //0x03
        m_statusstring = "Raw Mat Does Not Have 1 Channel";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTCONTINOUS: //0x04
        m_statusstring = "Raw Mat Not Continous";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDGAUSIANDECLUSTERDATASET: //0x05
        m_statusstring = "Gausian Decluster Data Set Could Not Be Built.";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_TOTALIMAGEINTENSITYTOHIGH: //0x06
        m_statusstring = "Total Image Intesity Too High";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_TOTALIMAGEINTENSITYTOLOW: //0x07
        m_statusstring = "Total Image Intensity Too Low";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDDECLUSTERDISTROBUTIONSET: //0x08
        m_statusstring = "Failed to Extract Gausian Decluster Distrobution Dataset";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_GAUSIANDISTROBUTIONDATASETEXCEEDSALLOWEDSTANDARDDEVIATION: //0x09
        m_statusstring = "Gausian Distributed Standard Deviation Too High";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_COULDNOTFINDVALIDBREAKINDEXFORVGROOVEDATASET: //0x10
        m_statusstring = "No Valid V Groove Break Point Found";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOGETGAUSIANDECLUSTERSUBSET: //0x11
        m_statusstring = "Could Not Extract Decluster Data Subset";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDVGROOVELEFTGAUSIANDECLUSTERDATASUBSET: //0x12
        m_statusstring = "Could not Extract Left Decluster Data Subset";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDVGROOVERIGHTGAUSIANDECLUSTERDATASUBSET: //0x13
        m_statusstring = "Could not Extract Right Decluster Data Subset";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE: //0x14
        m_statusstring = "Failed to Build Inlier Ransac";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTVGROOVEJOINTDATASET: //0x15
        m_statusstring = "Failed to Extract V Groove Joint Dataset";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTLEFTTSLINLIERDATASET: //0x16
        m_statusstring = "Failed to Extract Left TSL Inlier Dataset";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTRIGHTTSLINLIERDATASET: //0x17
        m_statusstring = "Failed to Extract Right TSL Inlier Dataset";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTLEFTBWLINLIERDATASET: //0x18
        m_statusstring = "Failed to Extract Left BWL Dataset";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTRIGHTBWLINLIERDATASET: //0x19
        m_statusstring = "Failed to Extract RIght BWL Dataset";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDLEFTTSLGEOMETRICENTITY: //0x20
        m_statusstring = "Failed to Extract Left TSL Geometric Entity";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDRIGHTTSLGEOMETRICENTITY: //0x21
        m_statusstring = "Failed to Extract Right TSL Geometric Entity";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDLEFTBWLGEOMETRICENTITY: //0x22
        m_statusstring = "Failed to Extract Left BWL Geometric Entity";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDRIGHTBWLGEOMETRICENTITY: //0x23
        m_statusstring = "Failed to Extract Right BWL Geometric Entity";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTTRACKINGPOINT: //0x24
        m_statusstring = "Failed to Extract Tracking Point";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDLASERPOWEROFF: //0x25
        m_statusstring = "Laser Is Not Powered On";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_SELECTEDJOINTTYPENOTIMPLEMENTED:
        m_statusstring = "Selected Weld Joint Process Not Implemented";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_SCANNEDVGROOVEJOINTFAILEDBOUNDRYCHECK:
        m_statusstring = "Scanned VGroove Failed Joint Definition Boundy Check";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_TRACKINGPOINTINSTABILITYERROR:
        m_statusstring = "Tracking Points Are Unstable.";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILED_TO_EXTRACTINLIERDATASET:
        m_statusstring = "Failed To Extract Inlier DataSet.";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILED_TO_EXTRACTOUTLIERDATASET:
        m_statusstring = "Failed to Extract Outlier DataSet.";
        break;
    case TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILED_TO_EXTRACTVGROOVEBREAKINDEX:
        m_statusstring = "Failed to Extract V Groove Break Index.";
        break;
    default:
        m_statusstring = "Unknown Return.";
        break;
    }
}


