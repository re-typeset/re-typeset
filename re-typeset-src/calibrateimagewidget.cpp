#include "calibrateimagewidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include <QDebug>

CalibrateImageWidget::CalibrateImageWidget(QWidget *parent)
    : QWidget(parent)
    , image_(Q_NULLPTR)
    , imageZoom_(1)
    , secondWidget_(Q_NULLPTR)
{

}

void CalibrateImageWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);
    if( image_ != Q_NULLPTR ) {
        QSize widgetSize = this->size() / realZoom(imageZoom_);
        QPoint widgetCorner = -imageCorner_ / realZoom(imageZoom_);
        QRect cutArea(widgetCorner, widgetSize);

        QImage image = (image_->copy(cutArea)).scaled(this->size(), Qt::KeepAspectRatio);
        p.drawImage(0,0, image);
    }
}

void CalibrateImageWidget::wheelEvent(QWheelEvent *e)
{
    int delta=0;
    if( e->angleDelta().y() > 0 ) {
        if( this->size().width() / realZoom(imageZoom_) > 10 ) {
            delta=1;
        }
    } else {
        if( image_->width() * realZoom(imageZoom_) > this->width() * 0.9 ) {
            delta=-1;
        }
    }

    if( 0 != delta ) {
        QPoint posInImageOnWidgetScale = e->pos() - imageCorner_;
        QPoint posInImageOnImageScale = posInImageOnWidgetScale / realZoom(imageZoom_);

        imageZoom_ += delta;

        QPoint posInNewImageOnWidgetScale = posInImageOnImageScale * realZoom(imageZoom_);
        imageCorner_ = e->pos() - posInNewImageOnWidgetScale;

        repaint();
        if( secondWidget_ != Q_NULLPTR ) {
            secondWidget_->imageZoom_=imageZoom_;
            secondWidget_->imageCorner_=imageCorner_;
            secondWidget_->repaint();
        }
    }
}

void CalibrateImageWidget::mousePressEvent(QMouseEvent *e)
{
    oldImageCorner_=imageCorner_;
    mousePressPoint_=e->pos();
    if( secondWidget_ != Q_NULLPTR ) {
        secondWidget_->imageCorner_=imageCorner_;
        secondWidget_->repaint();
    }
}

void CalibrateImageWidget::mouseMoveEvent(QMouseEvent *e)
{
    imageCorner_=oldImageCorner_ + (e->pos() - mousePressPoint_);
    repaint();
    if( secondWidget_ != Q_NULLPTR ) {
        secondWidget_->imageCorner_=imageCorner_;
        secondWidget_->repaint();
    }
}



void CalibrateImageWidget::mouseReleaseEvent(QMouseEvent *e)
{
    imageCorner_=oldImageCorner_ + (e->pos() - mousePressPoint_);
    repaint();
    if( secondWidget_ != Q_NULLPTR ) {
        secondWidget_->imageCorner_=imageCorner_;
        secondWidget_->repaint();
    }
}

void CalibrateImageWidget::setSecondWidget(CalibrateImageWidget *secondWidget)
{
    if( secondWidget!= this ) {
        secondWidget_ = secondWidget;
    }
}

void CalibrateImageWidget::setZoomNormal()
{
    imageZoom_=0;
    imageCorner_=QPoint();
    repaint();
    if( secondWidget_ != Q_NULLPTR ) {
        secondWidget_->imageZoom_=imageZoom_;
        secondWidget_->imageCorner_=imageCorner_;
        secondWidget_->repaint();
    }
}

void CalibrateImageWidget::setZoomFit()
{
    imageZoom_=reverseZoom( (double)this->height()/image_->height() );
    imageCorner_=QPoint();
    repaint();
    if( secondWidget_ != Q_NULLPTR ) {
        secondWidget_->imageZoom_=imageZoom_;
        secondWidget_->imageCorner_=imageCorner_;
        secondWidget_->repaint();
    }
}

void CalibrateImageWidget::setImage(QImage *image)
{
    image_ = image;
}

double CalibrateImageWidget::realZoom(double zoom) {
    return qPow( 10, zoom/10 );
}

double CalibrateImageWidget::reverseZoom(double realZoom) {
    //return ( std::log10( realZoom ) ) * 10;
    return ( qLn(realZoom)/qLn(10) ) * 10; //log_a_b = log_c_b/log_c_a
}
