#include "calibratewindow.hpp"
#include "ui_calibratewindow.h"
#include <QDir>
#include "scanneddocument.hpp"
#include <QDebug>

CalibrateWindow::CalibrateWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CalibrateWindow)
    , imageNum_(0)
    , treshold_(158)
{
	ui->setupUi(this);
    ui->calibrateInputImage->setImage( & inputImage_ );
    ui->calibrateOutputImage->setImage( & outputImage_ );
    ui->calibrateInputImage->setSecondWidget(ui->calibrateOutputImage);
    ui->calibrateOutputImage->setSecondWidget(ui->calibrateInputImage);
    connect(ui->zoomNormalButton, SIGNAL(clicked(bool)), ui->calibrateInputImage, SLOT(setZoomNormal()));
    connect(ui->zoomFitButton, SIGNAL(clicked(bool)), ui->calibrateInputImage, SLOT(setZoomFit()));
}

CalibrateWindow::~CalibrateWindow()
{
	delete ui;
}

QFormLayout *CalibrateWindow::getParametersFormLayout()
{
    return ui->variables;
}

void CalibrateWindow::setDirPath(const QString &dirPath)
{
    dirPath_ = dirPath;
    imageNum_ = 0;    
    reloadImage();
}

void CalibrateWindow::reloadImage()
{
    QDir dir(dirPath_);
    QStringList filesInDir=dir.entryList(
                      QDir::Files | QDir::NoDotAndDotDot,
                      QDir::Name );

    if( imageNum_ < 0 ) {
        imageNum_ = filesInDir.size() - 1;
    }
    if( imageNum_ >= filesInDir.size() ) {
        imageNum_ = 0;
    }

    if( filesInDir.size() > 0 ) {
        QString fileColor = dirPath_ + "/" + filesInDir[imageNum_];
        QImage imageColor( fileColor );
        inputImage_ = ScannedDocument::convertToMonoImage( imageColor, treshold_ );
        setWindowTitle( QString("%1 (%2)").arg(filesInDir[imageNum_], dirPath_));
    } else {
        inputImage_=QImage();
    }
    runAlgortithm();
}

void CalibrateWindow::runAlgortithm()
{

    ScannedPage page("","");
    Stats lineStats;

    page.getLinesAndHeights( lineStats, inputImage_ );

    lineStats.calculateParams();
    lineStats.clear();

    const StatsPack & stats=lineStats.getStats();

    page.reConnectDots( stats );
    page.trimDivideLines( stats, inputImage_ );
    page.connectDescriptionsToImages( stats, inputImage_ );
    page.findParagraphs( stats );

    page.findDividedWords( inputImage_);

    //StatsNumberHeader numHead;
    //page.checkNumberHeader( stats, numHead );
    //page.setNumberHeader( stats, number_, header_ );

    outputImage_=page.printDEbugImages( stats, inputImage_ );
    repaint();
}

void CalibrateWindow::setZoomFit()
{
    ui->calibrateInputImage->setZoomFit();
}

void CalibrateWindow::connectSpinBoxes()
{
    for( int i=0; i<ui->variables->rowCount(); ++i ) {
        QDoubleSpinBox * spinBox = static_cast<QDoubleSpinBox *>(ui->variables->itemAt(i, QFormLayout::FieldRole)->widget());
        connect(spinBox , SIGNAL(valueChanged(double)), this, SLOT(runAlgortithm()) );
    }
}

void CalibrateWindow::setTreshold(int treshold)
{
    treshold_ = treshold;
}

void CalibrateWindow::on_previousImageButton_clicked()
{
    --imageNum_;
    reloadImage();
}

void CalibrateWindow::on_nextImageButton_clicked()
{
    ++imageNum_;
    reloadImage();
}
