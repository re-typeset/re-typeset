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
