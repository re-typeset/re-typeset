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

#ifndef SCANNEDDOCUMENT_HPP
#define SCANNEDDOCUMENT_HPP
#include "scannedpage.hpp"
#include "printedpage.hpp"
#include <QDir>
#include <QObject>

class ScannedDocument: public QObject {
	Q_OBJECT
public:
	ScannedDocument();
	ScannedDocument(QString srcDir, QString destDir, bool * work, bool DEbugState=false);
	~ScannedDocument();
	/*
	 * Search directory and create page object for every file in this directory
	 */
	int loadPages( int treshold );

	/*
	 * Search words in document
	 */
	void findWords( bool comicMode=false, bool findDividedWords=true );

	int print( int width, int height, int margin, int fontHeight, bool hardMargins, bool noUpscalling,
			   bool fullColor, bool justify, bool rotateImages, bool comicMode, QString fileNamePrefix );

signals:
	void newProgressBarValue(int percent, QString name="", int n=0, int of=0);

protected:
	QString srcDir_;
	QString destDir_;
	Stats lineStats_;
	bool * work_;
	bool DEbugState_;
	ScannedPage::PageNumber number_;
	bool header_;

	QVector < ScannedPage > pages_;


};


#endif // SCANNEDDOCUMENT_HPP
