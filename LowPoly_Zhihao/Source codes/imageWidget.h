#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QWheelEvent>
#include <QMouseEvent>
#include "delaunay.h"
#include "kdtree.h"
#include "edgedrawing.h"
#include "lowpolyengine.h"

#define RGB_EDGE  qRgb(255,255,255)
#define RGB_BLANK qRgb(0,0,0)

namespace Ui {
class ImageWidget;
}

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = 0);
    ~ImageWidget();

    enum MODE {
        _ORIGIN,
        _BLUR,
        _EDGE,
        _LOWPOLY,
        _PROCESS} m_mode;

    bool m_isPicked;
    bool m_isChangeOriginalImage;

    void setMode(MODE m);
    LowPolyEngine* lowPolyEngine() { return &m_lowPolyEngine; }

    QImage originImage()  const { return this->m_orginImage;}
    QImage edgeImage()    const {return this->m_edgeImage;}
    QImage lowPolyImage() const { return this->m_lowPolyImage;}

    void setOriginImage(const QString& path);
    void refreshFromCurImage();
    void refreshDelaunayFromCurImage();
    void threadProcessDone();

protected:
    void paintEvent(QPaintEvent* e);
    void wheelEvent(QWheelEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

private:
    Ui::ImageWidget *ui;

    LowPolyEngine m_lowPolyEngine;

    QImage m_orginImage;
    QImage m_blurImage;
    QImage m_edgeImage;
    QImage m_lowPolyImage;

    QVector<QPointF> m_edgepts;
    QVector<TRIANGLE> m_triangles;
    QVector<QRgb> m_triColors;

    // scale and move
    QPoint m_leftPos;
    double m_scale;
    QPoint m_lastPos;
    QPolygonF m_pickPoly;

};

#endif // IMAGEPROCESSOR_H
