#include "edgedrawing.h"
#include <QDebug>

EdgeDrawing::EdgeDrawing()
{

}

QVector<_EDGE> EdgeDrawing::getEdgesFromImage(const QImage &img, QImage& gaussainImage, int gaussR, int sobelThreshold, int archorThreshold)
{
    this->m_orginalImage = img;
    gaussainImage = this->getGaussianBlurImage(this->m_orginalImage,gaussR);
    this->getGradientAndDirectionMap(this->getGrayImage(gaussainImage),sobelThreshold);
    qDebug()<<"[1]DONE: Gradient And direction";
    this->getArchors(archorThreshold);
    qDebug()<<"[2]DONE: Archors"<<this->m_archors.size();
    this->getEdges();
    qDebug()<<"[3]DONE: Edges"<<this->m_edges.size();

    return this->m_edges;
}

QImage EdgeDrawing::getEdgeImage()
{
    static QRgb colors[8] = {qRgb(255,255,255),
                            qRgb(255,0,255),
                            qRgb(255,255,0),
                            qRgb(0,255,255),
                            qRgb(255,155,155),
                            qRgb(155,255,155),
                            qRgb(155,155,255),
                            qRgb(55,255,55)};

    if(this->m_orginalImage.isNull())
        return QImage();

    QImage img = this->m_orginalImage;
    img.fill(QColor(0,0,0));

    for(unsigned int i=0; i<this->m_edges.size(); i++)
        for(unsigned int k=0;k<this->m_edges[i].size();k++)
            img.setPixel(this->m_edges[i][k],colors[i%8]);

    return img;
}

void EdgeDrawing::releaseAll()
{
    this->m_archors.clear();

    for(unsigned int i=0; i<this->m_directionImg.size(); i++)
        this->m_directionImg[i].clear();
    this->m_directionImg.clear();

    for(unsigned int i=0; i<this->m_gradientImg.size(); i++)
        this->m_gradientImg[i].clear();
    this->m_gradientImg.clear();

    for(unsigned int i=0; i<this->m_edges.size(); i++)
        this->m_edges[i].clear();
    this->m_edges.clear();
}

void EdgeDrawing::getGradientAndDirectionMap(const QImage &imggray, int threshold)
{
    this->releaseAll(); // release storage.

    // initialize
    QVector<QVector<float> > iMap;  // intensity map(float)
    iMap.resize(imggray.width());

    m_gradientImg.resize(imggray.width());
    m_directionImg.resize(imggray.width());

    for(unsigned int x=0; x<imggray.width(); x++)
    {
        m_gradientImg[x].fill(0,imggray.height());
        m_directionImg[x].fill(0,imggray.height());
        iMap[x].resize(imggray.height());

        for(unsigned int y=0; y<imggray.height(); y++)
            iMap[x][y] = QColor(imggray.pixel(x,y)).red();
    }
    // gradient & direction map
    const int w = imggray.width();
    const int h = imggray.height();

    for(unsigned int y=0; y<imggray.height(); y++)
    {
        for(unsigned int x=0; x<imggray.width(); x++)
        {
            double gx = 1*iMap[rx(x,-1,w)][ry(y,-1,h)] - 1*iMap[rx(x,1,w)][ry(y,-1,h)]
                      + 2*iMap[rx(x,-1,w)][ry(y, 0,h)] - 2*iMap[rx(x,1,w)][ry(y,0,h)]
                      + 1*iMap[rx(x,-1,w)][ry(y, 1,h)] - 1*iMap[rx(x,1,w)][ry(y,1,h)];

            double gy = 1*iMap[rx(x,-1,w)][ry(y,-1,h)] + 2*iMap[rx(x,0,w)][ry(y,-1,h)] +  1*iMap[rx(x,1,w)][ry(y,-1,h)]
                      - 1*iMap[rx(x,-1,w)][ry(y, 1,h)] - 2*iMap[rx(x,0,w)][ry(y, 1,h)] -  1*iMap[rx(x,1,w)][ry(y,1,h)];


            gx = fabs(gx);
            gy = fabs(gy);

            if(gx+gy > threshold)
                m_gradientImg[x][y] = gx+gy;



        }
    }
    QImage tmp1(w,h,QImage::Format_RGB32);
    QImage tmp2(w,h,QImage::Format_RGB32);

    for(unsigned int x=0; x<w; x++)
    {
        for(unsigned int y=0; y<h; y++)
        {
            if(m_directionImg[x][y] == DIR_VER)
                tmp1.setPixel(x,y,qRgb(255,100,100));
            if(m_directionImg[x][y] == DIR_HOR)
                tmp1.setPixel(x,y,qRgb(100,50,255));
            if(m_directionImg[x][y] == -1)
                tmp1.setPixel(x,y,qRgb(255,255,255));

            //qDebug()<<x<<y<<m_gradientImg[x][y];

            tmp2.setPixel(x,y,qRgb(m_gradientImg[x][y],m_gradientImg[x][y],m_gradientImg[x][y]));
        }
    }
    tmp1.save("C:/Users/LiewDennis/Desktop/dir.bmp");
    tmp2.save("C:/Users/LiewDennis/Desktop/g.bmp");
}

void EdgeDrawing::getArchors(float threshold)
{
    const int w = this->m_orginalImage.width();
    const int h = this->m_orginalImage.height();

    for(unsigned int x=1; x<w-1; x++)
    {
        for(unsigned int y=1; y<h-1; y++)
        {
            if(m_directionImg[x][y] == DIR_HOR)
            {
                if((m_gradientImg[x][y]-m_gradientImg[x][y-1])>= threshold
                        && (m_gradientImg[x][y]-m_gradientImg[x][y+1])>=threshold)
                    this->m_archors.push_back(QPoint(x,y));
            }
            else if(m_directionImg[x][y] == DIR_VER)
            {
                if((m_gradientImg[x][y]-m_gradientImg[x-1][y])>=threshold
                        && (m_gradientImg[x][y]-m_gradientImg[x+1][y])>=threshold)
                    this->m_archors.push_back(QPoint(x,y));
            }
        }
    }
    // FOR TEST:
//    QImage img = this->m_orginalImage;
//    img.fill(QColor(0,0,0));
//    for(unsigned int i=0; i<this->m_archors.size(); i++)
//            img.setPixel(this->m_archors[i],qRgb(255,255,255));
//    img.save("C:/Users/LiewDennis/Desktop/ttttttt1.jpg");
}

void EdgeDrawing::getEdges()
{
    const int w = this->m_orginalImage.width();
    const int h = this->m_orginalImage.height();

    QVector<QVector<bool> > isEdge(w);
    for(unsigned int i=0; i<isEdge.size(); i++)
        isEdge[i].fill(false,h);

    // start to find all edges with each archor
    for(unsigned int i=0; i<m_archors.size(); i++)
    {
        _EDGE edge; // a new edge start from m_archor[i];

        int x = m_archors[i].x();
        int y = m_archors[i].y();
        searchFromArchor(x,y,isEdge,edge);

        if(edge.size() != 0)
            this->m_edges.push_back(edge);
    }
}

void EdgeDrawing::searchFromArchor(int x, int y, QVector<QVector<bool> > &isEdge, _EDGE &edge)
{
    if(x-1<0 || y-1<0 || x+1>=this->m_orginalImage.width() || y+1>=this->m_orginalImage.height())
        return;
    if(m_gradientImg[x][y]>0 && !isEdge[x][y])
    {
        edge.push_back(QPoint(x,y));
        isEdge[x][y] = true;

        if(m_directionImg[x][y] == DIR_HOR) // 如果x,y横着走
        {
            // Go Left
            if(!isEdge[x-1][y-1] && !isEdge[x-1][y] && !isEdge[x-1][y+1])
            {
                if(m_gradientImg[x-1][y-1] > m_gradientImg[x-1][y]
                        && m_gradientImg[x-1][y-1] > m_gradientImg[x-1][y+1])
                    searchFromArchor(x-1,y-1,isEdge,edge);
                else if(m_gradientImg[x-1][y+1] > m_gradientImg[x-1][y]
                        && m_gradientImg[x-1][y+1] > m_gradientImg[x-1][y-1])
                    searchFromArchor(x-1,y+1,isEdge,edge);
                else
                    searchFromArchor(x-1,y,isEdge,edge);
            }
            // Go right
            if(!isEdge[x+1][y-1] && !isEdge[x+1][y] && !isEdge[x+1][y+1])
            {
                if(m_gradientImg[x+1][y-1] > m_gradientImg[x+1][y]
                        && m_gradientImg[x+1][y-1] > m_gradientImg[x+1][y+1])
                    searchFromArchor(x+1,y-1,isEdge,edge);
                else if(m_gradientImg[x+1][y+1] > m_gradientImg[x+1][y]
                        && m_gradientImg[x+1][y+1] > m_gradientImg[x+1][y-1])
                    searchFromArchor(x+1,y+1,isEdge,edge);
                else
                    searchFromArchor(x+1,y,isEdge,edge);
            }
        }
        else if(m_directionImg[x][y] == DIR_VER) // 如果x,y竖着走
        {
            // Go Top
            if(!isEdge[x+1][y-1] && !isEdge[x][y-1] && !isEdge[x-1][y-1])
            {
                if(m_gradientImg[x-1][y-1] > m_gradientImg[x][y-1]
                        && m_gradientImg[x-1][y-1] > m_gradientImg[x+1][y-1])
                    searchFromArchor(x-1,y-1,isEdge,edge);
                else if(m_gradientImg[x+1][y-1] > m_gradientImg[x-1][y-1]
                        && m_gradientImg[x+1][y-1] > m_gradientImg[x][y-1])
                    searchFromArchor(x+1,y-1,isEdge,edge);
                else
                    searchFromArchor(x,y-1,isEdge,edge);
            }
            // Go Down
            if(!isEdge[x+1][y+1] && !isEdge[x][y+1] && !isEdge[x-1][y+1])
            {
                if(m_gradientImg[x-1][y+1] > m_gradientImg[x][y+1]
                        && m_gradientImg[x-1][y+1] > m_gradientImg[x+1][y+1])
                    searchFromArchor(x-1,y+1,isEdge,edge);
                else if(m_gradientImg[x+1][y+1] > m_gradientImg[x-1][y+1]
                        && m_gradientImg[x+1][y+1] > m_gradientImg[x][y+1])
                    searchFromArchor(x+1,y+1,isEdge,edge);
                else
                    searchFromArchor(x,y+1,isEdge,edge);
            }
        }
    }
}

QImage EdgeDrawing::getGrayImage(const QImage &img)
{
    QImage imggray = img;

    for(unsigned int x=0; x<img.width(); x++)
    {
        for(unsigned int y=0; y<img.height(); y++)
        {
            QColor rgb(img.pixel(x,y));
            double intensity = (rgb.red()+rgb.green()+rgb.blue())/3.0;

            imggray.setPixel(x,y,qRgb(intensity,intensity,intensity));
        }
    }

    //imggray.save("C:/Users/LiewDennis/Desktop/gtt1.jpg");

    return imggray;
}

QImage EdgeDrawing::getGaussianBlurImage(const QImage &img, int r)
{
    if(r==0)
        return img;
    QVector<double> weights = getGaussianWeights(r);  // 权重矩阵，长度为2*r+1

    QImage imgtmp  = img;
    QImage imgblur = img;

    // 第一个方向
    for(unsigned int y=0; y<img.height(); y++)
    {
        for(unsigned int x=0; x<img.width(); x++)
        {
            double red=0, green=0, blue=0;

            for(int i=-r; i<=r; i++)
            {
                int inx = rx(x,i,img.width());

                QColor rgb(img.pixel(inx,y));

                red   += rgb.red()*weights[r+i];
                green += rgb.green()*weights[r+i];
                blue  += rgb.blue()*weights[r+i];
            }
            imgtmp.setPixel(x,y,qRgb(red,green,blue));
        }
    }

    // 第二个方向
    for(unsigned int y=0; y<imgtmp.height(); y++)
    {
        for(unsigned int x=0; x<imgtmp.width(); x++)
        {
            double red=0, green=0, blue=0;

            for(int i=-r; i<=r; i++)
            {
                int iny = ry(y,i,imgtmp.height());

                QColor rgb(imgtmp.pixel(x,iny));

                red   += rgb.red()*weights[r+i];
                green += rgb.green()*weights[r+i];
                blue  += rgb.blue()*weights[r+i];
            }
            imgblur.setPixel(x,y,qRgb(red,green,blue));
        }
    }

    return imgblur;
}

int EdgeDrawing::rx(int x, int offset, int width)
{
    if(x+offset < 0 || x+offset>=width)
        return x-offset;

    return x+offset;
}

int EdgeDrawing::ry(int y, int offset, int height)
{
    if(y+offset < 0 || y+offset>=height)
        return y-offset;

    return y+offset;
}

QVector<double> EdgeDrawing::getGaussianWeights(int r)
{
#define SQRT_2PI 2.506628274631  // sqrt(2*PI)

    double sigma = r/3.0;
    double sum = 0;
    QVector<double> weights;
    weights.resize(2*r+1);

    for(int i=0; i<2*r+1; i++)  // r=1: 0,1,2
    {
        // 计算高斯函数值
        weights[i] = (1.0/(sigma*SQRT_2PI))
                * exp(-1.0*pow(i-r,2)/(2*pow(sigma,2)));
        sum += weights[i];
    }

    for(unsigned int i=0; i<2*r+1; i++)
        weights[i]/=sum;

    return weights;
}
