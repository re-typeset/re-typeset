// Re-Typeset program for converting scanned documents to smaller size of page
// without changing text size.
//
// Copyright (C) 2013-2015 Piotr Mika (piotr.mt.mika@gmail.com).
//
// This  program is free software:  you can  redistribute it and/or  modify it
// under the terms of the  GNU General Public License as published by the Free
// Software Foundation,  either version 3 of the License,  or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY  WARRANTY;  without  even the implied  warranty  of MERCHANTABILITY  or
// FITNESS FOR  A PARTICULAR  PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the  GNU  General  Public  License along
// with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef STATS_HPP
#define STATS_HPP
#include <QVector>

class StatsPack {
public:
	StatsPack();
	StatsPack( int height, int divToNextLine );
	int height_;
	int divToNextLine_;
};

class Stats {
public:
	Stats();
	void add( StatsPack x );
	void calculateParams();
	StatsPack getStats();
	void clear();
	void setComicMode();

	static const int ComicModeDivider=30;
protected:
	QVector < int > heights_;
	QVector < int > divsToNextLine_;
	StatsPack stats_;

};

class StatsNumberHeader {
public:
	StatsNumberHeader();
	int header_;
	int numberTop_;
	int numberBottom_;
};

#endif // STATS_HPP
