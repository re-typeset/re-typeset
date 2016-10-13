#include "letters.hpp"
#include <QDebug>


namespace {

const unsigned char MaxNumColumns = 5;

unsigned char Letters_0_9[] = {
	0b0110, 0b010, 0b0110, 0b0110, 0b0010, 0b1111, 0b0110, 0b1111, 0b0110, 0b0110,
	0b1001, 0b110, 0b1001, 0b1001, 0b0100, 0b1000, 0b1000, 0b0001, 0b1001, 0b1001,
	0b1001, 0b010, 0b0010, 0b0010, 0b1111, 0b1110, 0b1110, 0b0010, 0b0110, 0b0111,
	0b1001, 0b010, 0b0100, 0b1001, 0b0010, 0b0001, 0b1001, 0b0100, 0b1001, 0b0001,
	0b0110, 0b111, 0b1111, 0b0110, 0b0010, 0b0110, 0b0110, 0b1000, 0b0110, 0b0110
};
unsigned char Letters_A_M[] = {
	0b0110, 0b1110, 0b0110, 0b1110, 0b1111, 0b1111, 0b0110, 0b10010, 0b111, 0b1111, 0b1001, 0b1000, 0b10001,
	0b1001, 0b1001, 0b1001, 0b1001, 0b1000, 0b1000, 0b1000, 0b10010, 0b010, 0b0001, 0b1010, 0b1000, 0b11011,
	0b1001, 0b1110, 0b1000, 0b1001, 0b1110, 0b1110, 0b1011, 0b11110, 0b010, 0b0001, 0b1100, 0b1000, 0b10101,
	0b1111, 0b1001, 0b1001, 0b1001, 0b1000, 0b1000, 0b1001, 0b10010, 0b010, 0b1001, 0b1010, 0b1000, 0b10001,
	0b1001, 0b1110, 0b0110, 0b1110, 0b1111, 0b1000, 0b0110, 0b10010, 0b111, 0b0110, 0b1001, 0b1111, 0b10001
};
unsigned char Letters_N_Z[] = {
	0b1001, 0b0110, 0b1110, 0b0110, 0b1110, 0b0111, 0b11111, 0b1001, 0b10001, 0b10001, 0b1001, 0b10001, 0b1111,
	0b1101, 0b1001, 0b1001, 0b1001, 0b1001, 0b1000, 0b00100, 0b1001, 0b10001, 0b10001, 0b1001, 0b01010, 0b0001,
	0b1011, 0b1001, 0b1001, 0b1001, 0b1001, 0b0110, 0b00100, 0b1001, 0b01010, 0b10101, 0b0110, 0b00100, 0b0010,
	0b1001, 0b1001, 0b1110, 0b0110, 0b1110, 0b0001, 0b00100, 0b1001, 0b01010, 0b11011, 0b1001, 0b00100, 0b0100,
	0b1001, 0b0110, 0b1000, 0b0001, 0b1001, 0b1110, 0b00100, 0b0111, 0b00100, 0b10001, 0b1001, 0b00100, 0b1111
};
unsigned char Letters_a_m[] = {
	0b0000, 0b000, 0b000, 0b000, 0b000, 0b00, 0b000, 0b000, 0b1, 0b01, 0b000, 0b0, 0b00000,
	0b0000, 0b100, 0b000, 0b001, 0b010, 0b01, 0b010, 0b100, 0b0, 0b00, 0b100, 0b1, 0b00000,
	0b0110, 0b110, 0b011, 0b011, 0b101, 0b10, 0b101, 0b100, 0b1, 0b01, 0b101, 0b1, 0b11110,
	0b1010, 0b101, 0b100, 0b101, 0b110, 0b11, 0b011, 0b111, 0b1, 0b01, 0b110, 0b1, 0b10101,
	0b0111, 0b110, 0b011, 0b011, 0b011, 0b10, 0b110, 0b101, 0b1, 0b10, 0b101, 0b1, 0b10101
};
unsigned char Letters_n_z[] = {
	0b000, 0b000, 0b000, 0b000, 0b000, 0b0110, 0b000, 0b000, 0b00000, 0b00000, 0b000, 0b000, 0b0000,
	0b000, 0b000, 0b110, 0b010, 0b000, 0b1000, 0b010, 0b000, 0b00000, 0b00000, 0b000, 0b000, 0b1111,
	0b110, 0b010, 0b101, 0b101, 0b111, 0b0110, 0b111, 0b101, 0b10001, 0b10101, 0b101, 0b101, 0b0010,
	0b101, 0b101, 0b110, 0b011, 0b100, 0b0001, 0b010, 0b101, 0b01010, 0b10101, 0b010, 0b011, 0b0100,
	0b101, 0b010, 0b100, 0b001, 0b100, 0b0110, 0b001, 0b011, 0b00100, 0b01010, 0b101, 0b001, 0b1111
};
unsigned char LettersOther[] ={
	0b11111, 0b0000, 0b0, 0b00, 0b01, 0b10, 0b1, 0b0110, 0b00, 0b0,
	0b10001, 0b0000, 0b0, 0b00, 0b10, 0b01, 0b1, 0b1001, 0b11, 0b1,
	0b10001, 0b1111, 0b0, 0b00, 0b10, 0b01, 0b1, 0b0010, 0b10, 0b0,
	0b10001, 0b0000, 0b0, 0b01, 0b10, 0b01, 0b0, 0b0000, 0b11, 0b1,
	0b11111, 0b0000, 0b1, 0b10, 0b01, 0b10, 0b1, 0b0010, 0b00, 0b0
};
enum LettersOtherCharacters {
	Unknown,
	Hyphen,
	Dot,
	Comma,
	OpeningBracket,
	ClosingBracket,
	ExclamationMark,
	QuestionMark,
	Copyright_C,
	Colon,
	LettersOtherCharacters_LastPlusOne
};

bool inRange(QChar low, QChar up, QChar c) {
	return ( c >= low && c<= up );
}

unsigned char otherChar2enum(char c) {
	unsigned char val;
	switch( c ) {
	case '-':
		val=Hyphen;
	break;
	case '.':
		val=Dot;
	break;
	case ',':
		val=Comma;
	break;
	case '(':
		val=OpeningBracket;
	break;
	case ')':
		val=ClosingBracket;
	break;
	case '!':
		val=ExclamationMark;
	break;
	case '?':
		val=QuestionMark;
	break;
	case '$':
		val=Copyright_C;
	break;
	case ':':
		val=Colon;
	break;
	default:
		val=Unknown;
	break;
	}
	return val;
}

Letters::Letter getPosFromSet( unsigned char * set, char pos, char beg, char end ) {
	Letters::Letter l;
	for( int i=0; i<Letters::NumLines; ++i ) {
		l.lines[i] = set[pos + i*(end-beg+1)];
	}
	return l;
}

Letters::Letter getLetterFromSet( unsigned char * set, char letter, char beg, char end ) {
	unsigned char pos = letter-beg;
	return getPosFromSet( set, pos, beg, end);
}


} //namespace


Letters::Letter Letters::getLetter(char c)
{
	Letter l;
	if( inRange( '0', '9', c ) ) {
		l = getLetterFromSet( Letters_0_9, c, '0', '9' );
	} else if( inRange( 'A', 'M', c ) ) {
		l = getLetterFromSet( Letters_A_M, c, 'A', 'M' );
	} else if( inRange( 'N', 'Z', c ) ) {
		l = getLetterFromSet( Letters_N_Z, c, 'N', 'Z' );
	} else if( inRange( 'a', 'm', c ) ) {
		l = getLetterFromSet( Letters_a_m, c, 'a', 'm' );
	} else if( inRange( 'n', 'z', c ) ) {
		l = getLetterFromSet( Letters_n_z, c, 'n', 'z' );
	} else {
		l = getPosFromSet( LettersOther, otherChar2enum(c), 0, LettersOtherCharacters_LastPlusOne-1 );
	}
	return l;
}

unsigned char Letters::Letter::numColumns()
{
	for( char nc = MaxNumColumns; nc > 0; --nc ) {
		unsigned char mask = 1 << (nc - 1);
		for( unsigned char i=0; i<NumLines; ++i ) {
			if( lines[i] & mask ) {
				return nc;
			}
		}
	}
	return 0;
}
