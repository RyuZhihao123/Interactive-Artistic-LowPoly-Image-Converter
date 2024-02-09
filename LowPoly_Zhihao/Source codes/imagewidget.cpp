#include "imagewidget.h"
#include "ui_imageprocessor.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget),m_mode(MODE::_LOWPOLY),m_scale(1.0f)
{
    ui->setupUi(this);
    this->setFocusPolicy(Qt::StrongFocus);

    this->setOriginImage("C:/Users/LiewDennis/Desktop/tmpsamples/2.bmp");  //default image;
}

void ImageWidget::setMode(MODE m)
{
    this->m_mode = m;
    update();
}

void ImageWidget::setOriginImage(const QString &path)
{
    this->m_orginImage = QImage(path);

    this->m_scale = 1.0f;
    this->m_leftPos = QPoint(30,30);
    this->m_isPicked = false;
    this->m_isChangeOriginalImage = true;

    this->m_lowPolyEngine.setOriginImage(this->m_orginImage);
    this->m_lowPolyEngine.setStartMode(LowPolyEngine::_REDO_ALL);
}

void ImageWidget::refreshFromCurImage()
{
    this->m_lowPolyEngine.setStartMode(LowPolyEngine::_REDO_ALL);
}
void ImageWidget::refreshDelaunayFromCurImage()
{
    this->m_lowPolyEngine.setEdgePts(this->m_edgepts);
    this->m_lowPolyEngine.setStartMode(LowPolyEngine::_REDO_DELAUNAY);
}

void ImageWidget::threadProcessDone()
{
    this->m_lowPolyEngine.returnResultToWidget(this->m_edgeImage,
                                               this->m_blurImage,
                                               this->m_lowPolyImage,
                                               this->m_edgepts,
                                               this->m_triangles,
                                               this->m_triColors);

    this->m_isChangeOriginalImage = false;
    this->update();
}

void ImageWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255,255,255));
    painter.drawRect(QRect(0,0,width(),height()));
    painter.setBrush(QColor(0,0,0,0));

     // 修改比例后的缩放
    const int w = this->m_orginImage.width() * this->m_scale;
    const int h = this->m_orginImage.height()* this->m_scale;

    if(this->m_isChangeOriginalImage)
    {
        painter.drawImage(m_leftPos,this->m_orginImage.scaled(w,h));
        return;
    }
    if(m_mode == MODE::_ORIGIN )
        painter.drawImage(m_leftPos,this->m_orginImage.scaled(w,h));
    if(m_mode == MODE::_BLUR)
        painter.drawImage(m_leftPos,this->m_blurImage.scaled(w,h));
    if(m_mode == MODE::_EDGE)
        painter.drawImage(m_leftPos,this->m_edgeImage.scaled(w,h));

    if(m_mode == MODE::_LOWPOLY)
    {
        painter.drawImage(m_leftPos,this->m_orginImage.scaled(w,h));
        for(unsigned int i=0; i<m_triangles.size(); i++)
        {
            QPolygonF poly;
            for(unsigned int k=0; k<m_triangles[i].size(); k++)
            {
                QPointF tmp;

                tmp.setX(m_leftPos.x() + m_triangles[i][k].x()*m_scale);
                tmp.setY(m_leftPos.y() + m_triangles[i][k].y()*m_scale);
                poly.push_back(tmp);
            }
            painter.setRenderHint(QPainter::HighQualityAntialiasing);
            QPen pen(QColor(0,0,0,0));
            pen.setWidth(0);
            painter.setPen(pen);
            painter.setBrush(QColor(this->m_triColors[i]));
            painter.drawPolygon(poly,Qt::WindingFill);
        }
    }

    if(m_mode == MODE::_PROCESS)
    {
        painter.drawImage(m_leftPos,this->m_orginImage.scaled(w,h));
        for(unsigned int i=0; i<m_triangles.size(); i++)
        {
            QPolygonF poly;
            for(unsigned int k=0; k<m_triangles[i].size(); k++)
            {
                QPointF tmp;

                tmp.setX(m_leftPos.x() + m_triangles[i][k].x()*m_scale);
                tmp.setY(m_leftPos.y() + m_triangles[i][k].y()*m_scale);
                poly.push_back(tmp);
            }
            painter.setRenderHint(QPainter::HighQualityAntialiasing);
            QPen pen(QColor(10,10,255,160));
            pen.setWidth(1);
            painter.setPen(pen);

            painter.drawPolygon(poly);
        }
        // edge points
        for(unsigned int i=0; i<m_edgepts.size(); i++)
        {
            QPen pen(QColor(10,10,255));
            pen.setWidth(2);
            painter.setPen(pen);
            painter.setBrush(QColor(10,10,255));
            painter.drawEllipse(m_leftPos.x()+m_edgepts[i].x()*m_scale,
                                m_leftPos.y()+m_edgepts[i].y()*m_scale
                                ,2,2);
        }

        if(this->m_isPicked)
        {
            QPen pen(QColor(250,20,10,180));
            pen.setWidth(2);
            painter.setPen(pen);
            painter.setBrush(QColor(255,20,10,80));

            QPolygonF poly;
            for(unsigned int i=0; i<m_pickPoly.size(); i++)
            {
                QPointF tmp;

                tmp.setX(m_leftPos.x() + m_pickPoly[i].x()*m_scale);
                tmp.setY(m_leftPos.y() + m_pickPoly[i].y()*m_scale);
                poly.push_back(tmp);
            }
            painter.drawPolygon(poly);
        }
    }
}


void ImageWidget::wheelEvent(QWheelEvent *e)
{
    if(e->delta() > 0)
        this->m_scale += 0.1;
    else if(this->m_scale-0.1 > 0)
        this->m_scale -= 0.1;

    this->update();
}

void ImageWidget::mousePressEvent(QMouseEvent *e)
{

    this->m_lastPos = e->pos();

    // 添加新的edge points;
    if( !this->m_lowPolyEngine.isRunning() && e->modifiers() == Qt::ControlModifier
            && e->buttons() == Qt::LeftButton && m_mode == MODE::_PROCESS)
    {
        float w = e->pos().x()/m_scale;
        float h = e->pos().y()/m_scale;
        QPointF newEdgePt(w-m_leftPos.x()/m_scale, h-m_leftPos.y()/m_scale);

        this->m_edgepts.push_back(newEdgePt);

        this->m_lowPolyEngine.setEdgePts(this->m_edgepts);
        this->m_lowPolyEngine.setStartMode(LowPolyEngine::_REDO_DELAUNAY);

        this->update();
        return;
    }

    if(!this->m_lowPolyEngine.isRunning() && e->modifiers() == Qt::AltModifier
            && e->buttons() == Qt::LeftButton && m_mode == MODE::_PROCESS)
    {
        this->m_isPicked = true;
        this->update();
        return;
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(!this->m_lowPolyEngine.isRunning() && e->modifiers() == Qt::AltModifier
            && e->buttons() == Qt::LeftButton && m_mode == MODE::_PROCESS)
    {
        float x2 = (e->pos().x() - m_leftPos.x())/m_scale;
        float y2 = (e->pos().y() - m_leftPos.y())/m_scale;
        float x = (m_lastPos.x() - m_leftPos.x())/m_scale;
        float y = (m_lastPos.y() - m_leftPos.y())/m_scale;

        QPolygonF poly;
        poly.push_back(QPointF(x,y));
        poly.push_back(QPointF(x2,y));
        poly.push_back(QPointF(x2,y2));
        poly.push_back(QPointF(x,y2));

        this->m_pickPoly = poly;
        this->m_isPicked = true;
        this->update();

        return;
    }

    // For regular movement of mouse;
    QPoint curPos = e->pos();

    this->m_leftPos.rx() += (curPos.x() - m_lastPos.x());
    this->m_leftPos.ry() += (curPos.y() - m_lastPos.y());
    this->m_lastPos = curPos;
    this->update();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->m_isPicked && !this->m_lowPolyEngine.isRunning())
    {
        this->m_isPicked = false;

        float x2 = (e->pos().x() - m_leftPos.x())/m_scale;
        float y2 = (e->pos().y() - m_leftPos.y())/m_scale;
        float x = (m_lastPos.x() - m_leftPos.x())/m_scale;
        float y = (m_lastPos.y() - m_leftPos.y())/m_scale;

        QPolygonF poly;
        poly.push_back(QPointF(x,y));
        poly.push_back(QPointF(x2,y));
        poly.push_back(QPointF(x2,y2));
        poly.push_back(QPointF(x,y2));

        for(unsigned int i=0; i<m_edgepts.size(); i++)
        {
            if(poly.containsPoint(m_edgepts[i],Qt::OddEvenFill))
                m_edgepts.removeAt(i--);
        }


        this->m_lowPolyEngine.setEdgePts(this->m_edgepts);
        this->m_lowPolyEngine.setStartMode(LowPolyEngine::_REDO_DELAUNAY);
        this->update();
        return;
    }
}


ImageWidget::~ImageWidget()
{
    delete ui;
}
