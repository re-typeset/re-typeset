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

#include "printedpage.hpp"
#include "consts.hpp"
#include "version.hpp"
#include <QPainter>

CprintedPage::CprintedPage() {
	;//NOOP
}

CprintedPage::~CprintedPage() {
	if( image_!=Q_NULLPTR ) {
		delete image_;
	}
}

CprintedPage::CprintedPage(int width, int height, int margin, int fontHeight, bool justify, bool rotateImages,
						   bool comicMode, bool DEbugState): margin_( margin ), isDividedWord_(false), justify_(justify),
	rotateImages_(rotateImages), outLastPage_(-1), comicMode_(comicMode), DEbugState_( DEbugState ) {
	image_ = new QImage( width, height, QImage::Format_RGB32 );
	image_->fill( Qt::white );
	lineHeight_=Cconsts::LineHeightFromTextHeight( fontHeight );
	spaceToNextLine_=0;
	cursorX=margin_;
	cursorY=margin_;

}

void CprintedPage::addNumberHeader(CprintedLine & numHead, int srcPageNum) {
	disableProgressBar=false;
	srcPageNum_=srcPageNum;
	int localCursorX=margin_;
	if( numHead.size() > 0 ) {
		numHead.maxHeight_=0;
		QPainter writer( image_ );
		for( int i=0; i<numHead.size(); ++i ) {
			numHead.maxHeight_=max( numHead.maxHeight_, numHead[i].height() );
			if( localCursorX+numHead[i].width()+margin_>image_->width() ) {
				cursorY+=numHead.maxHeight_; //lineHeight_*(1+Cconsts::Print::SpaceToNextLineInMedian);
				localCursorX=margin_;
				numHead.maxHeight_=0;
			}
			writer.drawImage( localCursorX, cursorY, numHead[i] );
			localCursorX+=Cconsts::Print::SpaceInMedian*lineHeight_+numHead[i].width();
		}
		cursorY+=( 1 +Cconsts::Print::SpaceAfterNumHeadInMedian)*lineHeight_;
	}
}

bool CprintedPage::printImagesFromQueue(bool final) {//true -- create new page
	bool firstStep=true;
	QPainter writer( image_ );
	while( imageQueue_.size()>0 ) {

		if( imageQueue_.first().height() < image_->height()-cursorY-margin_ ) {
			int del=(image_->width()-imageQueue_.first().width())/2;
			writer.drawImage( del, cursorY, imageQueue_.first() );
			cursorY+=imageQueue_.first().height()+lineHeight_;
			firstStep=false;
			imageQueue_.pop_front();
			if( final && imageQueue_.size()==0 ) {
				return true;
			}
		} else if( firstStep ) {
			if( imageQueue_.first().height() > image_->height() ) {
				imageQueue_.first()=imageQueue_.first().scaledToHeight( image_->height(), Qt::SmoothTransformation );
			}
			image_->fill( Qt::white );
			disableProgressBar=true;
			int yDel=(image_->height()-imageQueue_.first().height())/2;
			int xDel=(image_->width()-imageQueue_.first().width())/2;
			writer.drawImage( xDel, yDel, imageQueue_.first() );
			imageQueue_.pop_front();
			return true;
		} else {
			return true;
		}
	}
	return false;
}


bool CprintedPage::printLastLine() {
	QPainter writer( image_ );
	if( line_.maxHeight_+margin_+cursorY > image_->height() ) {//end of page
		return true;
	}
	for( int i=0; i<line_.size(); ++i ) {
		writer.drawImage( cursorX, cursorY, line_[i] );
		cursorX+=line_[i].width()+Cconsts::Print::SpaceInMedian*lineHeight_;
	}
	return false;
}

bool CprintedPage::addParagraph(CprintedLine & paragraph) {
	if( paragraph.isEmpty() ) {
		return false;
	}
	QPainter writer( image_ );

	if( isDividedWord_ && ! paragraph.isImage_ ) {
		if( dividedWord_.width() + paragraph.first().width() < image_->width()-2*margin_ ) {
			paragraph.first()=join( dividedWord_, paragraph.first() );
		} else {//connected words too long
			paragraph.prepend( dividedWord_ );
		}
		isDividedWord_=false;
	}

	while( paragraph.size()>0 ) {

		while( paragraph.size()>0
			   && ! paragraph.isNewParagraph_
			   && ! paragraph.isEmptyLineBefore_
			   && ! paragraph.isImage_
			   && line_.sumLengths_+line_.size()*Cconsts::Print::SpaceInMedian*lineHeight_+margin_+paragraph.first().width()+cursorX < image_->width()
			   ) {

			if( paragraph.hasDividedWordAtEnd_ && paragraph.size()==1 ) {
				dividedWord_=paragraph.first();
				isDividedWord_=true;
				paragraph.hasDividedWordAtEnd_=false;
				paragraph.pop_front();
			} else {
				line_.push_back( paragraph.first() );
				line_.sumLengths_+=paragraph.first().width();
				line_.maxHeight_=max( line_.maxHeight_, paragraph.first().height() );
				paragraph.pop_front();
			}
		}
		if( paragraph.isImage_ ) {
			if( rotateImages_ && paragraph.first().width() > image_->width()
					&& paragraph.first().width() > paragraph.first().height()
					&& image_->width() < image_->height() ) {
				paragraph.first()=paragraph.first().transformed( QMatrix().rotate( -90 ) );
			}

			if( paragraph.first().width() > image_->width() ) {
				paragraph.first()=paragraph.first().scaledToWidth( image_->width(), Qt::SmoothTransformation );
			}
			if( ( ! comicMode_ ) && ( paragraph.first().height() + lineHeight_ + margin_ + cursorY <= image_->height() ) ) {
				int del=(image_->width()-paragraph.first().width())/2;
				cursorY+=lineHeight_;
				writer.drawImage( del, cursorY, paragraph.first() );
				cursorY+=paragraph.first().height()+lineHeight_;
			} else {
				imageQueue_.push_back( paragraph.first() );
			}
			paragraph.pop_front();
			paragraph.isImage_=false;
			paragraph.isEmptyLineBefore_=false;
			if( imageQueue_.size() > Cconsts::Print::MaxImagesQueueSize || comicMode_ ) {
				return true;
			}
		} else if( paragraph.isNewParagraph_ || paragraph.isEmptyLineBefore_ ) {
			if( line_.maxHeight_+margin_+cursorY > image_->height() ) {//end of page
				return true;
			}
			for( int i=0; i<line_.size(); ++i ) {
				writer.drawImage( cursorX, cursorY, line_[i] );
				cursorX+=line_[i].width()+Cconsts::Print::SpaceInMedian*lineHeight_;
			}
			if( paragraph.isNewParagraph_ ) {
				cursorY+=line_.maxHeight_+Cconsts::Print::SpaceToNextLineInMedian*lineHeight_;
				cursorX=margin_+Cconsts::Print::ParagraphIndentInMedian*lineHeight_;
			} else {
				cursorX=margin_;
				cursorY+=line_.maxHeight_+Cconsts::Print::SpaceToNextLineInMedian*lineHeight_*2+lineHeight_;
			}
			line_.clear();
			line_.maxHeight_=0;
			line_.sumLengths_=0;
			paragraph.isNewParagraph_=false;
			paragraph.isEmptyLineBefore_=false;
		} else if( paragraph.size()>0 ) {
			if( line_.maxHeight_+margin_+cursorY > image_->height() ) {//end of page
				return true;
			}
			int space;
			int comp=0;
			if( line_.size()>1 && justify_ ) {
				space=( image_->width()-margin_-line_.sumLengths_-cursorX ) / ( line_.size()-1 );
				comp=( image_->width()-margin_-line_.sumLengths_-cursorX ) - space*( line_.size()-1 );
			} else {
				space=Cconsts::Print::SpaceInMedian*lineHeight_;
			}
			for( int i=0; i<line_.size(); ++i ) {
				writer.drawImage( cursorX, cursorY, line_[i] );
				cursorX+=line_[i].width()+space;
				if( comp > 0 ) {
					++cursorX;
					--comp;
				}
			}
			cursorY+=line_.maxHeight_+Cconsts::Print::SpaceToNextLineInMedian*lineHeight_;
			cursorX=margin_;
			line_.clear();
			line_.maxHeight_=0;
			line_.sumLengths_=0;
		} else {
			return false;//get more data
		}
	}
	return false;
}


void CprintedPage::saveAndClear(QString fileName, bool hardMargins) {	

	if( hardMargins ) {
		image_->setPixel( 0, 0, QColor( 0, 0, 0 ).rgb() );
		image_->setPixel( 0, image_->height()-1, QColor( 0, 0, 0 ).rgb() );
		image_->setPixel( image_->width()-1, 0, QColor( 0, 0, 0 ).rgb() );
		image_->setPixel( image_->width()-1, image_->height()-1, QColor( 0, 0, 0 ).rgb() );
	}

	int dx=0;
	if( image_->width()-Cconsts::Print::TypicalWatermarkLength > 0 ) {
		dx = (image_->width()-Cconsts::Print::TypicalWatermarkLength)/2;
	}
	QPoint startPoint( dx, image_->height()-CprintedPixelFont::TextHeight-1 );
	startPoint=CprintedPixelFont::putText( *image_, startPoint, CprintedPixelFont::Przeskladv );
	startPoint.rx() += CprintedPixelFont::Space;
	
	const char * verPos=VERSION;	
	while( * verPos ) {
		if( (* verPos) >= '0' && (* verPos) <= '9' ) {
			startPoint=CprintedPixelFont::putText( *image_, startPoint, CprintedPixelFont::Number, (* verPos)-'0' );
		} else {
			startPoint=CprintedPixelFont::putText( *image_, startPoint, CprintedPixelFont::Dot );
		}
		startPoint.rx() += 1;
		++verPos;
	}
	startPoint.rx() -= 1;
		
	startPoint.rx() += CprintedPixelFont::Space;
	startPoint=CprintedPixelFont::putText( *image_, startPoint, CprintedPixelFont::cPiotrMika );

	image_->save( fileName, "PNG" );
	image_->fill( Qt::white );
	//cursorX=margin_;
	cursorY=margin_;

	if( srcPageNum_!=-1 ) {
		CprintedPageStat st;
		st.name_=fileName;
		st.disableProgressBar=disableProgressBar;

		if( outStat_.isEmpty() ) {
			st.numOfAll_=0;
			st.numOnPage_=0;
			outLastPage_=srcPageNum_;
		} else {
			st.numOfAll_=outStat_.last().numOfAll_ + 1;
			if( srcPageNum_ == outLastPage_ ) {
				st.numOnPage_=outStat_.last().numOnPage_ + 1;
			} else {
				st.numOnPage_=0;
				outLastPage_=srcPageNum_;
			}
		}
		outStat_.push_back( st );
	}
}

void CprintedPage::addProgressBarsForAllPages() {
	if( outStat_.isEmpty() ) {
		return;
	}
	int pageNum=outStat_.last().numOnPage_;
	int progressBarGap=margin_*Cconsts::Print::ProgressBarGap;
	int progressBarHeight=margin_*Cconsts::Print::ProgressBarHeight;
	int progressBarDX=(1-Cconsts::Print::ProgressBarDiv)*image_->width();

	for( int i=outStat_.size()-2; i>=0; --i ) {
		if( outStat_[i+1].numOnPage_ == 0 ) {
			pageNum=outStat_[i].numOnPage_;
		}
		if( ! outStat_[i].disableProgressBar ) {
			QImage img( outStat_[i].name_ );
			QPainter p( &img );

			p.setPen( QColor( 224, 224, 224 ) );
			p.setBrush( QColor( 224, 224, 224 ) );

			for( int j=0; j<pageNum+1; ++j ) {
				int left=(1-Cconsts::Print::ProgressBarDiv)*img.width() * (j)/(pageNum+1);
				int right=(1-Cconsts::Print::ProgressBarDiv)*img.width() * (j+1)/(pageNum+1)-4;
				p.drawRect( left, progressBarGap, right-left, progressBarHeight );

				if(	outStat_[i].numOnPage_ == j ) {
					p.setBrush( QColor( 255, 255, 255 ) );
				}
			}
			p.drawRect( progressBarDX, progressBarGap, Cconsts::Print::ProgressBarDiv*img.width(), progressBarHeight );

			p.setBrush( QColor( 224, 224, 224 ) );
			int allProgres=Cconsts::Print::ProgressBarDiv*img.width() * (outStat_[i].numOfAll_+1)/outStat_.size();
			p.drawRect( progressBarDX, progressBarGap, allProgres, progressBarHeight );

			img.save( outStat_[i].name_, "PNG" );
		}

	}

	if( ! outStat_.last().disableProgressBar ) {
		QImage img( outStat_.last().name_ );
		QPainter p( &img );
		p.setBrush( QColor( 224, 224, 224 ) );
		p.setPen( QColor( 224, 224, 224 ) );

		int pageProgress=(1-Cconsts::Print::ProgressBarDiv)*img.width();
		int allProgres=Cconsts::Print::ProgressBarDiv*img.width();

		p.drawRect( 0, progressBarGap, pageProgress-progressBarGap, progressBarHeight );
		p.drawRect( progressBarDX, progressBarGap, allProgres, progressBarHeight );

		img.save( outStat_.last().name_, "PNG" );
	}
}

int CprintedPage::numTocItems() {
	int writeArea=image_->height()-2*margin_;
	writeArea -= 7*(Cconsts::Print::SpaceToNextLineInMedian+1)*lineHeight_;
	return writeArea / (lineHeight_*(Cconsts::Print::SpaceToNextLineInMedian+1) ) ;
}

void CprintedPage::createTitlePage(QVector<QPair< QPair<int, int>, CprintedLine> > & toc) {
	image_->setDotsPerMeterX( Cconsts::Print::DotsPerMeter( lineHeight_ ) );
	image_->setDotsPerMeterY( Cconsts::Print::DotsPerMeter( lineHeight_ ) );

	QPainter writer( image_ );
	QRect line;

	writer.setFont( Cconsts::Print::FontItalic() );
	line.setRect( 0, lineHeight_*(Cconsts::Print::SpaceToNextLineInMedian+1)*2.3, image_->width(), lineHeight_*1.3 );
	writer.drawText( line, Qt::AlignCenter|Qt::TextWordWrap, QString("Re-Typeset %1").arg( VERSION ) );

	writer.setFont( Cconsts::Print::Font() );
	line.setRect( 0, 0, image_->width(), lineHeight_*(Cconsts::Print::SpaceToNextLineInMedian+1)*2 );
	writer.drawText( line, Qt::AlignCenter|Qt::TextWordWrap, QObject::tr("Book converted by program" ) );

	line.setRect( 0, lineHeight_*(Cconsts::Print::SpaceToNextLineInMedian+1)*4, image_->width(), lineHeight_ );
	writer.drawText( line, Qt::AlignCenter|Qt::TextWordWrap, "© Piotr Mika, Marcin Garbiak" );

	line.setRect( 0, lineHeight_*(Cconsts::Print::SpaceToNextLineInMedian+1)*6, image_->width(), lineHeight_ );
	writer.drawText( line, Qt::AlignCenter|Qt::TextWordWrap, QDateTime::currentDateTime().toString( Qt::DefaultLocaleShortDate ) );

	for( int i=0; i<toc.size(); ++i ) {
		line.setRect( 0, 2*margin_+(7+i)*(Cconsts::Print::SpaceToNextLineInMedian+1)*lineHeight_, lineHeight_*3, lineHeight_ );
		writer.drawText( line, Qt::AlignRight, QString::number( toc[i].first.first ) + "~" );
		if( toc[i].second.isEmpty() ) {
			line.setLeft( line.right() );
			line.setWidth( lineHeight_*20 );
			writer.drawText( line, Qt::AlignLeft, QObject::tr( "approx num: %1").arg( toc[i].first.second ) );
		} else {
			cursorX=lineHeight_*(3+Cconsts::Print::SpaceInMedian);
			cursorY=line.top();
			toc[i].second.sumLengths_=(toc[i].second.size()-1)*lineHeight_*Cconsts::Print::SpaceInMedian;
			for( int j=0; j<toc[i].second.size(); ++j ) {
				toc[i].second.sumLengths_+=toc[i].second[j].width();
			}
			if( toc[i].second.sumLengths_ <= image_->width()-margin_-cursorX ) {//fits
				for( int j=0; j<toc[i].second.size(); ++j ) {
					writer.drawImage( cursorX, cursorY, toc[i].second[j] );
					cursorX+=toc[i].second[j].width()+lineHeight_*Cconsts::Print::SpaceInMedian;
				}
			} else {//cut
				for( int j=0; j<2; ++j ) {//2 first words always
					if( j<toc[i].second.size() ) {
						writer.drawImage( cursorX, cursorY, toc[i].second[j] );
						cursorX+=toc[i].second[j].width()+lineHeight_*Cconsts::Print::SpaceInMedian;
					}
				}
				if( toc[i].second.size() > 2 ) {//2 last words always
					int j=toc[i].second.size()-1;
					int cursorX2=image_->width()-margin_-toc[i].second[ j ].width();
					writer.drawImage( cursorX2, cursorY, toc[i].second[ j ] );
					cursorX2-=lineHeight_*Cconsts::Print::SpaceInMedian;
					--j;
					if( j > 2 ) {
						cursorX2-=toc[i].second[j].width();
						writer.drawImage( cursorX2, cursorY, toc[i].second[j] );
					}
					cursorX2-=lineHeight_;
					writer.drawText(cursorX2, cursorY-lineHeight_/2, lineHeight_, lineHeight_, Qt::AlignCenter, "..." );

					for( int j=2; j<toc[i].second.size()-2; ++j ) {//fill if there is place
						if( cursorX2 - cursorX > toc[i].second[j].width() ) {
							writer.drawImage( cursorX, cursorY, toc[i].second[j] );
							cursorX+=toc[i].second[j].width()+lineHeight_*Cconsts::Print::SpaceInMedian;
						} else {
							writer.drawImage( cursorX, cursorY, toc[i].second[j].copy( 0, 0, cursorX2-cursorX, toc[i].second[j].height() ) );
							cursorX=cursorX2;
							break;
						}
					}
				}
			}
		}
	}
}


QImage CprintedPage::join(const QImage & a, const QImage & b) {
	int gap = round( Cconsts::Print::SpaceBetweenLetters * lineHeight_ );
	QImage j( a.width()+gap+b.width(), max( a.height(), b.height() ), QImage::Format_RGB32 );
	j.fill( Qt::white );
	QPainter p( &j );
	p.drawImage( 0, 0, a );
	p.drawImage( a.width()+gap, 0, b );

	if( DEbugState_ ) {
		p.setPen( QColor( 255, 0, 0 ) );
		p.drawLine( 0, j.height()-1, j.width(), j.height()-1 );
	}
	return j;
}

