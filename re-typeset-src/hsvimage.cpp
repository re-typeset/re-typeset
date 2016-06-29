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

#include "hsvimage.hpp"
#include <QColor>

HSVImage::HSVImage()
    : width_(0)
    , height_(0)
{
    //NOOP
}

HSVImage::HSVImage(const QImage & img)
    : width_(img.width())
    , height_(img.height())
{
    data_.resize(img.width() * img.height());
    for( int x=0; x<img.width(); ++x ) {
        for( int y=0; y<img.height(); ++y ) {
            data_[xy2index(x,y)] = HSV(img.pixel(x, y));
        }
    }
}

void HSVImage::setPixel(int x, int y, HSVImage::HSV hsv)
{
    data_[xy2index(x,y)] = hsv;
}

HSVImage::HSV HSVImage::getPixel(int x, int y) const
{
    return data_[xy2index(x,y)];
}

void HSVImage::equalizeHistogram()
{
    const int SIZE = 256;
    const int MAXVAL=SIZE-1;
    const double SAT=0.2;

    int sat_p[SIZE] = {0};
    int sat_cdf[SIZE] = {0};
    int val_p[SIZE] = {0};
    int val_cdf[SIZE] = {0};


    for( int x=0; x<width_; ++x ) {
        for( int y=0; y<height_; ++y ) {
            ++(sat_p[ data_[xy2index(x,y)].saturation ]);
            ++(val_p[ data_[xy2index(x,y)].value ]);
        }
    }
    int sat_sum=0;
    int val_sum=0;
    for( int i=0; i<SIZE; ++i ) {
        sat_sum += sat_p[i];
        sat_cdf[i] = sat_sum;

        val_sum += val_p[i];
        val_cdf[i] = val_sum;
    }
    int num_pix = width_ * height_;

    for( int x=0; x<width_; ++x ) {
        for( int y=0; y<height_; ++y ) {
            quint8 sat1 = data_[xy2index(x,y)].saturation;
            quint8 sat2 = ((double)sat_cdf[sat1]) * MAXVAL / num_pix;
            quint8 sat = SAT*sat2 +(1-SAT)*sat1;
            data_[xy2index(x,y)].saturation = sat;

            quint8 val = data_[xy2index(x,y)].value;
            data_[xy2index(x,y)].value = ((double)val_cdf[val]) * MAXVAL / num_pix;
        }
    }

}

QImage HSVImage::toQImage() const
{
    QImage img(width_, height_, QImage::Format_RGB32);
    for( int x=0; x<img.width(); ++x ) {
        for( int y=0; y<img.height(); ++y ) {
            img.setPixel(x, y, data_[xy2index(x,y)].toQRgb());
        }
    }
    return img;
}

int HSVImage::xy2index(int x, int y) const
{
    return width_ * y + x;
}


HSVImage::HSV::HSV()
{
    //NOOP
}

HSVImage::HSV::HSV(QRgb pixel)
{
    QColor c( pixel );
    hue = c.hue();
    saturation = c.saturation();
    value = c.value();
}

QRgb HSVImage::HSV::toQRgb() const
{
    QColor c = QColor::fromHsv(hue, saturation, value);
    return c.rgb();
}
