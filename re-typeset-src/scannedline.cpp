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

#include "scannedline.hpp"

ScannedLine::ScannedLine() {
	isImage_=false;
	isNewParagraph_=false;
	toDelete_=false;
	toDivide_=false;
	hasDividedWordAtEnd_=false;
	spaceToNextLine_=-1;
}

void ScannedLine::setNextLineTop(int value) {
	spaceToNextLine_ = value-(top()+height());
}

bool ScannedLine::trim(const QImage & image) {
	QRgb black=QColor( 0, 0 ,0 ).rgb();
	int x=0, y=0;
	for( x=0; x<this->width(); ++x ) {
		for( y=0; y<this->height(); ++y ) {
			if( image.pixel( this->left()+x, this->top()+y ) == black ) {
				this->setLeft( x+this->left() );
				x=this->width();
				break;
			}//from left
		}
	}
	if( x==this->width() && y==this->height() ) {
		return false;//empty image, y==height-1 if there is one pixel at bottom right
	}

	for( x=this->width()-1; x>=0; --x ) {
		for( y=0; y<this->height(); ++y ) {
			if( image.pixel( this->left()+x, this->top()+y )==black ) {
				this->setRight( this->left()+x );
				x=-1;
				break;
			}//from right
		}
	}

	for( y=0; y<this->height(); ++y ) {
		for( x=0; x<this->width(); ++x ) {
			if( image.pixel( this->left()+x, this->top()+y )==black ) {
				this->setTop( this->top()+y );
				y=this->height();
				break;
			}//from top
		}
	}
	for( y=this->height()-1; y>=0; --y ) {
		for( x=0; x<this->width(); ++x ) {
			if( image.pixel( this->left()+x, this->top()+y )==black ) {
				this->setBottom( this->top()+y );
				y=-1;
				break;
			}//from bottom
		}
	}
	return true;
}

int ScannedLine::blackPixelsInRow(const QImage & image, int y) {
	int c=0;
	QRgb black=QColor( 0, 0 ,0 ).rgb();
	for( int i=this->left(), maxI=this->width()+this->left(); i<maxI; ++i ) {
		if( image.pixel( i, y )==black ) {
			++c;
		}
	}
	return c;
}

QVector<ScannedLine> ScannedLine::collapseLine(const QImage & image, StatsPack stats ) {
	QVector< ScannedLine > out;
	if( ! this->trim( image ) ) {
		return out;
	}

	if( this->height() < Cconsts::MinHeightForFigureInMedian*stats.height_ ) {//line of normal height
		out.push_back( *this );
	} else if( this->height() < Cconsts::AccidentiallyConnectedLines::MaxHeight( stats ) ) {
		if( this->cutAccidentiallyConnectedLines( image, stats, out ) ) {//divided by 3
			return out;
		} else {//can be cutted vertically
			if( this->height() > Cconsts::MinHeightForOneVerticalDiv( stats ) ) {//no protection of math formulas
				this->preciseDivideVertical( image, stats, out, 2 );//div vert always run div hori
			} else {
				out.push_back( *this );//height < 2, don't cut
			}
		}
	} else {//big image or multicolumn
		this->preciseDivideVertical( image, stats, out, 6 );
	}
	return out;
}

void ScannedLine::preciseDivideVertical(const QImage & image, StatsPack stats, QVector<ScannedLine> & out, int level) {
	QVector < ScannedLine > columns;
	ScannedLine currentColumn;
	currentColumn.setTop( this->top() );
	currentColumn.setBottom( this->bottom() );

	QRgb black=QColor( 0, 0 ,0 ).rgb();
	bool columnStarted=false;

	int x, maxX, y, maxY;
	int w=0;
	for( x=this->left(), maxX=this->left()+this->width(); x<maxX; ++x ) {//for column
		int c=0;
		for( y=this->top(), maxY=this->top()+this->height(); y<maxY; ++y ) {
			if( image.pixel(x, y) == black ) {//one black pixel == column
				++c;
				break;
			}
		}
		if( y == maxY ) {
			++w;
		}
		if( c > 0 ) {
			w=0;
			if( ! columnStarted ) {
				columnStarted=true;
				currentColumn.setLeft( x );
				currentColumn.blockLeftPos_=x;
			}
		} else {
			if( columnStarted && w > Cconsts::MinHorizontalSpaceInMedian*stats.height_ ) {//only whites - end of line
				currentColumn.setRight( x-1 );
				columnStarted=false;
				w=0;
				columns.push_back( currentColumn );
			}
		}
	}//for column -- end
	if( columnStarted ) {
		currentColumn.setRight( this->right() );
		columnStarted=false;
		columns.push_back( currentColumn );
	}

	if( columns.size() > 1 ) {
		QVector< QVector< ScannedLine > > tmpOut;
		QVector< ScannedLine > emptyOut;
		for( int i=0; i<columns.size(); ++i ) {
			tmpOut.push_back( emptyOut );
			columns[i].preciseDivideHorizontal( image, stats, tmpOut[i], level-1 );
		}
		for( int i=1; i<tmpOut.size(); ++i ) {//INFO can be problem with more levels
			if( tmpOut[i-1].size() == tmpOut[i].size() && tmpOut[i].size() > 1 ) {//join columns with the same number of lines
				for( int j=0; j<tmpOut[i].size(); ++j ) {
					tmpOut[i-1][j].setRight( tmpOut[i][j].right() );
				}
				tmpOut.remove( i );
				--i;
			}
		}
		for( int i=0; i<tmpOut.size(); ++i ) {
			for( int j=0; j<tmpOut[i].size(); ++j ) {
				out.push_back( tmpOut[i][j] );
			}
		}
	} else {
		columns.first().preciseDivideHorizontal( image, stats, out, level-1 );
	}
}

void ScannedLine::preciseDivideHorizontal(const QImage & image, StatsPack stats, QVector<ScannedLine> & out, int level) {
	QVector < ScannedLine > lines;

	ScannedLine currentLine;
	currentLine.setLeft( this->left() );
	currentLine.setRight( this->right() );
	currentLine.blockLeftPos_=this->blockLeftPos_;

	QRgb black=QColor( 0, 0 ,0 ).rgb();
	bool lineStarted=false;

	for( int y=this->top(), maxY=this->height()+this->top(); y<maxY; ++y ) {//for linie
		int c=0;
		for( int x=this->left(), maxX=this->width()+this->left(); x<maxX; ++x ) {
			if( image.pixel(x, y) == black ) {
				++c;
				break;
			}
		}
		if( c > 0 ) {
			if( ! lineStarted ) {
				lineStarted=true;
				currentLine.setTop( y );
				if( ! lines.empty() ) {
					lines.last().setNextLineTop( y );
				}
			}
		} else {
			if( lineStarted ) {
				currentLine.setBottom( y-1 );
				lineStarted=false;
				lines.push_back( currentLine );
			}
		}
	}//for linie -- end
	if( lineStarted ) {
		currentLine.setBottom( this->bottom() );
		lineStarted=false;
		lines.push_back( currentLine );
	}


	for( int i=0; i<lines.size(); ++i ) {
		if( lines[i].height() < Cconsts::MinHeightForFigureInMedian*stats.height_ ) {//normal height
			if( lines[i].trim( image ) ) {
				out.push_back( lines[i] );
			}
		} else if( lines[i].height() < Cconsts::AccidentiallyConnectedLines::MaxHeight( stats ) ) {//line 3x
			if( ! lines[i].cutAccidentiallyConnectedLines( image, stats, out ) ) {//can't divide for 3, maybe one horizontally if there was no try before
				if( level > 0 ) {//NOTE maybe additional condition for dividing
					lines[i].preciseDivideVertical( image, stats, out, 1 );
				} else {
					lines[i].isImage_=true;
					lines[i].trim( image );
					out.push_back( lines[i] );
				}
			}
		} else {//image
			if( level > 0 ) {
				lines[i].preciseDivideVertical( image, stats, out, level-1 );
			} else {
				lines[i].isImage_=true;
				lines[i].trim( image );
				out.push_back( lines[i] );
			}
		}
	}
}

void ScannedLine::checkIfHasDividedWordAtEnd(const QImage & image ) {
	QRgb black=QColor( 0, 0 ,0 ).rgb();
	int dirt=0;

	int x, endX;
	for( x=this->left()+this->width()-1, endX=this->left()+this->width()-this->height()*Cconsts::DividedWord::MaxLength; x>endX; --x ) {
		int c=0;
		for( int y=this->top(), endY=this->top()+this->height(); y<endY; ++y ) {
			if( y < this->top()+this->height()*Cconsts::DividedWord::UpperGap ) {
				if( image.pixel( x, y ) == black ) {
					if( dirt == 0 ) {
						++dirt;
					} else {//too many dots
						endX=x+1;
						y=endY;
					}
				}
			} else if( y <= this->top()+this->height()*(1-Cconsts::DividedWord::LowerGap) ) {
				if( image.pixel( x, y ) == black ) {
					++c;
				}
			} else {
				if( image.pixel( x, y ) == black ) {
					if( dirt == 0 ) {
						++dirt;
					} else {//too many dots
						endX=x+1;
						y=endY;
					}
				}
			}
		}
		if( c < 1 || c > this->height()*Cconsts::DividedWord::MaxHeight ) {
			endX=x+1;
		}
	}

	int dx=this->left()+this->width()-1-x;
	if( dx >= this->height()*Cconsts::DividedWord::MinLength && dx <= this->height()*Cconsts::DividedWord::MaxLength ) {
		this->hasDividedWordAtEnd_=true;
		this->setRight( x+1 );

		for( x=this->width()-1; x>=0; --x ) {//cut from right after removing ``-''
			for( int y=0; y<this->height(); ++y ) {
				if( image.pixel( this->left()+x, this->top()+y )==black ) {
					this->setRight( this->left()+x );
					x=-1;
					break;
				}
			}
		}//copy form CscannedLine::trim()

	}
}

void ScannedLine::getWords(const QImage & imageMono, const QImage & imageColor, StatsPack stats, PrintedLine & par,
							double scalingRatio, int maxWordLength, bool fullColor) {


	QRgb black=QColor( 0, 0 ,0 ).rgb();
	bool wordStarted=false;
	bool wordTooLong=false;

	int begX=0;
	int spaceX=Cconsts::MinHorizontalSpaceInMedian*stats.height_;
	int x, maxX, y, maxY;
	int w=0;
	for( x=this->left(), maxX=this->left()+this->width(); x<maxX; ++x ) {//for column
		int c=0;
		for( y=this->top(), maxY=this->top()+this->height(); y<maxY; ++y ) {
			if( imageMono.pixel(x, y) == black ) {
				++c;
				break;
			}
		}
		if( y == maxY ) {
			++w;
		}
		if( c > 0 ) {
			w=0;
			if( wordStarted ) {
				if( x-begX > maxWordLength && ( ! this->isImage_ ) ) {
					wordTooLong=true;
				}
			} else {
				wordStarted=true;
				begX=x;
			}
		}
		if( c == 0 || wordTooLong ) {
			if( (wordStarted && w > spaceX) || wordTooLong ) {
				wordStarted=false;
				w=0;
				if( this->isImage_ ) {
					par.push_back( (imageColor.copy( begX, this->top(), x-begX-spaceX, this->height() )) );
				} else if( fullColor ) {
					par.push_back( (imageColor.copy( begX, this->top(), x-begX-spaceX, this->height() )).scaledToHeight(
									   this->height()*scalingRatio, Qt::SmoothTransformation ) );
				} else {
					par.push_back( (imageMono.copy( begX, this->top(), x-begX-spaceX, this->height() )).scaledToHeight(
									   this->height()*scalingRatio, Qt::SmoothTransformation ) );
				}
				if( wordTooLong ) {
					QRgb white=QColor( 255, 255 ,255 ).rgb();
					for( int xx=par.last().width()-1; xx>max( 0, par.last().width()-spaceX ); --xx ) {
						for( int yy=0; yy<par.last().height(); ++yy ) {
							if( yy==par.last().height()/2 || yy==par.last().height()/2-1 ) {
								par.last().setPixel( xx, yy, black );
							} else {
								par.last().setPixel( xx, yy, white );
							}
						}
					}
					wordTooLong=false;
					x=max( this->left(), x-3*spaceX );
				}
			}
		}
	}//for column -- end
	if( wordStarted ) {
		if( this->isImage_ ) {
			par.push_back( (imageColor.copy( begX, this->top(), this->left()+this->width()-begX, this->height() )) );
		} else if( fullColor ) {
			par.push_back( (imageColor.copy( begX, this->top(), this->left()+this->width()-begX, this->height() )).scaledToHeight(
							   this->height()*scalingRatio, Qt::SmoothTransformation ) );
		} else {
			par.push_back( (imageMono.copy( begX, this->top(), this->left()+this->width()-begX, this->height() )).scaledToHeight(
							   this->height()*scalingRatio, Qt::SmoothTransformation ) );
		}
	}
}

bool ScannedLine::cutAccidentiallyConnectedLines(const QImage & image, StatsPack stats, QVector<ScannedLine> & out) {

	if( this->height() < ( (2*Cconsts::AccidentiallyConnectedLines::MaxNumberOfLines-1)*stats.height_
						   + (2*Cconsts::AccidentiallyConnectedLines::MaxNumberOfLines-3)*stats.divToNextLine_ )/2 ) {//div by 2

		if( Cconsts::AccidentiallyConnectedLines::CanCut( stats,
														  this->blackPixelsInRow( image, this->top()+this->height()*1/4),
														  this->blackPixelsInRow( image, this->top()+this->height()*2/4),
														  this->blackPixelsInRow( image, this->top()+this->height()*3/4) ) ) {//can cut

			this->isImage_=false;
			out.push_back( *this );
			out.last().setBottom( this->top()+this->height()/2-1 );
			out.last().trim( image );

			out.push_back( *this );
			out.last().setTop( this->top()+this->height()/2+1 );
			out.last().trim( image );

			return true;
		}
	} else {//div by 3

		if( Cconsts::AccidentiallyConnectedLines::CanCut( stats,
														  this->blackPixelsInRow( image, this->top()+this->height()*1/6),
														  this->blackPixelsInRow( image, this->top()+this->height()*2/6),
														  this->blackPixelsInRow( image, this->top()+this->height()*3/6),
														  this->blackPixelsInRow( image, this->top()+this->height()*4/6),
														  this->blackPixelsInRow( image, this->top()+this->height()*5/6) ) ) {//can cut

			this->isImage_=false;
			out.push_back( *this );
			out.last().setBottom( this->top()+this->height()/3-1 );
			out.last().trim( image );

			out.push_back( *this );
			out.last().setTop( this->top()+this->height()/3+1 );
			out.last().setBottom( this->top()+this->height()*2/3-1 );
			out.last().trim( image );

			out.push_back( *this );
			out.last().setTop( this->top()+this->height()*2/3+1 );
			out.last().trim( image );

			return true;
		}
	}
	return false;
}
