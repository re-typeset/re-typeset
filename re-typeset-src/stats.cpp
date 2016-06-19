// Re-Typeset program for converting scanned documents to smaller size of page
// without changing text size.
//
// Copyright (C) 2014/2015 Piotr Mika (piotr.mt.mika@gmail.com).
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

#include "stats.hpp"
#include <algorithm>//from STL

Stats::Stats() {
	;//NOOP
}

void Stats::add(StatsPack x) {
	heights_.push_back( x.height_ );
	divsToNextLine_.push_back( x.divToNextLine_ );
}

void Stats::calculateParams() {
	if( heights_.size() != 0 && divsToNextLine_.size() != 0 ) {
		//getting median as n-th element
		std::nth_element( heights_.begin(), heights_.begin()+heights_.size()/2, heights_.end() );
		stats_.height_=heights_[ heights_.size()/2 ];

		std::nth_element( divsToNextLine_.begin(), divsToNextLine_.begin()+divsToNextLine_.size()/2, divsToNextLine_.end() );
		stats_.divToNextLine_=divsToNextLine_[ divsToNextLine_.size()/2 ];
	} else {
		stats_.height_=-1;
		stats_.divToNextLine_=-1;
	}
}

StatsPack Stats::getStats() {
	return stats_;
}

void Stats::clear() {
	heights_.clear();
	divsToNextLine_.clear();
}

void Stats::setComicMode() {
	stats_.height_ /= ComicModeDivider;
	stats_.divToNextLine_ /= ComicModeDivider;
}

StatsPack::StatsPack() {
	;//NOOP
}

StatsPack::StatsPack(int height, int divToNextLine): height_(height), divToNextLine_(divToNextLine) {
	;//NOOP
}


StatsNumberHeader::StatsNumberHeader(): header_( 0 ), numberTop_( 0 ), numberBottom_( 0 ) {
	;//NOOP
}
