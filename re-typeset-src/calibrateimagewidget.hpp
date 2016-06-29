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

#ifndef CALIBRATEIMAGEWIDGET_H
#define CALIBRATEIMAGEWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QImage>

class CalibrateImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalibrateImageWidget(QWidget *parent = 0);

    void setImage(QImage *image); 

    void setSecondWidget(CalibrateImageWidget *secondWidget);



signals:

public slots:
    void setZoomNormal();
    void setZoomFit();

private:
    void paintEvent(QPaintEvent * e);
    void wheelEvent(QWheelEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);

    QImage * image_;
    int imageZoom_;
    QPoint imageCorner_;
    QPoint oldImageCorner_;
    QPoint mousePressPoint_;
    CalibrateImageWidget * secondWidget_;

    double realZoom(double zoom);
    double reverseZoom(double realZoom);
};

#endif // CALIBRATEIMAGEWIDGET_H
