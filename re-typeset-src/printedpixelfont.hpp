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

#ifndef PRINTEDPIXELFONT_HPP
#define PRINTEDPIXELFONT_HPP
#include <QImage>
#include <QPoint>
#include <QColor>

class PrintedPixelFont
{
public:
	PrintedPixelFont();
	enum Text { Number, Przeskladv, cPiotrMika, In, Out, Num, Dot, Comma, Colon };
	static const int TextHeight=5;
	static const int Space=4;

	static const bool Mask[5][197];


	static QPoint putText( QImage & image, QPoint startPoint, Text text, int val=0 );
};

#endif // PRINTEDPIXELFONT_HPP
