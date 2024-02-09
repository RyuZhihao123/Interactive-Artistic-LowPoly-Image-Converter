# LowPoly-Image-Converter-Zhihao
<!--
<div align=center><img height="200" src="https://github.com/RyuZhihao123/LowPoly-Image-Converter/blob/master/LowPoly_Zhihao/figures/6.png"/></div> 
-->

I created an easy-to-use software for easily converting images into artist LowPoly style using ``C++``.

$\bullet$ **Executable Software:**
[[Download EXE]](https://github.com/RyuZhihao123/LowPoly-Image-Converter/tree/master/LowPoly_Zhihao)

Double click the ``TestLowPoly.exe`` to directly execute my software. You can run the program easily **without any configuration** in Windows 7+ !
But please remember to download the entire folder (including .dlls) and then run this executable program.

$\bullet$ **Source Codes:**
[[Download Codes]](https://github.com/RyuZhihao123/LowPoly-Image-Converter/tree/master/LowPoly_Zhihao/Source%20codes).

You can compile my codes with most common C++ IDEs. We suggested to use Qt 5.8+ or Visual Studio 2019+. 


## 1. User Interface

![Result2.png](https://github.com/RyuZhihao123/LowPoly-Image-Converter/blob/master/LowPoly_Zhihao/figures/1-1.png)


- **Load & Save Images:** Use the two buttons in the top-right to load new images or save your creations.


- **Parameter Control:** Users can easily adjust a few simple parameters to control the LowPolys in different level of detail. The provided parameters includes:
    - ``Blur``: Larger Blur values will smooth the original image more heavely.
    - ``Achor Threshold``: It controls how many pixels will be selected as initial Archors.
    - ``Sobel Threshold``: It controls how the final LowPoly image will reflect the edge features.
    - ``Min Triangle Size``: It controls the size of the Delauney Triangularization.
    - ``Poisson Dict Radius``: It controls the density of intial sampled poisson points.

- **Free Edit the Delauney Triangularization:** As shown in the right of Fig.1, we enable the users to edit the final LowPoly images by freely adding or removing the delauney points as their wish.
To do this, you need to switch the ComboBox into the ``**Process`` mode as follows. Under this mode, you can use ``Ctrl+LeftMouseButton`` to add new Delauney points and ``Alt+LeftMouseButton`` to remove the points within a rectangular area.

<div align=center><img height="200" src="https://github.com/RyuZhihao123/LowPoly-Image-Converter/blob/master/LowPoly_Zhihao/figures/0.png"/></div>


## 2. Results

Alright, now let's enjoy this software and explore your own LowPolys!

<div align=center><img height="500" src="https://github.com/RyuZhihao123/LowPoly-Image-Converter/blob/master/LowPoly_Zhihao/figures/5.png"/></div>

Results under different paramter settings.

<div align=center><img width="800" src="https://github.com/RyuZhihao123/LowPoly-Image-Converter/blob/master/LowPoly_Zhihao/figures/2.png"/></div>

<div align=center><img width="800" src="https://github.com/RyuZhihao123/LowPoly-Image-Converter/blob/master/LowPoly_Zhihao/figures/3.png"/></div>

<div align=center><img width="800" src="https://github.com/RyuZhihao123/LowPoly-Image-Converter/blob/master/LowPoly_Zhihao/figures/4.png"/></div>





## 3. A Clean Version of Codes for Edge_Drawing
Since many friends asked me about my implementation of the Edge Drawing Algorithm, which is one of the steps of my LowPoly software.
I sorted out a clean version C++ codes for this algorithm seperately.
It can produce high-quality edge segments, which are clean, well-localized and one-pixel wide. Please refer to this paper (Topal and Akinlar[1]) for more details.

You can get my sources codes from this [[Link]](https://github.com/RyuZhihao123/LowPoly-Image-Converter/tree/master/TestEdgeDrawing):

### Reference:
[1] Cihan Topal, Cuneyt Akinlar. 'Edge Drawing:A combined real-time edge and segment detector'.

### Requirments:
ONLY NEEDS ***Qt***(Qt5.2 or later is supported) or Visual Studio 2019+ to configure this project. NOT DEPENDS ON OpenCV.

### Example Results:
Different edges are shown in different colors.
<div align=center><img width="700" src="https://github.com/RyuZhihao123/Edge_Drawing/blob/master/result_examples/result3.png"/></div>


### Usage:
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

### Others:
(1) Calling ***getEdgesFromImage()*** will return a *vector* of *_EDGE*.<br />
    **QVector\<QVector\<QPoint\>\>** is used to describe the edge information, which is a 2-dimentional vector. And each **QVector\<QPoint\>** is used to define one single edge.
```
QVector<_EDGE> getEdgesFromImage(const QImage& img,int gaussR=5,int sobelThreshold=30,int archorThreshold=60);
```
(2) Calling ***getEdgeImage()*** will return an edge image.
```
QImage getEdgeImage(const QImage &img, int gaussR=5,int sobelThreshold=30,int archorThreshold=60);
```
(3) Paramters:<br />
            **img**     : the original image you input.<br />
            **gaussR**  : the distance of gaussian blur.<br />
            **sobelThreshold** : the threshold of sobel filter<br />
            **archorThreshold**: the threshold to determine archors.

