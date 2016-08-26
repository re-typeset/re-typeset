#ifndef LETTERS_HPP
#define LETTERS_HPP

#include <QChar>

namespace Letters
{
	const int NumLines = 5;
    const int SpaceColumns = 5;
	struct Letter {
		unsigned char lines[NumLines];
        unsigned char numColumns();
	};

    Letter getLetter(char c);


} //letters

#endif // LETTERS_HPP
