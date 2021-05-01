#ifndef PIXELCOLUMN_H
#define PIXELCOLUMN_H

#include <QObject>
#include "pixelcluster.h"
#include <QList>

class PixelColumn : public QObject
{
    Q_OBJECT
public:
    explicit PixelColumn(QObject *parent = nullptr);
    QList<PixelCluster> m_column;

signals:

};

#endif // PIXELCOLUMN_H
