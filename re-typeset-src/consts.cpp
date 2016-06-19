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

#include "consts.hpp"
#include <QApplication>

int min( int a, int b) {
	if( a<b) {
		return a;
	} else {
		return b;
	}
}

int max( int a, int b) {
	if( a>b) {
		return a;
	} else {
		return b;
	}
}



int Cconsts::MinHeightForOneVerticalDiv(StatsPack stats) {
//	return (int)( (double)(2*stats.height_ + stats.divToNextLine_)*1.2 ); no math formulas protection
	return (int)( (double)( 2*stats.height_ ) );
}

int Cconsts::LineHeightFromTextHeight(int h) {
	return h*6/5;
}

int Cconsts::TextHeightFromLineHeight(int h) {
	return h*5/6;
}

Cconsts::Cconsts() {
	QCoreApplication::quit();
}

int Cconsts::AccidentiallyConnectedLines::MaxHeight(StatsPack stats) {
	return (MaxNumberOfLines*stats.height_
			+ (MaxNumberOfLines-1)*stats.divToNextLine_)*1.1;
}

bool Cconsts::AccidentiallyConnectedLines::CanCut(StatsPack stats, int textWeight1, int spaceWeight1,
												  int textWeight2, int spaceWeight2, int textWeight3) {
	if( min( min( textWeight1, textWeight2 ), textWeight3 ) > max( spaceWeight1, spaceWeight2 )*TextToSpaceCoefficient
			&& spaceWeight1 < WeightOfConnectionInMedian*stats.height_
			&& spaceWeight2 < WeightOfConnectionInMedian*stats.height_ ) {
		return true;
	} else {
		return false;
	}
}

Cconsts::AccidentiallyConnectedLines::AccidentiallyConnectedLines() {
	QCoreApplication::quit();
}

Cconsts::DividedWord::DividedWord() {
	QCoreApplication::quit();
}

int Cconsts::Print::DotsPerMeter( int fontHeight ) {
	return 255*fontHeight;
}

const QFont Cconsts::Print::FontBold() {
	return QFont( "Serif", 10, QFont::Bold );
}

const QFont Cconsts::Print::Font() {
	return QFont( "Serif", 10, QFont::Normal );
}

const QFont Cconsts::Print::FontItalic()
{
	return QFont( "Serif", 11, QFont::Normal, true );
}

Cconsts::Print::Print() {
	QCoreApplication::quit();
}

Cconsts::Description::Description() {
	QCoreApplication::quit();
}

Cconsts::NumberHeader::NumberHeader() {
	QCoreApplication::quit();
}

Cconsts::Progress::Progress() {
	QCoreApplication::quit();
}


int round(double a) {
    int b=a;
    if( a-b > 0.5 ) {
        return a+1;
    } else {
        return a;
    }
}
