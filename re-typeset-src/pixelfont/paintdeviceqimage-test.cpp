#include <gmock/gmock.h>

#include "paintdeviceqimage.hpp"

class PaintDeviceQImageTest : public ::testing::Test {
public:
	const int Width = 123;
	const int Height = 321;
	PaintDeviceQImageTest()
		: img(Width, Height, QImage::Format_RGB32 )
	{
		//NOOP
	}
	QImage img;
};

TEST_F(PaintDeviceQImageTest, getParams)
{
	EXPECT_EQ( img.width(), Width );
	EXPECT_EQ( img.height(), Height );

	PaintDeviceQImage pdqi( img );
	LettersWriter::PaintDevice * lwpd = &pdqi;

	EXPECT_EQ( lwpd->width(), Width );
	EXPECT_EQ( lwpd->height(), Height );
}

TEST_F(PaintDeviceQImageTest, putPixel)
{
	const QRgb Red = QColor( 255, 0, 0 ).rgb();
	const QRgb Green = QColor( 0, 255, 0 ).rgb();
	QPoint p( Width/2, Height/2 );

	img.setPixel( p, Green );

	EXPECT_EQ( img.pixel( p ), Green );

	PaintDeviceQImage pdqi( img );
	LettersWriter::PaintDevice * lwpd = &pdqi;

	lwpd->setPixel( p, Red );

	EXPECT_EQ( img.pixel( p ), Red );
}

