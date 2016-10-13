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

#include "scannedpage.hpp"
#include <QDebug>

ScannedPage::ScannedPage() {
	;//NOOP
}

ScannedPage::ScannedPage(QString fileMono, QString fileColor): fileMono_(fileMono), fileColor_(fileColor), processedLine_( 0 ) {
	;//NOOP
}

void ScannedPage::getLinesAndHeights(Stats & stats) {
	QImage image( fileMono_ );
	getLinesAndHeights(stats, image);
}

void ScannedPage::getLinesAndHeights(Stats &stats, const QImage &image)
{
	ScannedLine currentLine;
	currentLine.setLeft( 0 );
	currentLine.blockLeftPos_=0;
	currentLine.setRight( image.width()-1 );

	QRgb black=QColor( 0, 0 ,0 ).rgb();
	bool lineStarted=false;

	for( int height=image.height(), i=0; i<height; ++i ) {//for linie
		int c=0;
		for( int width=image.width(), j=0; j<width; ++j ) {
			if( image.pixel(j, i) == black ) {
				++c;
				break;
			}
		}
		if( c > 0 ) {
			if( ! lineStarted ) {
				lineStarted=true;
				currentLine.setTop( i );
				if( ! lines_.empty() ) {
					lines_.last().setNextLineTop( i );
				}
			}
		} else {
			if( lineStarted ) {
				currentLine.setBottom( i-1 );
				lineStarted=false;
				if( ! lines_.empty() ) {
					StatsPack pack( lines_.last().height(), lines_.last().spaceToNextLine_ );
					stats.add( pack );
				}
				lines_.push_back( currentLine );
			}
		}
	}//for linie -- end
	if( lineStarted ) {
		currentLine.setBottom( image.height()-1 );
		lineStarted=false;
		lines_.push_back( currentLine );
		StatsPack pack( lines_.last().height(), lines_.last().spaceToNextLine_ );
		stats.add( pack );
	}
}

void ScannedPage::reConnectDots(StatsPack stats) {
	for( int i=1; i<lines_.size(); ++i ) {
		ScannedLine & up=lines_[i-1];
		ScannedLine & mid=lines_[i];
		//joining dots ex: `` ijiji''

		if( up.height() < Consts::MinHeightForLineInMedian*stats.height_ ) {
			if( mid.height() > Consts::MinHeightForLineInMedian*stats.height_
					&& mid.bottom()-up.top()
					< Consts::MaxHeightForJoinedLineInHerHeight*mid.height() ) {
				mid.setTop( up.top() );
			}
			--i;
			lines_.remove( i );
			if( i > 0 && i < lines_.size() ) {
				lines_[ i-1 ].setNextLineTop( lines_[ i ].top() );
			}
		}
	}
	if( ( ! lines_.empty() ) && lines_.last().height() < Consts::MinHeightForLineInMedian*stats.height_ ) {
		lines_.pop_back();
	}
}

void ScannedPage::trimDivideLines(StatsPack stats) {
	QImage image( fileMono_ );
	trimDivideLines(stats, image);
}

void ScannedPage::trimDivideLines(StatsPack stats, const QImage &image) {
	for( int i=0; i<lines_.size(); ++i ) {
		QVector< ScannedLine > newLines = lines_[i].collapseLine( image, stats );
		if( ! newLines.empty() ) {//line returns lines on which she was divided
			lines_[i]=newLines[0];
			for( int j=1; j<newLines.size(); ++j ) {
				++i;
				lines_.insert( i, newLines[j] );
			}
		} else {
			lines_.remove( i );
			--i;
		}
	}
}

void ScannedPage::connectDescriptionsToImages(StatsPack stats) {
	QImage image( fileMono_ );
	connectDescriptionsToImages(stats, image);
}

void ScannedPage::connectDescriptionsToImages(StatsPack stats, const QImage &image) {
	QRgb black=QColor( 0, 0 ,0 ).rgb();
	int spaceX=Consts::MinHorizontalSpaceInMedian*stats.height_;
	enum Stage {FirstSpace, Description, EndSpace, CorrectDescription};

	for( int i=0; i<lines_.size(); ++i ) {
		if( lines_[i].isImage_ ) {
			Stage stage=FirstSpace;
			int yEndOfDescription=0;
			int c=0;

			for( int y=lines_[i].top()+lines_[i].height(), endY=image.height(); y<endY; ++y ) {//to end

				for( int x=lines_[i].left(), endX=lines_[i].left()+spaceX; x<endX; ++x ) {//white margins of image
					if( image.pixel( x, y ) == black ) {
						++c;
					}
				}
				for( int x=lines_[i].left()+lines_[i].width()-spaceX, endX=lines_[i].left()+lines_[i].width(); x<endX; ++x ) {
					if( image.pixel( x, y ) == black ) {
						++c;
					}
				}
				if( c>2 ) {
					y=image.height();
					break;
				}

				if( stage == FirstSpace ) {
					int d=0;
					for( int x=lines_[i].left()+spaceX, endX=lines_[i].left()+lines_[i].width()-spaceX; x<endX; ++x ) {
						if( image.pixel( x, y ) == black ) {
							++d;
							if( d>2 ) {
								stage=Description;
								break;
							}
						}
					}
					if( y-lines_[i].top()-lines_[i].height() > Consts::Description::MaxHeightInMedian*stats.height_ ) {
						y=image.height();
					}
				} else if( stage == Description ) {
					int d=0;
					for( int x=lines_[i].left()+spaceX, endX=lines_[i].left()+lines_[i].width()-spaceX; x<endX; ++x ) {
						if( image.pixel( x, y ) == black ) {
							++d;
							break;
						}
					}
					if( d == 0 ) {
						yEndOfDescription=y;
						stage=EndSpace;
					}
				} else if( stage == EndSpace ) {
					int d=0;
					for( int x=lines_[i].left()+spaceX, endX=lines_[i].left()+lines_[i].width()-spaceX; x<endX; ++x ) {
						if( image.pixel( x, y ) == black ) {
							++d;
							if( d>2 ) {
								if( y-yEndOfDescription < Consts::Description::MinSpaceAfterInMedian*stats.height_ ) {
									stage=Description;
									break;
								} else {
									if( yEndOfDescription-lines_[i].top()-lines_[i].height() < Consts::Description::MaxHeightInMedian*stats.height_ ) {
										stage=CorrectDescription;
									}
									y=image.height();
									break;
								}
							}
						}
					}
					if( y-yEndOfDescription>= Consts::Description::MinSpaceAfterInMedian*stats.height_
							|| y == image.height()-1 ) {
						if( yEndOfDescription-lines_[i].top()-lines_[i].height() < Consts::Description::MaxHeightInMedian*stats.height_ ) {
							stage=CorrectDescription;
						}
						y=image.height();
						break;
					}
				}
			}
			if( stage == CorrectDescription ) {
				lines_[i].setBottom( yEndOfDescription-1 );
				for( int j=0; j<lines_.size(); ++j ) {
					if( i != j && lines_[i].intersects( lines_[j] ) ) {
						if( lines_[i].left() <= lines_[j].left() && lines_[i].right() >= lines_[j].right() ) {
							lines_[j].toDelete_=true;
						} else if( lines_[i].left()-lines_[j].left() > lines_[j].right()-lines_[i].right() ) {
							lines_[j].setRight( lines_[i].left()-1 -1 );
							lines_[j].toDivide_=true;
						} else {
							lines_[j].setLeft( lines_[i].right()+1 +1 );
							lines_[j].toDivide_=true;
							lines_[j].blockLeftPos_=lines_[i].right()+1 +1;
						}
					}
				}
			}
		}//if isImage
	}
	for( int i=0; i<lines_.size(); ++i ) {
		if( lines_[i].toDivide_ ) {
			QVector< ScannedLine > newLines = lines_[i].collapseLine( image, stats );
			if( ! newLines.empty() ) {
				lines_[i]=newLines[0];
				for( int j=1; j<newLines.size(); ++j ) {
					++i;
					lines_.insert( i, newLines[j] );
				}
			} else {
				lines_.remove( i );
				--i;
			}
		} else if( lines_[i].toDelete_ ) {
			lines_.remove( i );
			--i;
		}
	}
}

void ScannedPage::findParagraphs(StatsPack stats) {
	if( lines_.isEmpty() ) {
		return;
	}
	QVector< int > blockInfo;
	int begI=0;
	int endI=0;
	int blockStart=0;
	blockStart=lines_.first().blockLeftPos_;
	blockInfo.push_back( lines_.first().left() );

	for( int i=1; i<lines_.size(); ++i ) {

		if( lines_[i].blockLeftPos_ == blockStart ) {
			blockInfo.push_back( lines_[i].left() );
		} else {//new block, recalculate old
			endI=i;
			int div;
			if( blockInfo.size() < 4 ) {
				div=0;
			} else if( blockInfo.size() < 7 ) {
				div=1;
			} else {
				div=(blockInfo.size())/2 - 2;
			}
			std::nth_element( blockInfo.begin(), blockInfo.begin()+div, blockInfo.end() );
			int med=blockInfo[ div ];
			blockInfo.clear();
			for( int j=begI; j<endI; ++j ) {
				if( lines_[j].left() > med + stats.height_*Consts::MinParagraphIndentInMedian
						&& lines_[j].left() < med + stats.height_*Consts::MaxParagraphIndentInMedian ) {
					lines_[j].isNewParagraph_=true;
				}
			}
			blockStart=lines_[i].blockLeftPos_;
			blockInfo.push_back( lines_[i].left() );
			begI=i;
		}
	}
	int div;
	if( blockInfo.size() < 4 ) {
		div=0;
	} else if( blockInfo.size() < 7 ) {
		div=1;
	} else {
		div=(blockInfo.size())/2 - 2;
	}
	std::nth_element( blockInfo.begin(), blockInfo.begin()+div, blockInfo.end() );
	int med=blockInfo[ div ];
	for( int j=begI; j<lines_.size(); ++j ) {
		if( lines_[j].left() > med + stats.height_*Consts::MinParagraphIndentInMedian
				&& lines_[j].left() < med + stats.height_*Consts::MaxParagraphIndentInMedian ) {
			lines_[j].isNewParagraph_=true;
		}
	}
}

void ScannedPage::findDividedWords() {
	QImage image( fileMono_ );
	findDividedWords(image);

}

void ScannedPage::findBaselines(){
	QImage image(fileMono_);
	findBaselines( image );
}

void ScannedPage::findBaselines(const QImage &image){

	for( int i=0; i<lines_.size(); ++i ) {
		if( ! lines_[i].isImage_ ) {
			lines_[i].findBaselines(image);
		}
	}
}

void ScannedPage::findDividedWords(const QImage &image) {
	for( int i=0; i<lines_.size(); ++i ) {
		if( ! lines_[i].isImage_ ) {
			lines_[i].checkIfHasDividedWordAtEnd( image );
		}
	}
}

void ScannedPage::checkNumberHeader(StatsPack stats, StatsNumberHeader & numHead) {
	if( lines_.isEmpty() ) {
		return;
	}
	if( lines_.first().spaceToNextLine_ >= Consts::NumberHeader::MinSpaceToTextInSpace*stats.divToNextLine_
			&& lines_.first().width() >= Consts::NumberHeader::MaxLengthOfNumberInMedian*stats.height_ ) {
		++numHead.header_;
	}
	if( lines_.first().width() < Consts::NumberHeader::MaxLengthOfNumberInMedian*stats.height_ ) {
		++numHead.numberTop_;
	}
	if( lines_.last().width() < Consts::NumberHeader::MaxLengthOfNumberInMedian*stats.height_ ) {
		++numHead.numberBottom_;
	}
}

void ScannedPage::setNumberHeader(StatsPack stats, ScannedPage::PageNumber numberPosition, bool header) {
	if( lines_.isEmpty() ) {
		return;
	}
	if( header && lines_.first().spaceToNextLine_ >= Consts::NumberHeader::MinSpaceToTextInSpace*stats.divToNextLine_
			&& lines_.first().width() >= Consts::NumberHeader::MaxLengthOfNumberInMedian*stats.height_ ) {
		header_=lines_.first();
		lines_.pop_front();
	}
	if( numberPosition == Top && lines_.first().width() < Consts::NumberHeader::MaxLengthOfNumberInMedian*stats.height_ ) {
		pageNumber_=lines_.first();
		lines_.pop_front();
	}
	if( numberPosition == Bottom && lines_.last().width() < Consts::NumberHeader::MaxLengthOfNumberInMedian*stats.height_ ) {
		pageNumber_=lines_.last();
		lines_.pop_back();
	}
}

void ScannedPage::printDEbugImages(StatsPack stats) {
	QImage inputImage( fileMono_ );
	QImage image = printDEbugImages(stats, inputImage);
	image.save( fileMono_ + ".DE.png" , "PNG" );
}

QImage ScannedPage::printDEbugImages(StatsPack stats, const QImage &inputImage) {
	QImage image=inputImage.convertToFormat( QImage::Format_RGB32 );
	image.setDotsPerMeterX( Consts::Print::DotsPerMeter( stats.height_ ) );
	image.setDotsPerMeterY( Consts::Print::DotsPerMeter( stats.height_ ) );
	QPainter painter( &image );

	painter.setFont( Consts::Print::FontBold() );
	for( int i=0; i<lines_.size(); ++i ) {
		painter.setPen( QColor( 0, 0, 0, 0 ) );
		painter.setBrush( QColor( 255, 255, 255, 170 ) );
		painter.drawRect( lines_[i] );
		painter.setBrush( QColor( 0, 255, 0, 70 ) );
		painter.drawRect( lines_[i] );

		QString info=QString::number( i+1 );// + " b:" + QString::number( lines_[i].blockLeftPos_ );
		if( lines_[i].isImage_ ) {
			info += " ⨂";
		}
		if( lines_[i].isNewParagraph_ ) {
			info += " ¶";
		}
		if( lines_[i].hasDividedWordAtEnd_ ) {
			info += " ÷";
		}

		painter.setPen( QColor( 0, 0, 0, 255 ) );
		painter.drawText( lines_[i], info );//, Qt::AlignLeft,
		painter.drawLine( lines_[i].left(), lines_[i].top()+lines_[i].upperBaseline_, lines_[i].right(), lines_[i].top()+lines_[i].upperBaseline_ );
		painter.drawLine( lines_[i].left(), lines_[i].top()+lines_[i].lowerBaseline_, lines_[i].right(), lines_[i].top()+lines_[i].lowerBaseline_ );

	}
	if( header_.isValid() ) {
		painter.setPen( QColor( 0, 0, 0, 0 ) );
		painter.setBrush( QColor( 255, 255, 255, 170 ) );
		painter.drawRect( header_ );
		painter.setBrush( QColor( 0, 255, 0, 70 ) );
		painter.drawRect( header_ );
		QString info="↖"; //"↖ b:" + QString::number( header_.blockLeftPos_ ) + " ↖";
		painter.setPen( QColor( 0, 0, 0, 255 ) );
		painter.drawText( header_, info );//, Qt::AlignLeft
	}
	if( pageNumber_.isValid() ) {
		painter.setPen( QColor( 0, 0, 0, 0 ) );
		painter.setBrush( QColor( 255, 255, 255, 170 ) );
		painter.drawRect( pageNumber_ );
		painter.setBrush( QColor( 0, 255, 0, 70 ) );
		painter.drawRect( pageNumber_ );
		QString info="#"; //"# b:" + QString::number( pageNumber_.blockLeftPos_ ) + " #";
		painter.setPen( QColor( 0, 0, 0, 255 ) );
		painter.drawText( pageNumber_.adjusted( 0, 0, stats.height_*3, stats.height_*3 ), info );//, Qt::AlignLeft,
	}
	painter.setPen( QColor( 0, 0, 0, 0 ) );
	painter.setBrush( QColor( 255, 255, 0, 255 ) );
	painter.drawRect( image.width()/2, image.height()/2, stats.height_*4, stats.height_*3 );
	painter.setPen( QColor( 255, 0, 0, 255 ) );
	painter.drawText( image.width()/2, image.height()/2, stats.height_*4, stats.height_*3, Qt::AlignCenter | Qt::AlignVCenter, "Σ: " + QString::number( lines_.size() ) );
	return image;
}

void ScannedPage::getNumHead(StatsPack stats, PrintedLine & numHead, double scalingRatio, int maxWordLength, bool fullColor) {
	numHead.clear();
	QImage imageMono( fileMono_ );
	QImage imageColor( fileColor_ );
	if( pageNumber_.isValid() ) {
		pageNumber_.getWords( imageMono, imageColor, stats, numHead, scalingRatio, maxWordLength, fullColor );
	}
	if( header_.isValid() ) {
		header_.getWords( imageMono, imageColor, stats, numHead, scalingRatio, maxWordLength, fullColor );
	}
}

bool ScannedPage::getParagraph(StatsPack stats, PrintedLine & paragraph, double scalingRatio, int maxWordLength, bool fullColor) {
	if( lines_.isEmpty() ) {
		return false;
	}
	QImage imageMono( fileMono_ );
	QImage imageColor( fileColor_ );
	lines_[ processedLine_ ].getWords( imageMono, imageColor, stats, paragraph, scalingRatio, maxWordLength, fullColor );

	paragraph.isNewParagraph_=lines_[ processedLine_ ].isNewParagraph_;

	if( processedLine_>0 && ( ! lines_[ processedLine_-1 ].isImage_ )
			&& lines_[ processedLine_-1 ].spaceToNextLine_>=Consts::MinHeightForEmptyLineInSpaceToNext*stats.divToNextLine_ ) {
		paragraph.isEmptyLineBefore_=true;
	}

	++processedLine_;//get next in next step

	if( lines_[ processedLine_-1 ].isImage_ || lines_[ processedLine_-1 ].hasDividedWordAtEnd_ ) {
		paragraph.isImage_=lines_[ processedLine_-1 ].isImage_;
		paragraph.hasDividedWordAtEnd_=lines_[ processedLine_-1 ].hasDividedWordAtEnd_;
		if( processedLine_<lines_.size() ) {//no next
			return true;
		} else {
			return false;
		}
	}


	while( processedLine_<lines_.size() && ! lines_[ processedLine_ ].isNewParagraph_
		   && ! lines_[ processedLine_ ].isImage_ && lines_[ processedLine_-1 ].hasDividedWordAtEnd_
		   && lines_[ processedLine_-1 ].spaceToNextLine_<Consts::MinHeightForEmptyLineInSpaceToNext*stats.divToNextLine_ ) {
		lines_[ processedLine_ ].getWords( imageMono, imageColor, stats, paragraph, scalingRatio, fullColor );
		++processedLine_;
	}
	paragraph.hasDividedWordAtEnd_=lines_[ processedLine_-1 ].hasDividedWordAtEnd_;
	if( processedLine_<lines_.size() ) {
		return true;
	}
	return false;
}

void ScannedPage::removeMonoImage() {
	QFile::remove( fileMono_ );
}
