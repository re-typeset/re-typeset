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

#include "extendedimage.hpp"
#include "hsvimage.hpp"

ExtendedImage::ExtendedImage()
{
	//NOOP
}

ExtendedImage::ExtendedImage(int width, int height, QImage::Format format)
	: QImage(width, height, format)
{
	//NOOP
}

ExtendedImage::ExtendedImage(const QString &fileName, const char *format)
	: QImage(fileName, format)
{
	//NOOP
}

ExtendedImage::ExtendedImage(const QImage &image)
	: QImage(image)
{
	//NOOP
}

ExtendedImage ExtendedImage::convertToMonoImage(int treshold)
{
	QImage imageMono( size(), QImage::Format_Mono );
	const uint white=0;
	const uint black=1;
	for( int x=0; x<width(); ++x ) {
		for( int y=0; y<height(); ++y ) {
			if( qGray( pixel( x, y ) ) > treshold ) {
				imageMono.setPixel( x, y, black );
			} else {
				imageMono.setPixel( x, y, white );
			}
		}
	}
	return imageMono;
}

ExtendedImage ExtendedImage::histogramEqualization()
{
	HSVImage hsvImage(*this);
	hsvImage.equalizeHistogram();
	return hsvImage.toQImage();
}
