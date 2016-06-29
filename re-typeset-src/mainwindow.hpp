// Re-Typeset program for converting scanned documents to smaller size of page
// without changing text size.
//
// Copyright (C) 2013-2015 Piotr Mika (piotr.mt.mika@gmail.com).
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

// MULTILANG:  Translation  implementation is based on Qt example presented on
// page http://qt-project.org/wiki/How_to_create_a_multi_language_application.
// Content  is available  under  ``Creative Commons Attribution-ShareAlike 2.5
// Generic'', see: http://creativecommons.org/licenses/by-sa/2.5/.

#ifndef MAINWINDOW_CPP
#define MAINWINDOW_CPP

#include <QTranslator>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "scanneddocument.hpp"
#include <QCloseEvent>
#include "processingthread.hpp"
#include "calibratewindow.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void setProgressBar(int percent, QString name, int n=0 , int of=0);
	void processingFinished();

private slots:
	void on_srcDirButton_clicked();
	void on_treshSlider_sliderMoved(int position);
	void on_treshVal_valueChanged(int arg1);
	void on_pageSizeHeight_valueChanged(int arg1);
	void on_textSizeHeight_valueChanged(int arg1);
	void on_textLinesPerPage_valueChanged(int arg1);
	void on_treshSlider_valueChanged(int value);
	void on_pageSizeMargin_valueChanged(int arg1);
	void on_outStartButton_clicked();
	void on_destDirButton_clicked();
	void on_predefScreen800_clicked();
	void on_predefScreen1024_clicked();
	void on_predefScreen1440_clicked();
	void on_predefLines15_clicked();
	void on_predefLines20_clicked();
	void on_predefLines25_clicked();
	void on_destDirCreate_clicked();

    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();

    void on_actionExit_triggered();

	void on_optionsCalibrateAlg_clicked();

private:
	Ui::MainWindow *ui;
	QFileDialog::Option QFileDialog_ShowAllFiles; //No '0' value in 'enum Option' in 'QFileDialog' class
	CalibrateWindow * calibrate_;

//MULTILANG
protected slots:
	void slotLanguageChanged(QAction* action);
private:
	void loadLanguage(const QString& rLanguage);
	void createLanguageMenu(void);
	QTranslator translator_; //contains the translations for this application
	QTranslator translatorQt_; //contains the translations for qt
	QString currLang_; //contains the currently loaded language 
	QString langPath_; //Path of language files. This is always fixed to /languages.
protected:
	void changeEvent( QEvent * event );
	void switchTranslator(QTranslator & translator, const QString & filename);
//end MULTILANG

private:
    QString startText_;
	QString stopText_;
	ProcessingThread processingThread_;
	QString changeFileToBasedir( QString dir );

protected:
	void uIWidgetsSetDisabled( bool disabled );
	void closeEvent( QCloseEvent * event );

};

#endif // MAINWINDOW_CPP
