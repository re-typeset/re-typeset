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

#ifndef SCANNEDLINE_HPP
#define SCANNEDLINE_HPP
#include <QVector>
#include <QString>
#include <QRect>
#include <QColor>
#include <QPainter>
#include "consts.hpp"
#include "stats.hpp"
#include "printedline.hpp"


class ScannedLine: public QRect {
public:
	ScannedLine();

	bool isImage_;
	bool isNewParagraph_;
	bool hasDividedWordAtEnd_;
	bool toDelete_;
	bool toDivide_;
	int  blockLeftPos_;
	int spaceToNextLine_;
	int upperBaseline_;
	int lowerBaseline_;

	void setNextLineTop(int value);

	bool trim( const QImage & image );

	int blackPixelsInRow( const QImage & image, int y );

	QVector < ScannedLine > collapseLine(const QImage & image, StatsPack stats );

	bool cutAccidentiallyConnectedLines( const QImage & image, StatsPack stats, QVector < ScannedLine > & out );
	void preciseDivideHorizontal( const QImage & image, StatsPack stats, QVector < ScannedLine > & out, int level=0);
	void preciseDivideVertical( const QImage & image, StatsPack stats, QVector < ScannedLine > & out, int level=0 );
	void checkIfHasDividedWordAtEnd( const QImage & image);
	void findBaselines(const QImage & image );

	void getWords( const QImage & imageMono, const QImage & imageColor, StatsPack stats,
				   PrintedLine & par , double scalingRatio, int maxWordLength, bool fullColor=false);

};

#endif // SCANNEDLINE_HPP
