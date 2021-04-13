#ifndef QIMAGEQMLVIEWER_H
#define QIMAGEQMLVIEWER_H

#include <QObject>
#include <QQuickItem>
#include <QtQuick/QQuickPaintedItem>
#include <toby.h>
#include <QPainter>

class QImageQMLViewer: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool is_toby_attached READ getTobyAttached)

public:
    explicit QImageQMLViewer(QObject *parent = nullptr);
    ~QImageQMLViewer();
    static void declareQML();
    Q_INVOKABLE void setToby(Toby *_toby);
    bool getTobyAttached(){return m_is_toby_set;}

private:
    QImage m_frame;
    Toby* m_toby;
    bool m_is_toby_set;

    void paint(QPainter *painter) override;

signals:
    void newFrameGrabbed();
    void tobySet();

public slots:
    void newCameraFrameHandler(const QImage &_img);
};
#endif // QIMAGEQMLVIEWER_H
