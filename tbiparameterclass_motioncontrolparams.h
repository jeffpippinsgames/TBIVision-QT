#ifndef TBIPARAMETERCLASS_MOTIONCONTROLPARAMS_H
#define TBIPARAMETERCLASS_MOTIONCONTROLPARAMS_H

#include <QObject>
#include <QDataStream>

class TBIMotionControlParameters : public QObject
{
    Q_OBJECT

    //Q_PROPERTY(int propertyname READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(float xStepsPerPixel READ getXStepsPerPixel WRITE setXStepsPerPixel NOTIFY xStepsPerPixelChanged)
    Q_PROPERTY(float zStepsPerPixel READ getZStepsPerPixel WRITE setZStepsPerPixel NOTIFY zStepsPerPixelChanged)

public:
    explicit TBIMotionControlParameters(QObject *parent = nullptr);

    //Get Methods
    float getXStepsPerPixel(){return m_x_steps_per_pixel;}
    float getZStepsPerPixel(){return m_z_steps_per_pixel;}

    //Set Methods
    void setXStepsPerPixel(float _value);
    void setZStepsPerPixel(float _value);

    //Save File Methods
    void setDefautValues();
    void saveToFile(QDataStream &_filedatastream);
    void loadFromFile(QDataStream &_filedatastream);

private:
    float m_x_steps_per_pixel;
    float m_z_steps_per_pixel;

signals:
    void xStepsPerPixelChanged();
    void zStepsPerPixelChanged();

};

#endif // TBIPARAMETERCLASS_MOTIONCONTROLPARAMS_H
