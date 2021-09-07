#ifndef TBIWELD_ENUMERATOR_H
#define TBIWELD_ENUMERATOR_H

#include <QObject>
#include <QQuickItem>

class TBIWeldType_Enumerator : public QObject
{
    Q_OBJECT
public:
    explicit TBIWeldType_Enumerator(QObject *parent = nullptr);
    enum WeldType_t         {TBI_FILLET_WELD = 0x01,
                             TBI_VGROOVE_WELD = 0x02,
                             TBI_BUTTJOINT_WELD = 0x03,};

    enum WeldProcessingReturnType_t {TBI_WPRT_OK = 0x01,
                                     TBI_WPRT_FAILED_GENERAL = 0x02};

    Q_ENUMS(ControlErrorCode_t)
    static void declareQML()
    {
        qmlRegisterType<TBIWeldType_Enumerator>("tbi.vision.components", 1, 0, "TBIWeldType_Enumerator");
    }

signals:

};

#endif // TBIWELD_ENUMERATOR_H
