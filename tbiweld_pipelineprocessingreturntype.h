#ifndef TBIWELD_PIPELINEPROCESSINGRETURNTYPE_H
#define TBIWELD_PIPELINEPROCESSINGRETURNTYPE_H

#include <QObject>
#include <QQuickItem>
#include <QString>
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc.hpp"


class TBIWeld_ProcessingPipeLineReturnType : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int status READ getStatus NOTIFY statusChanged)
    Q_PROPERTY(QString statusString READ getStatusString NOTIFY statusStringChanged)

public:
    explicit TBIWeld_ProcessingPipeLineReturnType(QObject *parent = nullptr);
    enum PipelineReturnType_t {TBI_PIPELINE_OK = 0x01,
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
                               TBI_PIPELINE_TRACKINGPOINTINSTABILITYERROR = 0x28,
                               TBI_PIPELINE_FAILED_TO_EXTRACTINLIERDATASET = 0x29,
                               TBI_PIPELINE_FAILED_TO_EXTRACTOUTLIERDATASET = 0x30,
                               TBI_PIPELINE_FAILED_TO_EXTRACTVGROOVEBREAKINDEX = 0x31};

    Q_ENUMS(PipelineReturnType_t)
    static void declareQML()
    {
        qmlRegisterType<TBIWeld_ProcessingPipeLineReturnType>("tbi.vision.components", 1, 0, "TBIWeld_ProcessingPipeLineReturnType");
    }

    int getStatus(){return (int)m_status;}
    QString getStatusString(){return m_statusstring;}
    void setStatus(TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _status){m_status = _status; this->setStatusString(); emit statusChanged(); emit statusStringChanged();}
    void drawStatusStringToMat(cv::Mat &_dst);

signals:
    void statusChanged();
    void statusStringChanged();

private:
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t m_status;
    QString m_statusstring;

    void setStatusString();



};

#endif // TBIWELD_PIPELINEPROCESSINGRETURNTYPE_H

