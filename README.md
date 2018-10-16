# Edge_Drawing
Implementation of Edge Drawing Algorithm with Qt, which can produce high-quality edge segments, which are clean, well-localized and one-pixel wide, refering to the paper[1].

This project is implemented by ***RyuZhihao123*** using *Qt*. Users can get egdes which are one pixel wide and clean from an input image.

## Reference:
[1] Cihan Topal, Cuneyt Akinlar. 'Edge Drawing:A combined real-time edge and segment detector'.

## Requirments:
Only ***Qt***. No need for OpenCV etc.

## Example Results:
Different Edge is showed in different colors.
![Result1.png](https://github.com/RyuZhihao123/Edge_Drawing/blob/master/result_examples/result3.png)
Some other results. all the edges are showed in the same color.
![Result2.png](https://github.com/RyuZhihao123/Edge_Drawing/blob/master/result_examples/result1.png)
![Result3.png](https://github.com/RyuZhihao123/Edge_Drawing/blob/master/result_examples/result2.png)
![Result4.png](https://github.com/RyuZhihao123/Edge_Drawing/blob/master/result_examples/result4.png)
![Result5.png](https://github.com/RyuZhihao123/Edge_Drawing/blob/master/result_examples/result5.png)
## Usage:
```
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
```

## Others:
(1). Structure ***_EDGE*** is defined as QVector\<QPoint\>, which is a *vector* of *QPoint*.

(2) Calling ***getEdgesFromImage()*** will return a *vector* of *_EDGE*.\<br /\>
    **QVector\<_EDGE\>** is equal to **QVector\<QVector\<QPoint\>\>**, which is a 2-dimentional vector. and each **QVector\<QPoint\>** is used to describe an edge.
```
QVector<_EDGE> getEdgesFromImage(const QImage& img,int gaussR=5,int sobelThreshold=30,int archorThreshold=60);
```
(3) Calling ***getEdgeImage()*** will return an edge image.
```
QImage getEdgeImage(const QImage &img, int gaussR=5,int sobelThreshold=30,int archorThreshold=60);
```
(4) Paramters:
            **img**     : the original image you input.
            **gaussR**  : the distance of gaussian blur.
            **sobelThreshold** : the threshold of sobel filter
            **archorThreshold**: the threshold to determine archors.

