#ifndef TBIPARAMETERCLASS_PYLONCAMERAPARAMS_H
#define TBIPARAMETERCLASS_PYLONCAMERAPARAMS_H

#include <QObject>
#include <QString>
#include <QQuickItem>
#include <QDataStream>
#include "tbicore_constants.h"

class TBIPylonCameraParamers : public QObject
{
    Q_OBJECT

    //Q_PROPERTY(int propertyname READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int maxHeight READ getMaxHeight WRITE setMaxHeight NOTIFY maxHeightChanged)
    Q_PROPERTY(int maxWidth READ getMaxWidth WRITE setMaxWidth NOTIFY maxWidthChanged)
    Q_PROPERTY(QString cameraName READ getCameraName WRITE setCameraName NOTIFY cameraNameChanged)
    Q_PROPERTY(int aoiWidth READ getAOIWidth WRITE setAOIWidth NOTIFY aoiWidthChanged)
    Q_PROPERTY(int aoiHeight READ getAOIHeight WRITE setAOIHeight NOTIFY aoiHeightChanged)
    Q_PROPERTY(double exposure READ getExposure WRITE setExposure NOTIFY exposureChanged)
    Q_PROPERTY(int gain READ getGain WRITE setGain NOTIFY gainChanged)
    Q_PROPERTY(bool connected READ getConnected NOTIFY connectionChanged)

public:
    explicit TBIPylonCameraParamers(QObject *parent = nullptr);


    //Get Methods
    int getMaxHeight(){return m_max_height;}
    int getMaxWidth(){return m_max_width;}
    QString getCameraName(){return m_camera_name;}
    int getAOIWidth(){return m_aoi_width;}
    int getAOIHeight(){return m_aoi_height;}
    double getExposure(){return m_exposure;}
    int getGain(){return m_gain;}
    bool getConnected(){return m_connected;}

    //Set Methods
    void setMaxHeight(int _height);
    void setMaxWidth(int _width);
    void setCameraName(QString _name);
    void setAOIWidth(int _width);
    void setAOIHeight(int _height);
    void setExposure(double _exposure);
    void setGain(int _gain);
    void setConnection(bool _connection);

    //Save File Methods
    void setDefautValues();
    void saveToFile(QDataStream &_filedatastream);
    void loadFromFile(QDataStream &_filedatastream);


private:

    int m_max_height;
    int m_max_width;
    QString m_camera_name;
    int m_aoi_width;
    int m_aoi_height;
    double m_exposure;
    int m_gain;
    bool m_connected;


signals:
    void maxHeightChanged();
    void maxWidthChanged();
    void cameraNameChanged();
    void aoiWidthChanged();
    void aoiHeightChanged();
    void exposureChanged();
    void gainChanged();
    void connectionChanged();
};

#endif // TBIPARAMETERCLASS_PYLONCAMERAPARAMS_H
