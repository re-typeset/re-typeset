// This is not The Greatest Code in the World, no! This is just a tribute!
//
// Re-Typeset program for converting scanned documents to smaller size of page
// without changing text size.
//
// Copyright (C) 2013-2016 Piotr Mika (piotr.mt.mika@gmail.com).
//
// This  program is free software:  you can  redistribute it and/or  modify it
// under the terms of the  GNU General Public License as published by the Free
// Software Foundation,  either version 3 of the License,  or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY  WARRANTY;  without  even the implied  warranty  of MERCHANTABILITY  or
// FITNESS FOR  A PARTICULAR  PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the  GNU  General  Public  License along
// with this program. If not, see <http://www.gnu.org/licenses/>.

#include "calibratewindow.hpp"
#include "ui_calibratewindow.h"
#include <QDir>
#include "scanneddocument.hpp"
#include <QDebug>
#include "extendedimage.hpp"

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

	setWindowTitle(tr("Calibrate algorithm"));
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
		ExtendedImage imageColor( fileColor );
		inputImage_ = imageColor.convertToMonoImage( treshold_ );
		setWindowTitle( QString("%1 (%2)").arg(filesInDir[imageNum_], dirPath_));
		runAlgortithm();
	} else {
		inputImage_=QImage();
	}

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
	page.findBaselines( inputImage_ );

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
