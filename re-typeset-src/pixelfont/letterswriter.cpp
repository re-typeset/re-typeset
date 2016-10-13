#include "letterswriter.hpp"
#include <QDebug>


namespace {
const unsigned char SpaceBetweenLetters = 1;
}

LettersWriter::LettersWriter(LettersWriter::PaintDevice &pd)
	: pd_(pd)
{
	//NOOP
}
bool LettersWriter::write(const QString text, QRect area, int color, Direction direction)
{
	if( direction == Horizontal ) {
		return writeH( text, area, color);
	} else {
		return writeV( text, area, color);
	}
}

bool LettersWriter::writeH(const QString text, QRect area, int color)
{
	bool ret=true;
	int textLength = calculateLength(text);
	int leftSpace = 0;
	char yRange = qMin( area.height(), pd_.height() );
	QRect devArea( 0, 0, pd_.width(), pd_.height() );
	QRect writeArea = devArea & area;
	if( textLength < writeArea.width() ) {
		leftSpace = (writeArea.width() - textLength)/2;
		writeArea.setLeft( writeArea.left() + leftSpace );
	}
	int x=0;
	for( int i=0; i<text.length(); ++i ) {
		char c = text[i].unicode();
		if( c == ' ' ) {
			x += Letters::SpaceColumns;
		} else {
			Letters::Letter l;
			l = Letters::getLetter( c );
			char coln = l.numColumns();
			for( unsigned char y=0; y<yRange; ++y ) {

				for( char inLetterPos = coln; inLetterPos > 0; --inLetterPos ) {
					unsigned char mask = 1 << (inLetterPos - 1);
					if( mask & l.lines[y] ) {
						QPoint pos = writeArea.topLeft();
						pos.rx() += x + (coln-inLetterPos);
						pos.ry() += y;
						if( writeArea.contains(pos) ) {
							pd_.setPixel( pos, color );
						} else {
							ret = false;
						}
					}
				}
			}
			x+=coln;
		}
		x+=SpaceBetweenLetters;
	}
	return ret;
}

bool LettersWriter::writeV(const QString text, QRect area, int color)
{
	bool ret=true;
	int textLength = calculateLength(text);
	int bottomSpace = 0;
	char xRange = qMin( area.width(), pd_.width() );
	QRect devArea( 0, 0, pd_.width(), pd_.height() );
	QRect writeArea = devArea & area;
	if( textLength < writeArea.height() ) {
		bottomSpace = (writeArea.height() - textLength)/2;
		writeArea.setBottom( writeArea.bottom() - bottomSpace );
	}
	int y=0;
	for( int i=0; i<text.length(); ++i ) {
		char c = text[i].unicode();
		if( c == ' ' ) {
			y += Letters::SpaceColumns;
		} else {
			Letters::Letter l;
			l = Letters::getLetter( c );
			char coln = l.numColumns();
			for( unsigned char x=0; x<xRange; ++x ) {

				for( char inLetterPos = coln; inLetterPos > 0; --inLetterPos ) {
					unsigned char mask = 1 << (inLetterPos - 1);
					if( mask & l.lines[x] ) {
						QPoint pos = writeArea.bottomLeft();
						pos.rx() += x ;
						pos.ry() += inLetterPos -y -coln;
						if( writeArea.contains(pos) ) {
							pd_.setPixel( pos, color );
						} else {
							ret = false;
						}
					}
				}
			}
			y+=coln;
		}
		y+=SpaceBetweenLetters;
	}
	return ret;
}

int LettersWriter::calculateLength(const QString text)
{
	int len=0;
	for( int i=0; i<text.length(); ++i ) {
		char c = text[i].unicode();
		if( c == ' ' ) {
			len += Letters::SpaceColumns;
		} else {
			len += Letters::getLetter( c ).numColumns();
		}
		len += SpaceBetweenLetters;
	}
	len -= SpaceBetweenLetters;
	if( len < 0 ) {
		len =0;
	}
	return len;
}
