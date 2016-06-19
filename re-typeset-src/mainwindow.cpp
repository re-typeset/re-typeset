// Re-Typeset program for converting scanned documents to smaller size of page
// without changing text size.
//
// Copyright (C) 2013-2015 Piotr Mika (piotr.mt.mika@gmail.com).
// Copyright (C)      2015 Marcin Garbiak (marcin.garbiak@gmail.com).
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

#include "mainwindow.hpp"
#include "license.hpp"
#include "ui_mainwindow.h"
#include <QImage>
#include <QDebug>
#include <QPainter>
#include <qthread.h>

#define DE qDebug()

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	QFileDialog_ShowAllFiles=static_cast< QFileDialog::Option >( 0 ); //No '0' value in 'enum Option' in 'QFileDialog' class
	ui->setupUi(this);

	ui->textSizeHeight->setSuffix( " px" );
	ui->pageSizeMargin->setSuffix( " px" );
	ui->pageSizeHeight->setSuffix( " px" );
	ui->pageSizeWidth->setSuffix( " px" );

	createLanguageMenu();
	setWindowIcon( QIcon( QApplication::applicationDirPath() + "res/re-typeset.png" ) );

	QString locale = QLocale::system().name();
	locale.truncate(locale.lastIndexOf('_'));
	loadLanguage( locale );

	startText_=tr( "Start" );
	stopText_=tr( "Stop" );
	ui->outStartButton->setText( startText_ );
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::setProgressBar(int percent, QString name, int n, int of) {
	ui->outProgressBar->setValue( percent );
	if( of !=0 ) {
		ui->outProgressBar->setFormat( tr("%p%, step %1 of %2, %3" ).arg( n ).arg( of ).arg( name ) );
	}
	setWindowTitle( ui->outProgressBar->text() + " — Re-Typeset" );
}

void MainWindow::processingFinished() {

	ui->outStartButton->setText( startText_ );
	ui->outStartButton->setDisabled( false );
	ui->outProgressBar->setDisabled( true );
	ui->outProgressBar->setFormat( "%p%" );
	ui->outProgressBar->setValue( 0 );
	setWindowTitle( "Re-Typeset" );

	uIWidgetsSetDisabled( false );

	if( processingThread_.status != 0 ) {
		QMessageBox::information( this, tr( "Done"), tr("Processing finished.") );
	}
}

void MainWindow::on_textSizeHeight_valueChanged(int arg1) {
	ui->textLinesPerPage->blockSignals( true );
	ui->textLinesPerPage->setValue( (ui->pageSizeHeight->value() - ui->pageSizeMargin->value()*2 )
									/ Consts::LineHeightFromTextHeight( arg1 ) - 1 );
	ui->textLinesPerPage->blockSignals( false );
}

void MainWindow::on_textLinesPerPage_valueChanged(int arg1) {
	ui->textSizeHeight->blockSignals( true );
	ui->textSizeHeight->setValue( Consts::TextHeightFromLineHeight(
									  ( ui->pageSizeHeight->value() - ui->pageSizeMargin->value()*2 )
									  / ( arg1 +1 ) ) );
	ui->textSizeHeight->blockSignals( false );
}
void MainWindow::on_pageSizeHeight_valueChanged(int arg1) {
	(void) arg1;
	on_textSizeHeight_valueChanged( ui->textSizeHeight->value() );
}

void MainWindow::on_pageSizeMargin_valueChanged(int arg1) {
	(void) arg1;
	on_textSizeHeight_valueChanged( ui->textSizeHeight->value() );
}

void MainWindow::on_srcDirButton_clicked() {
	QString dir=QFileDialog::getExistingDirectory( this, tr("Choose source directory"),
												   ui->srcDir->text(), QFileDialog_ShowAllFiles );
	dir=changeFileToBasedir( dir );
	ui->srcDir->setText( dir );
	dir+=".out";

	QFileInfo fi( ui->srcDir->text() );
	if( QDir( dir ).exists() ) {
		ui->destDir->setText( dir );
		ui->destDirCreate->setText( tr( "Create directory: NAME.out" ) );
		ui->destDirCreate->setEnabled( false );
	} else {
		ui->destDirCreate->setText( tr( "Create directory: \n``%1.out''" ).arg( fi.baseName() ) );
		ui->destDirCreate->setEnabled( true );
	}
	ui->outFilesPrefix->setText( fi.baseName() );

}

void MainWindow::on_destDirCreate_clicked() {
	QString dir=ui->srcDir->text();
	dir+=".out";
	if( QDir().mkdir( dir ) ) {
		ui->destDirCreate->setText( tr( "Create directory: NAME.out" ) );
		ui->destDirCreate->setEnabled( false );
		ui->destDir->setText( dir );
	}
}

void MainWindow::on_destDirButton_clicked() {
	QString dir=QFileDialog::getExistingDirectory( this, tr("Choose source directory"),
												   ui->srcDir->text(), QFileDialog_ShowAllFiles );
	dir=changeFileToBasedir( dir );
	ui->destDir->setText( dir );
}

void MainWindow::on_treshSlider_sliderMoved(int position) {
	ui->treshVal->setValue( position );
}

void MainWindow::on_treshVal_valueChanged(int arg1) {
	ui->treshSlider->setValue( arg1 );
}

void MainWindow::on_treshSlider_valueChanged(int value) {
	ui->treshVal->setValue( value );
}


void MainWindow::on_outStartButton_clicked() {
	if( ui->outStartButton->text() == startText_ ) {
		if( ( ! ui->srcDir->text().isEmpty() ) && ( ! ui->destDir->text().isEmpty() )
				&& QDir( ui->srcDir->text() ).exists()
				&& QDir( ui->destDir->text() ).exists() ) {

			ui->outStartButton->setText( stopText_ );
			ui->outProgressBar->setDisabled( false );

			uIWidgetsSetDisabled( true );

			processingThread_.srcDir=ui->srcDir->text();
			processingThread_.destDir=ui->destDir->text();
			processingThread_.DEbugState=ui->actionShowDebugImages->isChecked();
			processingThread_.treshold=ui->treshVal->value();
			processingThread_.comicMode=ui->optionsComicMode->isChecked();
			processingThread_.findDividedWords=ui->optionsFindDividedWords->isChecked();
			processingThread_.width=ui->pageSizeWidth->value();
			processingThread_.height=ui->pageSizeHeight->value();
			processingThread_.margin=ui->pageSizeMargin->value();
			processingThread_.fontHeight=ui->textSizeHeight->value();
			processingThread_.hardMargins=ui->optionsHardMargins->isChecked();
			processingThread_.noUpscalling=ui->optionsNoUpscaling->isChecked();
			processingThread_.fullColor=ui->optionsFullColorOutput->isChecked();
			processingThread_.justify=ui->optionsJustify->isChecked();
			processingThread_.rotateImages=ui->optionsRotateImages->isChecked();
			processingThread_.comicMode=ui->optionsComicMode->isChecked();
			processingThread_.fileNamePrefix=ui->outFilesPrefix->text();

			processingThread_.work=true;
			processingThread_.progressBarSignalReciever=this;

			connect( &processingThread_, SIGNAL( finished() ), this, SLOT( processingFinished() ) );

			processingThread_.start();


		} else {
			QMessageBox::critical( this, tr("Error, wrong directories"), tr("Directories paths set improperly.") );
		}
	} else {
		if( QMessageBox::Yes == QMessageBox::question( this, tr("Stop?"), tr("Do you want to stop processing?") ) ) {
			processingThread_.work=false;
			ui->outStartButton->setDisabled( true );
		}
	}
}

void MainWindow::on_predefScreen800_clicked() {
	ui->pageSizeHeight->setValue( 800 );
	ui->pageSizeWidth->setValue( 600 );
}

void MainWindow::on_predefScreen1024_clicked() {
	ui->pageSizeHeight->setValue( 1024 );
	ui->pageSizeWidth->setValue( 758 );
}

void MainWindow::on_predefScreen1440_clicked() {
	ui->pageSizeHeight->setValue( 1440 );
	ui->pageSizeWidth->setValue( 1080 );
}

void MainWindow::on_predefLines15_clicked() {
	ui->textLinesPerPage->setValue( 15 );
}

void MainWindow::on_predefLines20_clicked() {
	ui->textLinesPerPage->setValue( 20 ) ;
}

void MainWindow::on_predefLines25_clicked() {
	ui->textLinesPerPage->setValue( 25 ) ;
}

void MainWindow::uIWidgetsSetDisabled(bool disabled) {
	ui->srcDirLabel->setDisabled( disabled );
	ui->srcDir->setDisabled( disabled );
	ui->srcDirButton->setDisabled( disabled );
	ui->destDirLabel->setDisabled( disabled );
	ui->destDir->setDisabled( disabled );
	ui->destDirButton->setDisabled( disabled );
	ui->tresh->setDisabled( disabled );
	ui->pageSize->setDisabled( disabled );
	ui->options->setDisabled( disabled );
	ui->textSize->setDisabled( disabled );
	ui->predef->setDisabled( disabled );
	ui->outFiles->setDisabled( disabled );
    ui->actionShowDebugImages->setDisabled( disabled );
}

void MainWindow::closeEvent(QCloseEvent * event) {
	if( ui->outStartButton->text() == stopText_ ) {
		if( QMessageBox::Yes == QMessageBox::question( this, tr("Stop?"), tr("Do you want to stop processing?") ) ) {
			processingThread_.work=false;
			processingThread_.wait();
			event->accept();
		} else {
			event->ignore();
		}
	} else {
		event->accept();
	}
}

void MainWindow::on_actionAbout_triggered() {
	QMessageBox box;

	box.setStandardButtons( QMessageBox::Ok );
	box.setEscapeButton( QMessageBox::Ok );//for closing by [x]
	box.setIcon( QMessageBox::Information );

	box.setWindowTitle( tr( "Re-Typeset" ) );
	box.setText( License::header() );
	box.setInformativeText( License::body() );
	box.setDetailedText( License::license() );

	box.exec();
}

void MainWindow::on_actionAboutQt_triggered() {
   QMessageBox::aboutQt( this, "Qt" );
}

void MainWindow::on_actionExit_triggered() {
    qApp->exit( 0 );
}


//MULTILANG: see mainwindow.hpp header
void MainWindow::switchTranslator(QTranslator & translator, const QString & filename) {
	qApp->removeTranslator(&translator); // remove the old translator
	if(translator.load(filename)) {// load the new translator
		qApp->installTranslator(&translator);
	}
}

QString MainWindow::changeFileToBasedir(QString dir) {
	QFileInfo fi( dir );
	if( fi.isDir() ) {
		return dir;
	} else {
		return fi.path();
	}
}

void MainWindow::loadLanguage(const QString & rLanguage) {
	if(currLang_ != rLanguage) {
		currLang_ = rLanguage;
		QLocale locale = QLocale(currLang_);
		QLocale::setDefault(locale);
		switchTranslator(translator_, QString("res/re-typeset_%1.qm").arg(rLanguage));
        switchTranslator(translatorQt_, QString("languages/qt_%1.qm").arg(rLanguage));
	}
}

void MainWindow::createLanguageMenu() {
	QActionGroup* langGroup = new QActionGroup(ui->menuLanguage);
	langGroup->setExclusive(true);
	connect(langGroup, SIGNAL(triggered(QAction *)), this, SLOT(slotLanguageChanged(QAction *)));
	QString defaultLocale = QLocale::system().name();       // e.g. "de_DE"	// format systems language
	defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"
	langPath_ = QApplication::applicationDirPath();
	langPath_.append("/res");
	QDir dir(langPath_);
	QStringList fileNames = dir.entryList(QStringList("re-typeset_*.qm"));
	if( fileNames.size() == 0 ) {
		ui->menuLanguage->setDisabled( true );
	} else {
		for (int i = 0; i < fileNames.size(); ++i) {
			QString locale;// get locale extracted by filename
			locale = fileNames[i];                  // "TranslationExample_de.qm"
			locale.truncate(locale.lastIndexOf('.'));   // "TranslationExample_de"
			locale.remove(0, locale.indexOf('_') + 1);   // "de"
			QString lang = QLocale::languageToString(QLocale(locale).language());
			QIcon ico(QString("%1/%2.png").arg(langPath_).arg(locale));
			QAction *action = new QAction(ico, lang, this);
			action->setCheckable(true);
			action->setData(locale);
			ui->menuLanguage->addAction(action);
			langGroup->addAction(action);
			if (defaultLocale == locale) {// set default translators and language checked
				action->setChecked(true);
			}
		}
	}
}

void MainWindow::changeEvent(QEvent * event) {
	if(0 != event) {
		switch(event->type()) {
		case QEvent::LanguageChange:// this event is send if a translator is loaded
			ui->retranslateUi(this);
		break;
		case QEvent::LocaleChange:	{// this event is send, if the system, language changes
			QString locale = QLocale::system().name();
			locale.truncate(locale.lastIndexOf('_'));
			loadLanguage(locale);
		}
		break;
		default:
			;//NOOP (supress warning)
		}
	}
	QMainWindow::changeEvent(event);
}

void MainWindow::slotLanguageChanged(QAction * action) {
	if(0 != action) {// load the language dependant on the action content
		loadLanguage(action->data().toString());
	}
}
//end MULTILANG
