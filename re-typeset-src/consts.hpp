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
#define DE qDebug()



int min( int a, int b);

int max( int a, int b);

int round( double a );

class Consts {
public:

	/*
	 * linia o takiej zawartości czarnych punktów jest
	 * uznawana za białą.
	 * Ogonek przy 200 dpi ma 2 px grubości, a słowo
	 * pające 6 liter ma 110 px długości.
	 * Na 1000 px 3 ogonki, 2 będą usunięte
	 */
	//static const double MaxBlackPointsInLine = 0.5/100;

	/*
	 * minimalna wysokośc lini, aby była ona uznana za tekstową,
	 * w ułamku wartości mediany
	 */
	static const double MinHeightForLineInMedian = 0.33;

	/*
	 * podział na kolumny i słowa
	 */
	static const double MinHorizontalSpaceInMedian = 1.0/5;//1.0/4;

	/*
	 * doklejanie odciętych kropek,
	 * linia zbyt wąska + linia == wysokość nie większa niż
	 */
	static const double MaxHeightForJoinedLineInHerHeight = 1.4;


	static int MinHeightForOneVerticalDiv( StatsPack stats );


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
	static const double MinHeightForFigureInMedian = 2;

	static const double MinParagraphIndentInMedian=3.0/4;
	static const double MaxParagraphIndentInMedian=3.0;


	static int LineHeightFromTextHeight( int h );
	static int TextHeightFromLineHeight( int h );

	static const double MinHeightForEmptyLineInSpaceToNext=3.0;

	static const double MaxWordLengthInOutPageWidth=0.9;



	/*
	 * połączone przypadkowo linie np. przez kropki na obrazie
	 * lub zbyt długie ogonki, rozcinane niepełnym cięciem
	 * CscannedLine::cutAccidentiallyConnectedLines()
	 */
	class AccidentiallyConnectedLines {
	public:
		/*
		 * z tej wartości wyliczamy max grubość połączenia lini
		 */
		static const double WeightOfConnectionInMedian = 0.5;//1.0; //1.0/4;
		/*
		 * maksymalna wysokość bloku, który może byc rozbijany przez
		 * CscannedLine::cutAccidentiallyConnectedLines()
		 */
		static int MaxHeight( StatsPack stats );

		static bool CanCut( StatsPack stats, int textWeight1, int spaceWeight1, int textWeight2, int spaceWeight2=-1, int textWeight3=999999 );

		static const int TextToSpaceCoefficient = 10;

		static const int MaxNumberOfLines = 3;
	private:
		AccidentiallyConnectedLines();
	};

	class DividedWord {
	public:
		static const double UpperGap=11.0/31;//10.0/31//11.0/31
		static const double LowerGap=9.0/31;
		static const double MaxHeight=0.2;//0.15;
		static const double MinLength=1.0/6;
		static const double MaxLength=1.0/3;
	private:
		DividedWord();
	};

	class Print {
	public:
		static int DotsPerMeter(int fontHeight );
		static const QFont FontBold();
		static const QFont Font();
		static const QFont FontItalic();
		static const double SpaceInMedian=1.0/2;
		static const double SpaceToNextLineInMedian=1.0/5;
		static const double ParagraphIndentInMedian=2.0;
		static const double SpaceAfterNumHeadInMedian=1.0/2;
		static const int MaxImagesQueueSize=10;
		static const double ProgressBarDiv=0.8;
		static const double ProgressBarHeight=0.4;
		static const double ProgressBarGap=0.3;
        static const double SpaceBetweenLetters=2.0/32;
		static const int TypicalWatermarkLength=180;
	private:
		Print();
	};

	class Description {
	public:
		static const double MaxHeightInMedian=4;
		static const double MinSpaceAfterInMedian=0.8;
	private:
		Description();
	};

	class NumberHeader {
	public:
		static const double CorrectToAllCoefficient = 0.5;
		static const double MinSpaceToTextInSpace = 2.0;
		static const double MaxLengthOfNumberInMedian = 3.5;
	private:
		NumberHeader();
	};

	class Progress {
	public:
		static const int LoadPercent=12;
		static const int LoadDelay=0;
		static const int LoadNumber=1;

		static const int FindPercent=10;
		static const int FindDelay=12;
		static const int FindNumber=2;

		static const int PrintPercent=74;
		static const int PrintDelay=22;
		static const int PrintNumber=3;

		static const int ProgBarPercent=4;
		static const int ProgBarDelay=96;
		static const int ProgBarNumber=4;

		static const int TotalNumber=4;
	private:
		Progress();
	};
private:
	Consts();
};

#endif // CONSTS_HPP
