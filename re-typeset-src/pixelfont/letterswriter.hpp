#ifndef LETTERSWRITER_H
#define LETTERSWRITER_H

#include <QPoint>
#include <QRect>
#include <QString>
#include "letters.hpp"


class LettersWriter
{
public:
	class PaintDevice
	{
	public:
		virtual void setPixel(const QPoint &position, uint index_or_rgb) = 0;
		virtual int width() const = 0;
		virtual int height() const = 0;
	};
	enum Direction {
		Horizontal,
		Vertical,
	};

	LettersWriter( PaintDevice & pd );

	bool write( const QString text, QRect area, int color, Direction direction=Horizontal);
	static int calculateLength(const QString text);

private:
	PaintDevice & pd_;
	bool writeH( const QString text, QRect area, int color);
	bool writeV( const QString text, QRect area, int color);

};

#endif // LETTERSWRITER_H
