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

#ifndef CONSTS_HPP
#define CONSTS_HPP
#include <QDebug>
#include <QFont>
#include "stats.hpp"
#include "variableparameter.hpp"
#define DE qDebug()


namespace Consts {
void configure(QFormLayout * l);


/*
	 * minimalna wysokośc lini, aby była ona uznana za tekstową,
	 * w ułamku wartości mediany
	 */
extern VariableParameter MinHeightForLineInMedian;

/*
	 * podział na kolumny i słowa
	 */
extern VariableParameter MinHorizontalSpaceInMedian;

/*
	 * doklejanie odciętych kropek,
	 * linia zbyt wąska + linia == wysokość nie większa niż
	 */
static constexpr double MaxHeightForJoinedLineInHerHeight=1.4;


int MinHeightForOneVerticalDiv( StatsPack stats );


/*
	 * linia o większej wysokości niż 5*med(linia.wys)
	 * powinna być pocięta w pionie, a kazdy z elementów
	 * powinien być dalej dzielony na linie.
	 * elementy niedzielone oznaczyć jako obraz.
	 * W przypadku podziału obraz|tekst lub tekst|obraz
	 * rozważyć oblewanie obrazu tekstem.
	 * Uwaga: jeśli jest podział A|B|C to ilość lini
	 * w sąsiadujących musi być różna
	 */
extern VariableParameter MinHeightForFigureInMedian;

extern VariableParameter MinParagraphIndentInMedian;
extern VariableParameter MaxParagraphIndentInMedian;


int LineHeightFromTextHeight( int h );
int TextHeightFromLineHeight( int h );

extern VariableParameter MinHeightForEmptyLineInSpaceToNext;

static constexpr double MaxWordLengthInOutPageWidth=0.9;



/*
	 * połączone przypadkowo linie np. przez kropki na obrazie
	 * lub zbyt długie ogonki, rozcinane niepełnym cięciem
	 * CscannedLine::cutAccidentiallyConnectedLines()
	 */
namespace AccidentiallyConnectedLines {
/*
		 * z tej wartości wyliczamy max grubość połączenia lini
		 */
static constexpr double WeightOfConnectionInMedian = 0.5;//1.0; //1.0/4;
/*
		 * maksymalna wysokość bloku, który może byc rozbijany przez
		 * CscannedLine::cutAccidentiallyConnectedLines()
		 */
int MaxHeight( StatsPack stats );

bool CanCut( StatsPack stats, int textWeight1, int spaceWeight1, int textWeight2, int spaceWeight2=-1, int textWeight3=999999 );

static constexpr int TextToSpaceCoefficient = 10;

static constexpr int MaxNumberOfLines = 3;
}
namespace DividedWord {
static constexpr double UpperGap=11.0/31;//10.0/31//11.0/31
static constexpr double LowerGap=9.0/31;
static constexpr double MaxHeight=0.2;//0.15;
static constexpr double MinLength=1.0/6;
static constexpr double MaxLength=1.0/3;
}

namespace Print {
int DotsPerMeter(int fontHeight );
const QFont FontBold();
const QFont Font();
const QFont FontItalic();
static constexpr double SpaceInMedian=1.0/2;
static constexpr double SpaceToNextLineInMedian=1.0/5;
static constexpr double ParagraphIndentInMedian=2.0;
static constexpr double SpaceAfterNumHeadInMedian=1.0/2;
static constexpr int MaxImagesQueueSize=10;
static constexpr double ProgressBarDiv=0.8;
static constexpr double ProgressBarHeight=0.4;
static constexpr double ProgressBarGap=0.3;
static constexpr double SpaceBetweenLetters=2.0/32;
static constexpr int TypicalWatermarkLength=180;
}

namespace Description {
static constexpr double MaxHeightInMedian=4;
static constexpr double MinSpaceAfterInMedian=0.8;
}

namespace NumberHeader {
static constexpr double CorrectToAllCoefficient = 0.5;
static constexpr double MinSpaceToTextInSpace = 2.0;
static constexpr double MaxLengthOfNumberInMedian = 3.5;
}

namespace Progress {
static constexpr int LoadPercent=12;
static constexpr int LoadDelay=0;
static constexpr int LoadNumber=1;

static constexpr int FindPercent=10;
static constexpr int FindDelay=12;
static constexpr int FindNumber=2;

static constexpr int PrintPercent=74;
static constexpr int PrintDelay=22;
static constexpr int PrintNumber=3;

static constexpr int ProgBarPercent=4;
static constexpr int ProgBarDelay=96;
static constexpr int ProgBarNumber=4;

static constexpr int TotalNumber=4;
}

}

#endif // CONSTS_HPP
