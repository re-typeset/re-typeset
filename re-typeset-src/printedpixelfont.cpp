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

#include "printedpixelfont.hpp"

PrintedPixelFont::PrintedPixelFont() {
}

const bool PrintedPixelFont::Mask[5][197] = {
	{0,1,1,0,0,0,1,0,0,1,1,0,0,1,1,0,0,0,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,0,0,1,1,0,1,1,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,0,0,1,1,1,0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,0,1,0,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,0,0,1,0,0,1,1,0,0,1,0,0,1},
	{1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,1,0},
	{1,0,0,1,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,0,0,1,1,0,0,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0,1,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,1,0,1,0,1,0,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,1,0,1,0,0,1,0,1,0,0,1,0,1,1,1,0,0,1,0,1,0,0,1,0,1,1,0,0},
	{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,0,1,1,0,0,1,0,0,0,0,0,1,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1,1,1,1,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,1,0,1,1,1,1,0,1,1,1,0,0,1,0,0,1,0,1,0,1,1,1,1,0,1,0,1,0},
	{0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,0,0,1,0,0,1,1,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,1,1,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,0,1,1,0,0,1,0,0,1,0,1,0,0,1,0,1,1,1,0,0,1,0,1,0,0,1,0,1,0,0,1}
};

QPoint PrintedPixelFont::putText(QImage & image, QPoint startPoint, PrintedPixelFont::Text text, int val)
{
	int dx = 0;
	int endX = 0;
	if( text == Number ) {
		dx=val*4;
		endX=4;
	} else if( text == Przeskladv ) {
		dx=40;
		endX=51;
	} else if( text == cPiotrMika ) {
		dx=91;
		endX=103;
	} else if( text == Dot ) {
		dx=89;
		endX=1;
	}

	for( int x=0; x<endX; ++x ) {
		for( int y=0; y<TextHeight; ++y ) {
			if( Mask[y][dx+x] ) {
				QPoint p( startPoint.x()+x, startPoint.y()+y );
				if( image.rect().contains( p ) ) {
					image.setPixel( p, QColor( 224, 224, 224 ).rgb() );
				}
			}
		}
	}
	startPoint.rx() += endX;
	return startPoint;
}
