#include <gmock/gmock.h>
#include <climits>

#include "../re-typeset-src/pixelfont/letters.hpp"

TEST(LettersTest, GetLetter_G)
{
	Letters::Letter l = Letters::getLetter( 'G' );

	EXPECT_EQ( Letters::NumLines, 5 );
	EXPECT_EQ( l.lines[0], 0b0110 );
	EXPECT_EQ( l.lines[1], 0b1000 );
	EXPECT_EQ( l.lines[2], 0b1011 );
	EXPECT_EQ( l.lines[3], 0b1001 );
	EXPECT_EQ( l.lines[4], 0b0110 );
}

TEST(LettersTest, GetLetter_1)
{
	Letters::Letter l = Letters::getLetter( '1' );

	EXPECT_EQ( Letters::NumLines, 5 );
	EXPECT_EQ( l.lines[0], 0b010 );
	EXPECT_EQ( l.lines[1], 0b110 );
	EXPECT_EQ( l.lines[2], 0b010 );
	EXPECT_EQ( l.lines[3], 0b010 );
	EXPECT_EQ( l.lines[4], 0b111 );
}

TEST(LettersTest, GetLetter_P)
{
	Letters::Letter l = Letters::getLetter( 'P' );

	EXPECT_EQ( Letters::NumLines, 5 );
	EXPECT_EQ( l.lines[0], 0b1110 );
	EXPECT_EQ( l.lines[1], 0b1001 );
	EXPECT_EQ( l.lines[2], 0b1001 );
	EXPECT_EQ( l.lines[3], 0b1110 );
	EXPECT_EQ( l.lines[4], 0b1000 );
}
TEST(LettersTest, GetLetter_f)
{
	Letters::Letter l = Letters::getLetter( 'f' );

	EXPECT_EQ( Letters::NumLines, 5 );
	EXPECT_EQ( l.lines[0], 0b00 );
	EXPECT_EQ( l.lines[1], 0b01 );
	EXPECT_EQ( l.lines[2], 0b10 );
	EXPECT_EQ( l.lines[3], 0b11 );
	EXPECT_EQ( l.lines[4], 0b10 );
}
TEST(LettersTest, GetLetter_x)
{
	Letters::Letter l = Letters::getLetter( 'x' );

	EXPECT_EQ( Letters::NumLines, 5 );
	EXPECT_EQ( l.lines[0], 0b000 );
	EXPECT_EQ( l.lines[1], 0b000 );
	EXPECT_EQ( l.lines[2], 0b101 );
	EXPECT_EQ( l.lines[3], 0b010 );
	EXPECT_EQ( l.lines[4], 0b101 );
}

TEST(LettersTest, GetLetter_questionMark)
{
	Letters::Letter l = Letters::getLetter( '?' );

	EXPECT_EQ( Letters::NumLines, 5 );
	EXPECT_EQ( l.lines[0], 0b0110 );
	EXPECT_EQ( l.lines[1], 0b1001 );
	EXPECT_EQ( l.lines[2], 0b0010 );
	EXPECT_EQ( l.lines[3], 0b0000 );
	EXPECT_EQ( l.lines[4], 0b0010 );
}

TEST(LettersTest, GetLetter_OutOfRange)
{
	Letters::Letter l = Letters::getLetter( 1 );

	EXPECT_EQ( Letters::NumLines, 5 );
	EXPECT_EQ( l.lines[0], 0b11111 );
	EXPECT_EQ( l.lines[1], 0b10001 );
	EXPECT_EQ( l.lines[2], 0b10001 );
	EXPECT_EQ( l.lines[3], 0b10001 );
	EXPECT_EQ( l.lines[4], 0b11111 );
}

void clearLetter( Letters::Letter & l ) {
	for( int i=0; i<Letters::NumLines; ++i ){
		l.lines[i]=0;
	}
}

bool letterIsValid( Letters::Letter  l ) {
	for( int i=0; i<Letters::NumLines; ++i ){
		if( l.lines[i] != 0 ) {
			return true;
		}
	}
	return false;
}

TEST(LettersTest, GetLetter_WholeRange)
{
	Letters::Letter l;
	clearLetter(l);
	EXPECT_FALSE( letterIsValid(l) );
    int cnt = 0;

    for( int i=CHAR_MIN; i<=CHAR_MAX; ++i ) {
        char c = i;
		clearLetter(l);
        l = Letters::getLetter( c );
        EXPECT_TRUE( letterIsValid(l) ) << "DBG: c=" << (int)c ;
        ++cnt;
	}	
    EXPECT_EQ( cnt, 256 );
	//EXPECT NO CRASH
}

TEST(LettersTest, LetterWidth)
{
    Letters::Letter l;

    l = Letters::getLetter( '.' );
    EXPECT_EQ( 1, l.numColumns() );

    l = Letters::getLetter( 'j' );
    EXPECT_EQ( 2, l.numColumns() );

    l = Letters::getLetter( '1' );
    EXPECT_EQ( 3, l.numColumns() );

    l = Letters::getLetter( 't' );
	EXPECT_EQ( 3, l.numColumns() );

    l = Letters::getLetter( 'M' );
    EXPECT_EQ( 5, l.numColumns() );
}

