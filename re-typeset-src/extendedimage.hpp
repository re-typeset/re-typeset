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

#ifndef EXTENDEDIMAGE_H
#define EXTENDEDIMAGE_H
#include <QImage>

class ExtendedImage : public QImage
{
public:
	ExtendedImage();
	ExtendedImage(int width, int height, Format format);
	ExtendedImage(const QString & fileName, const char * format = 0);
	ExtendedImage(const QImage & image);

	ExtendedImage convertToMonoImage(int treshold);
	ExtendedImage histogramEqualization();
};

#endif // EXTENDEDIMAGE_H
