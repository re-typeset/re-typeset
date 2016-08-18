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

#ifndef CALIBRATEWINDOW_HPP
#define CALIBRATEWINDOW_HPP

#include <QDialog>
#include <QFormLayout>
#include <QImage>

namespace Ui {
class CalibrateWindow;
}

class CalibrateWindow : public QDialog
{
	Q_OBJECT

public:
	explicit CalibrateWindow(QWidget *parent = 0);
	~CalibrateWindow();

	QFormLayout * getParametersFormLayout();

	void setDirPath(const QString &dirPath);

	void setTreshold(int treshold);
	void connectSpinBoxes();

public slots:
	void runAlgortithm();
	void setZoomFit();

private slots:
	void on_previousImageButton_clicked();

	void on_nextImageButton_clicked();

private:
	void reloadImage();



	Ui::CalibrateWindow *ui;
	int imageNum_;
	int treshold_;
	QString dirPath_;
	QImage inputImage_;
	QImage outputImage_;

};

#endif // CALIBRATEWINDOW_HPP
