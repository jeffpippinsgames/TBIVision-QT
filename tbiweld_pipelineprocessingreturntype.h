#ifndef TBIWELD_PIPELINEPROCESSINGRETURNTYPE_H
#define TBIWELD_PIPELINEPROCESSINGRETURNTYPE_H

#include <QObject>
#include <QQuickItem>

class TBIWeld_ProcessingPipeLineReturnType : public QObject
{
    Q_OBJECT
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
                               TBI_PIPELINE_FAILEDTOEXTRACTVGROOVEJOINTDATASET = 0x15};
    Q_ENUMS(PipelineReturnType_t)
    static void declareQML()
    {
        qmlRegisterType<TBIWeld_ProcessingPipeLineReturnType>("tbi.vision.components", 1, 0, "TBIWeld_ProcessingPipeLineReturnType");
    }
signals:

};

#endif // TBIWELD_PIPELINEPROCESSINGRETURNTYPE_H

