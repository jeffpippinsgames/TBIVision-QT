#ifndef TOBYQMLVIEWER_H
#define TOBYQMLVIEWER_H

#include <QObject>
#include <QQuickItem>
#include <QtQuick/QQuickPaintedItem>
#include <toby.h>

class TobyQMLViewer: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(Toby* toby WRITE setToby)
public:

    explicit TobyQMLViewer(QQuickItem *parent = 0);
    void setToby(Toby* _toby){m_toby = _toby;}

private:
    Toby* m_toby;


};
#endif // TOBYQMLVIEWER_H
