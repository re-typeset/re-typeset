#ifndef PAINTDEVICEQIMAGE_H
#define PAINTDEVICEQIMAGE_H
#include "letterswriter.hpp"
#include <QImage>

class PaintDeviceQImage : public LettersWriter::PaintDevice
{
public:
	PaintDeviceQImage( QImage & image );
	virtual void setPixel(const QPoint &position, uint index_or_rgb);
	virtual int width() const;
	virtual int height() const;
private:
	QImage & image_;
};

#endif // PAINTDEVICEQIMAGE_H
