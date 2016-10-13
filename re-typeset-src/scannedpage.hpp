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

#ifndef SCANNEDPAGE_HPP
#define SCANNEDPAGE_HPP
#include "scannedline.hpp"
#include <QImage>
#include <QFile>

class ScannedPage {
public:
	enum PageNumber { None, Top, Bottom };
	ScannedPage();
	ScannedPage( QString fileMono, QString fileColor );
	/*
	 *Pierwszy etap przetwarzania: podział na linie
	 */
	void getLinesAndHeights( Stats & stats );
	void getLinesAndHeights( Stats & stats, const QImage & image );
	/*
	 *Drugi etap: dołączenie odciętych kropek
	 */
	void reConnectDots( StatsPack stats );
	/*
	 *Trzeci etap: zbyt wysokie linie będą rozbite
	 */
	void trimDivideLines( StatsPack stats );
	void trimDivideLines( StatsPack stats, const QImage & image );

	void connectDescriptionsToImages( StatsPack stats );
	void connectDescriptionsToImages( StatsPack stats, const QImage & image );

	void findParagraphs( StatsPack stats );

	void findBaselines();
	void findBaselines(const QImage &image);

	void findDividedWords();
	void findDividedWords( const QImage & image );

	void checkNumberHeader( StatsPack stats, StatsNumberHeader & numHead );

	void setNumberHeader( StatsPack stats, PageNumber number, bool header_ );

	void printDEbugImages( StatsPack stats );
	QImage printDEbugImages( StatsPack stats, const QImage & image );

	void getNumHead(StatsPack stats, PrintedLine & numHead, double scalingRatio, int maxWordLength, bool fullColor=false );
	bool getParagraph(StatsPack stats, PrintedLine & paragraph, double scalingRatio, int maxWordLength, bool fullColor=false );//false==page completed

	void removeMonoImage();
protected:
	QString fileMono_;
	QString fileColor_;
	QVector < ScannedLine > lines_;
	int processedLine_;
	ScannedLine header_;
	ScannedLine pageNumber_;


};

#endif // SCANNEDPAGE_HPP
