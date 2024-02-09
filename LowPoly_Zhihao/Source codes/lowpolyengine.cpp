#include "lowpolyengine.h"

float e_minTriangleSize = 5;
float e_gaussianBlurRadius = 0;
float e_sobelThreshold = 50;
float e_archorThreshold = 20;
float e_poissanRadius = 18;
bool  e_colorMode = false;  // true-average, false-center;

LowPolyEngine::LowPolyEngine()
{

}

void LowPolyEngine::setStartMode(THREAD_MODE mode)
{
    qsrand(QTime::currentTime().msec()*QTime::currentTime().second());
    this->m_threadMode = mode;
    this->start();
}

void LowPolyEngine::setOriginImage(const QImage &img)
{
    this->m_orginImage = img;

    this->m_edgepts.clear();
}

void LowPolyEngine::setEdgePts(const QVector<QPointF> edgepts)
{
    this->m_edgepts = edgepts;
}

void LowPolyEngine::returnResultToWidget(QImage &edgeImg, QImage& blurImg, QImage &lowpolyImg, QVector<QPointF> &edgepts, QVector<TRIANGLE>& tris, QVector<QRgb>& triColors)
{
    edgeImg = this->m_edgeImage;
    blurImg = this->m_blurImage;
    lowpolyImg = this->m_lowPolyImage;
    edgepts = this->m_edgepts;
    tris = this->m_triangles;
    triColors = this->m_triColors;

    this->m_triangles.clear();
    this->m_triColors.clear();
    this->m_edgepts.clear();
}

void LowPolyEngine::run()
{
    QTime s = QTime::currentTime();
    if(this->m_threadMode == THREAD_MODE::_REDO_ALL)
        this->m_lowPolyImage = getLowPolyImage(m_orginImage,e_minTriangleSize,e_sobelThreshold,e_archorThreshold,e_gaussianBlurRadius,e_poissanRadius);
    if(this->m_threadMode == THREAD_MODE::_REDO_DELAUNAY)
        this->m_lowPolyImage = this->renderCurLowPolyToImage(this->m_orginImage);
    QTime e = QTime::currentTime();

    qDebug()<<"耗时:"<<s.msecsTo(e)<<"ms";
}


QImage LowPolyEngine::getLowPolyImage(const QImage &img, int minDist, int sobelThreshold, int archorThreshold, int gaussR, int poissonR)
{
    this->m_edgepts.clear();

    // Blur & edge Image
    EdgeDrawing edgeDrawing;

    QVector<QVector<QPoint>> edges = edgeDrawing.getEdgesFromImage(img,this->m_blurImage,gaussR,sobelThreshold,archorThreshold);
    this->m_edgeImage = edgeDrawing.getEdgeImage();
    edgeDrawing.releaseAll();

    //minDist = 0.02*img.width()*img.height();
    QVector<QPointF> edgepts;
    for(unsigned i=0; i<edges.size(); i++)
    {
        if(edges[i].size() ==2 && edges[i].size()==1)
        {
            //edgepts.push_back(edges[i][0]);
        }
        else if(edges[i].size() <= minDist)
        {
            //edgepts.push_back(edges[i].front());
            //edgepts.push_back(edges[i].last());
        }
        else if(edges[i].size()>minDist)
        {
            unsigned int k;
            for(k=0; k<edges[i].size(); k+=minDist)
            {
                edgepts.push_back(edges[i][k]);
            }
            if(k<=edges[i].size()-1)
                edgepts.push_back(edges[i].last());
        }
    }

    while(edgepts.size() >=25000)
    {
        int id = qrand()%edgepts.size();
        edgepts.removeAt(id);
    }

    qDebug()<<"边缘点数量:"<<edgepts.size();
    // kd-tree
    std::vector<simplePoint3> cloud;  // markerPoints
    KDTree<simplePoint3> index;

    for(unsigned int i=0; i<edgepts.size(); i++)
    {
        simplePoint3 tmp;
        tmp.x = edgepts[i].x();
        tmp.y = edgepts[i].y();
        tmp.z = 0;

        cloud.push_back(tmp);
    }
    index.buildTree(cloud);

    for(unsigned int x=poissonR; x<img.width(); x+=2*poissonR)
    {
        for(unsigned int y=poissonR;y<img.height();y+=2*poissonR)
        {
            simplePoint3 query_pt;
            query_pt.x = x;
            query_pt.y = y;
            query_pt.z = 0;

            if(index.getPointsWithinCube(query_pt,poissonR).size()==0)
            {
               QPointF newpt(qrand()%(2*poissonR)+x-poissonR,qrand()%(2*poissonR)+y-poissonR);
               newpt.rx() = newpt.x()>=img.width()?img.width()-1:newpt.x();
               newpt.ry() = newpt.y()>=img.height()?img.height()-1:newpt.y();
               edgepts.push_back(newpt);
            }
        }
    }
    this->m_edgepts = edgepts;
    qDebug()<<"Poisson边缘点数量:"<<edgepts.size();

    emit sig_edgePts(this->m_edgepts.size());

    return renderCurLowPolyToImage(img);
}

QImage LowPolyEngine::renderCurLowPolyToImage(const QImage& img)
{
    Delaunay<float> m_delaunay;
    m_triangles.clear();
    m_triColors.clear();

    m_triangles = m_delaunay.getAllTriangles(m_edgepts);

    qDebug()<<"Delaunay Done";

    QVector<TRIANGLE> tris = m_triangles;
    m_triColors;
    for(unsigned int i=0; i<m_triangles.size(); i++)
    {
        // 使用中间区域的颜色
        if(e_colorMode == false)
        {
            int cx = (m_triangles[i][0].x()+m_triangles[i][1].x()+m_triangles[i][2].x())/3.0;
            int cy = (m_triangles[i][0].y()+m_triangles[i][1].y()+m_triangles[i][2].y())/3.0;
            m_triColors.push_back(img.pixel(cx,cy));
        }
        else if(e_colorMode == true)// 使用平均颜色
        {
            int sx = m_triangles[i].boundingRect().toRect().x();
            int sy = m_triangles[i].boundingRect().toRect().y();
            int ex = m_triangles[i].boundingRect().toRect().width() +sx;
            int ey = m_triangles[i].boundingRect().toRect().height()+sy;

            sx = sx>=img.width()?img.width()-1:sx;  // 修正
            sy = sy>=img.height()?img.height()-1:sy;

            int r = 0, g = 0, b = 0;
            int count =0;
            for(unsigned int x=sx;x<=ex; x++)
            {
                for(unsigned int y=sy; y<=ey; y++)
                {
                    if(m_triangles[i].containsPoint(QPointF(x,y),Qt::OddEvenFill))
                    {
                        QColor rgb(img.pixel(x,y));
                        r += rgb.red();
                        g += rgb.green();
                        b += rgb.blue();
                        count++;
                    }
                }
            }

            if(count!=0)
            {
                r/=count,g/=count,b/=count;
                m_triColors.push_back(qRgb(r,g,b));
            }
            else
            {
                this->m_triangles.removeAt(i--);
            }
        }

    }


    QImage imgres = img;
    //imgres = imgres.scaled(imgres.width()*2,imgres.height()*2);
    QPainter painter(&imgres);

    painter.drawImage(0,0,this->m_orginImage);

    painter.setRenderHint(QPainter::Antialiasing,true);
    for(unsigned int i=0; i<m_triangles.size(); i++)
    {
        QPolygonF poly = m_triangles[i];

        QPen pen(QColor(m_triColors[i]));
        pen.setWidth(0);
        painter.setPen(pen);
        painter.setBrush(QBrush(QColor(m_triColors[i])));
        painter.drawPolygon(poly,Qt::WindingFill);
    }
    return imgres;


}

