#ifndef EDGEDRAWING_H
#define EDGEDRAWING_H

#include <QImage>
#include <QVector2D>

#define DIR_VER 1
#define DIR_HOR 2

typedef QVector<QPoint> _EDGE;

class EdgeDrawing
{
public:
    EdgeDrawing();


    QVector<_EDGE> getEdgesFromImage(const QImage& img,QImage &gaussainImage, int gaussR=5, int sobelThreshold=30, int archorThreshold=20);
    QImage getEdgeImage();
    QImage m_orginalImage;
    QVector<QVector<float> > m_gradientImg;
    QVector<QVector<float> > m_directionImg;
    QVector<QPoint> m_archors;
    QVector<_EDGE>  m_edges;

    void releaseAll();
    QImage getGaussianBlurImage(const QImage &img, int r);
    QImage getGrayImage(const QImage &img);

    void getGradientAndDirectionMap(const QImage& imggray, int threshold=30);
    void getArchors(float threshold = 0);
    void getEdges();
    void searchFromArchor(int x,int y,QVector<QVector<bool> >& isEdge,_EDGE& edge);

    int rx(int x, int offset, int width);
    int ry(int y, int offset, int height);
    QVector<double> getGaussianWeights(int r);
};

#endif // EDGEDRAWING_H
