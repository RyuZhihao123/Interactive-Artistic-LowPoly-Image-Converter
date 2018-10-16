/*
 *    This program is written by RyuZhihao123(Liu Zhihao), who is now a first year graduate student in
 * University of Chinese Academy of Sciences(UCAS).
 *
 *    The algorithm used refers to the paper - 'Edge Drawing:A combined real-time edge and segment detector'
 * proposed by 'Cihan Topal' etc.
 *
 *   RyuZhihao123 2018/10/16
*/

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

    /* Function 'getEdgesFromImage()',
     * Call this function to get all edges within a vector.
     *   (1) Return: A vector of edges. Each edge is described as a vector<QPoint>.
     *   (2) Paramters:
     *       - img     : the original image you input.
     *       - gaussR  : the distance of gaussian blur.
     *       - sobelThreshold: the threshold of sobel filter
     *       - archorThreshold: the threshold to determine archors.
   */
    QVector<_EDGE> getEdgesFromImage(const QImage& img,int gaussR=5,int sobelThreshold=30,int archorThreshold=60);

    /*
     * Function 'getEdgeImage()'
     * Call this function to get the egde image from image 'img'.
     *    (1) Return: An Edge Image(QImage)
     *    (2) Paramters:
     *        As mentioned above.
    */
    QImage getEdgeImage(const QImage &img, int gaussR=5,int sobelThreshold=30,int archorThreshold=60);

    void releaseAll();

private:
    QImage m_orginalImage;
    QVector<QVector<float> > m_gradientImg;
    QVector<QVector<float> > m_directionImg;
    QVector<QPoint> m_archors;
    QVector<_EDGE>  m_edges;

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
