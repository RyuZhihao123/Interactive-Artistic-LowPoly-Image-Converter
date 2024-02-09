#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "imagewidget.h"

#define SS_STATE_RUNING QString("QLabel{background-color:rgb(255,20,20);color:rgb(255,255,255);font-family: \"微软雅黑\";font-size:19px;}")
#define SS_STATE_DONE   QString("QLabel{background-color:rgb(26,188,156);color:rgb(255,255,255);font-family: \"微软雅黑\";font-size:19px;}")

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    ImageWidget* m_imageProcessor;

protected:
    void resizeEvent(QResizeEvent* e);

    void updateParameters();

private slots:
    void slot_cbxModeChanged(QString mode);
    void slot_refreshLowPoly();
    void slot_loadNewImage();
    void slot_processImageStart();
    void slot_processImagePts(int count);
    void slot_processImageStop();
    void slot_radioBtnColorMode();
    void slot_savePolyImage();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
