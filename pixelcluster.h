#ifndef PIXELCLUSTER_H
#define PIXELCLUSTER_H

#include <QObject>
#include <QList>
#include <pixelfundamental.h>

class PixelCluster : public QObject
{
    Q_OBJECT
public:
    explicit PixelCluster(QObject *parent = nullptr);
    QList<PixelFundamental> m_cluster;
signals:

};

#endif // PIXELCLUSTER_H
