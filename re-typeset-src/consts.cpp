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

#include "consts.hpp"
#include <QApplication>

int Consts::MinHeightForOneVerticalDiv(StatsPack stats) {
//	return (int)( (double)(2*stats.height_ + stats.divToNextLine_)*1.2 ); no math formulas protection
	return (int)( (double)( 2*stats.height_ ) );
}

int Consts::LineHeightFromTextHeight(int h) {
	return h*6/5;
}

int Consts::TextHeightFromLineHeight(int h) {
	return h*5/6;
}

int Consts::AccidentiallyConnectedLines::MaxHeight(StatsPack stats) {
	return (MaxNumberOfLines*stats.height_
			+ (MaxNumberOfLines-1)*stats.divToNextLine_)*1.1;
}

bool Consts::AccidentiallyConnectedLines::CanCut(StatsPack stats, int textWeight1, int spaceWeight1,
												  int textWeight2, int spaceWeight2, int textWeight3) {
	if( qMin( qMin( textWeight1, textWeight2 ), textWeight3 ) > qMax( spaceWeight1, spaceWeight2 )*TextToSpaceCoefficient
			&& spaceWeight1 < WeightOfConnectionInMedian*stats.height_
			&& spaceWeight2 < WeightOfConnectionInMedian*stats.height_ ) {
		return true;
	} else {
		return false;
	}
}


int Consts::Print::DotsPerMeter( int fontHeight ) {
	return 255*fontHeight;
}

const QFont Consts::Print::FontBold() {
	return QFont( "Serif", 10, QFont::Bold );
}

const QFont Consts::Print::Font() {
	return QFont( "Serif", 10, QFont::Normal );
}

const QFont Consts::Print::FontItalic()
{
	return QFont( "Serif", 11, QFont::Normal, true );
}

static QString tr(const char * sourceText) {
	return QObject::tr(sourceText);
}

void Consts::configure(QFormLayout * l)
{
	QString median=tr("Median of line height");
	MinHeightForLineInMedian.configure(l, tr("Minimal line height"), 0.33, median);

	MinHorizontalSpaceInMedian.configure(l, tr("Minimal horizontal space"),1.0/5, median);

	MinHeightForFigureInMedian.configure(l, tr("Minimal height of figure"),  2, median);

	MinParagraphIndentInMedian.configure(l, tr("Minimal paragraph indent"), 3.0/4, median);
	MaxParagraphIndentInMedian.configure(l, tr("Maximum paragraph indent"), 3.0, median);

	MinHeightForEmptyLineInSpaceToNext.configure(l, tr("Minimal height for empty line"),3.0, tr("Median of space between lines"));


}

VariableParameter Consts::MinHeightForLineInMedian;
VariableParameter Consts::MinHorizontalSpaceInMedian;
VariableParameter Consts::MinHeightForFigureInMedian;
VariableParameter Consts::MinParagraphIndentInMedian;
VariableParameter Consts::MaxParagraphIndentInMedian;
VariableParameter Consts::MinHeightForEmptyLineInSpaceToNext;
