#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_imageProcessor = new ImageWidget(this->ui->centralWidget);

    updateParameters();
    this->setWindowIcon(QIcon(":/img/res/logo.png"));
    this->setWindowTitle("Low Poly - 作者：RyuZhihao123(刘志浩)");

    connect(ui->cbxMode,SIGNAL(currentIndexChanged(QString)),this,SLOT(slot_cbxModeChanged(QString)));

    connect(ui->sliderArchorThreshold,SIGNAL(valueChanged(int)),this,SLOT(slot_refreshLowPoly()));
    connect(ui->sliderEdgePoints,SIGNAL(valueChanged(int)),this,SLOT(slot_refreshLowPoly()));
    connect(ui->sliderPoissonRadius,SIGNAL(valueChanged(int)),this,SLOT(slot_refreshLowPoly()));
    connect(ui->sliderSobelThreshold,SIGNAL(valueChanged(int)),this,SLOT(slot_refreshLowPoly()));
    connect(ui->sliderBlurRadius,SIGNAL(valueChanged(int)),this,SLOT(slot_refreshLowPoly()));
    connect(ui->sliderArchorThreshold,SIGNAL(sliderReleased()),this,SLOT(slot_refreshLowPoly()));
    connect(ui->sliderEdgePoints,SIGNAL(sliderReleased()),this,SLOT(slot_refreshLowPoly()));
    connect(ui->sliderPoissonRadius,SIGNAL(sliderReleased()),this,SLOT(slot_refreshLowPoly()));
    connect(ui->sliderSobelThreshold,SIGNAL(sliderReleased()),this,SLOT(slot_refreshLowPoly()));
    connect(ui->sliderBlurRadius,SIGNAL(sliderReleased()),this,SLOT(slot_refreshLowPoly()));


    connect(ui->btnLoad,SIGNAL(clicked(bool)),this,SLOT(slot_loadNewImage()));
    connect(ui->btnLoad_2,SIGNAL(clicked(bool)),this,SLOT(slot_savePolyImage()));
    connect(m_imageProcessor->lowPolyEngine(),SIGNAL(started()),this,SLOT(slot_processImageStart()));
    connect(m_imageProcessor->lowPolyEngine(),SIGNAL(finished()),this,SLOT(slot_processImageStop()));
    connect(m_imageProcessor->lowPolyEngine(),SIGNAL(sig_edgePts(int)),this,SLOT(slot_processImagePts(int)));

    connect(ui->rBtnAverage,SIGNAL(clicked(bool)),this,SLOT(slot_radioBtnColorMode()));
    connect(ui->rBtnCenter,SIGNAL(clicked(bool)),this,SLOT(slot_radioBtnColorMode()));
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    m_imageProcessor->resize(this->ui->centralWidget->width()-ui->groupBox->width(),
                             this->ui->centralWidget->height());
    ui->groupBox->move(m_imageProcessor->x()+m_imageProcessor->width(),0);
}

void MainWindow::slot_cbxModeChanged(QString mode)
{
    if(mode == "OriginImage")
        m_imageProcessor->setMode(ImageWidget::_ORIGIN);
    if(mode == "BlurImage")
        m_imageProcessor->setMode(ImageWidget::_BLUR);
    if(mode == "EdgeImage")
        m_imageProcessor->setMode(ImageWidget::_EDGE);
    if(mode == "LowPoly")
        m_imageProcessor->setMode(ImageWidget::_LOWPOLY);
    if(mode == "***Process")
        m_imageProcessor->setMode(ImageWidget::_PROCESS);
}

void MainWindow::updateParameters()
{
    ui->lblArchorThreshold->setText(QString::number(e_archorThreshold));
    ui->lblMinTriangleSize->setText(QString::number(e_minTriangleSize));
    ui->lblPoissanRadius->setText(QString::number(e_poissanRadius));
    ui->lblSobelThreshold->setText(QString::number(e_sobelThreshold));

    ui->sliderArchorThreshold->setValue(e_archorThreshold);
    ui->sliderEdgePoints->setValue(e_minTriangleSize);
    ui->sliderPoissonRadius->setValue(e_poissanRadius);
    ui->sliderSobelThreshold->setValue(e_sobelThreshold);
    ui->sliderBlurRadius->setValue(e_gaussianBlurRadius);

    if(e_colorMode == true)
        ui->rBtnAverage->setChecked(true);
    else
        ui->rBtnCenter->setChecked(true);
}

void MainWindow::slot_refreshLowPoly()
{
    QSlider* slider = (QSlider*)QObject::sender();

    e_minTriangleSize = ui->sliderEdgePoints->value();
    e_archorThreshold = ui->sliderArchorThreshold->value();
    e_sobelThreshold = ui->sliderSobelThreshold->value();
    e_poissanRadius = ui->sliderPoissonRadius->value();
    e_gaussianBlurRadius = ui->sliderBlurRadius->value();

    ui->lblArchorThreshold->setText(QString::number(e_archorThreshold));
    ui->lblMinTriangleSize->setText(QString::number(e_minTriangleSize));
    ui->lblPoissanRadius->setText(QString::number(e_poissanRadius));
    ui->lblSobelThreshold->setText(QString::number(e_sobelThreshold));

    if(slider->isSliderDown())
        return;

    this->m_imageProcessor->refreshFromCurImage();
}

void MainWindow::slot_loadNewImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open An Image"),
                                                      ".e",
                                                      tr("Images (*.png *.jpg *.bmp)"));

    if(fileName == "")
        return;

    this->m_imageProcessor->setOriginImage(fileName);
}

void MainWindow::slot_savePolyImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "./untitled.png",
                                                    tr("Images (*.png)"));

    if(fileName == "")
        return;
    this->m_imageProcessor->lowPolyImage().save(fileName);
}

void MainWindow::slot_processImageStart()
{
    this->ui->groupBox->setEnabled(false);
    ui->lblState->setText(QString("Running"));
    ui->lblState->setStyleSheet(SS_STATE_RUNING);
}

void MainWindow::slot_processImageStop()
{
    this->ui->groupBox->setEnabled(true);
    this->m_imageProcessor->threadProcessDone();
    ui->lblState->setText(QString("Done"));
    ui->lblState->setStyleSheet(SS_STATE_DONE);
}

void MainWindow::slot_processImagePts(int count)
{
    int msec = 0.00003*count*count - 0.2576*count+1360.6;
    int sec = msec/1000;
    int ms  = msec - sec*1000;

    ui->lblState->setText(QString("Running(%1s%2ms)").arg(sec).arg(ms));
}

void MainWindow::slot_radioBtnColorMode()
{
    if(ui->rBtnAverage->isChecked())
        e_colorMode = true;
    if(ui->rBtnCenter->isChecked())
        e_colorMode = false;
    this->m_imageProcessor->refreshDelaunayFromCurImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}
