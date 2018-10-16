#include <QLabel>
#include <iostream>
#include "edgedrawing.h"

int main(int argc, char *argv[])
{
    // Notice that choose valid paths.
    QString inputPath = "C:/Users/LiewDennis/Desktop/default.jpg";
    QString savePath  = "C:/Users/LiewDennis/Desktop/result.jpg";

    EdgeDrawing edgeDrawing;

    // Test for 'getEdgesFromImage()'
    QVector<QVector<QPoint>> edgeVector =edgeDrawing.getEdgesFromImage(QImage(inputPath),5,30,8);
    std::cout<<"Edge Amount: "<<edgeVector.size()<<std::endl;

    // Test for 'getEdgeImage()'
    QImage edgeImg = edgeDrawing.getEdgeImage(QImage(inputPath),5,30,8);
    edgeImg.save(savePath);

    return 0;
}
