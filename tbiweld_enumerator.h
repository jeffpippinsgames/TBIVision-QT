#ifndef TBIWELD_ENUMERATOR_H
#define TBIWELD_ENUMERATOR_H

#include <QObject>
#include <QQuickItem>
#include <QString>

class TBIWeldType_Enumerator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int jointype READ getQMLJointType WRITE setQMLJointType NOTIFY jointTypeChanged)
    Q_PROPERTY(QString jointtypestring READ getQMLJointTypeString NOTIFY jointTypeStringChanged)

public:
    explicit TBIWeldType_Enumerator(QObject *parent = nullptr){this->setJointType(TBIWeldType_Enumerator::TBI_VGROOVE_WELD);}
    enum WeldType_t         {TBI_FILLET_WELD = 0x01,
                             TBI_VGROOVE_WELD = 0x02,
                             TBI_BUTTJOINT_WELD = 0x03,};

    Q_ENUMS(ControlErrorCode_t)
    static void declareQML()
    {
        qmlRegisterType<TBIWeldType_Enumerator>("tbi.vision.components", 1, 0, "TBIWeldType_Enumerator");
    }
    int getQMLJointType(){return (int)m_jointtype;}
    QString getQMLJointTypeString(){return m_jointnamestring;}

    void setQMLJointType(int _jointtype){m_jointtype = (TBIWeldType_Enumerator::WeldType_t)_jointtype; emit jointTypeChanged(); this->setJointString(this->m_jointtype);}

    TBIWeldType_Enumerator::WeldType_t getJointType(){return m_jointtype;}

    void setJointType(TBIWeldType_Enumerator::WeldType_t _type){m_jointtype = _type; emit jointTypeChanged(); this->setJointString(this->m_jointtype);}
    void setJointString(TBIWeldType_Enumerator::WeldType_t _weldtype)
    {
        switch(_weldtype)
        {
            case TBIWeldType_Enumerator::TBI_FILLET_WELD:
                m_jointnamestring = "Fillet Weld";
            break;
            case TBIWeldType_Enumerator::TBI_BUTTJOINT_WELD:
                m_jointnamestring = "Butt Weld";
            break;
            case TBIWeldType_Enumerator::TBI_VGROOVE_WELD:
                m_jointnamestring = "V-Groove Weld";
            break;
            default:
                m_jointnamestring = "Error in TBIWeldType_Enumertor: Unknown Weld Joint Type Set.";
            break;
        }
        emit this->jointTypeStringChanged();
    }
    Q_INVOKABLE void setJointToVGroove(){this->setJointType(TBIWeldType_Enumerator::TBI_VGROOVE_WELD);}
    Q_INVOKABLE void setJointToButt(){this->setJointType(TBIWeldType_Enumerator::TBI_BUTTJOINT_WELD);}
    Q_INVOKABLE void setJointToFillet(){this->setJointType(TBIWeldType_Enumerator::TBI_FILLET_WELD);}

private:
        TBIWeldType_Enumerator::WeldType_t m_jointtype;
        QString m_jointnamestring;
signals:
    void jointTypeChanged();
    void jointTypeStringChanged();

};

#endif // TBIWELD_ENUMERATOR_H
