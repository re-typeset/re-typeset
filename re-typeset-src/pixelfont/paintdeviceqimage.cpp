#include "paintdeviceqimage.hpp"


PaintDeviceQImage::PaintDeviceQImage(QImage & image)
    :image_(image)
{
    //NOOP
}

void PaintDeviceQImage::setPixel(const QPoint &position, uint index_or_rgb)
{
    image_.setPixel(position, index_or_rgb);
}

int PaintDeviceQImage::width() const
{
    return image_.width();
}

int PaintDeviceQImage::height() const
{
    return image_.height();
}
