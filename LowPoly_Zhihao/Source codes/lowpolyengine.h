#ifndef LOWPOLYENGINE_H
#define LOWPOLYENGINE_H

#include <QImage>
#include <QTime>
#include "delaunay.h"
#include "kdtree.h"
#include "edgedrawing.h"

extern float e_minTriangleSize; // sobel 取样的最小间隔
extern float e_gaussianBlurRadius;
extern float e_sobelThreshold;
extern float e_archorThreshold;
extern float e_poissanRadius;
extern bool  e_colorMode;  // true-average, false-center;

class LowPolyEngine: public QThread
{
    Q_OBJECT
public:
    LowPolyEngine();

    enum THREAD_MODE{
        _REDO_ALL,
        _REDO_DELAUNAY
    } m_threadMode;

    void setStartMode(THREAD_MODE mode);
    void setOriginImage(const QImage& img);
    void setEdgePts(const QVector<QPointF> edgepts);

    void returnResultToWidget(QImage& edgeImg, QImage& blurImg, QImage& lowpolyImg, QVector<QPointF>& edgepts, QVector<TRIANGLE> &tris, QVector<QRgb> &triColors);

    QImage m_orginImage;
    QImage m_blurImage;
    QImage m_edgeImage;
    QImage m_lowPolyImage;
    QImage m_processImage;

    QVector<QPointF> m_edgepts;
    QVector<TRIANGLE> m_triangles;
    QVector<QRgb> m_triColors;

    QImage renderCurLowPolyToImage(const QImage &img);
    QImage getLowPolyImage(const QImage& img, int minDist=10, int sobelThreshold=30, int archorThreshold=20, int gaussR=3, int poissonR=10);

protected:
    void run();

signals:
    void sig_edgePts(int count);
};

#endif // LOWPOLYENGINE_H
