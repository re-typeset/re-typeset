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

#include "scanneddocument.hpp"
#include "printedline.hpp"

CscannedDocument::CscannedDocument() {
	;//NOOP
}

CscannedDocument::CscannedDocument(QString srcDir , QString destDir, bool * work, bool DEbugState):
	srcDir_( srcDir ), destDir_( destDir ), work_(work), DEbugState_( DEbugState ),
	number_(CscannedPage::None), header_(false) { //logic about headers
	;//NOOP
}

CscannedDocument::~CscannedDocument() {
	if( DEbugState_ == false ) {
		for( int i=0; i<pages_.size(); ++i ) {
			pages_[i].removeMonoImage();
		}
	}
}

int CscannedDocument::loadPages(int treshold) {
	QDir dir(srcDir_);
	QStringList filesInDir=dir.entryList(
					  QDir::Files | QDir::NoDotAndDotDot,
					  QDir::Name );

	newProgressBarValue( 0, tr("loading images"), Cconsts::Progress::LoadNumber, Cconsts::Progress::TotalNumber );
	if( ! (*work_) ) {
		return 0;
	}

	for( int i=0, size=filesInDir.size(); i<size; ++i ) {

		newProgressBarValue( i*Cconsts::Progress::LoadPercent/size+Cconsts::Progress::LoadDelay );
		if( ! (*work_) ) {
			return 0;
		}

		QString fileColor = srcDir_ + "/" + filesInDir[i];
		QString fileMono = destDir_ + "/TMP_" + filesInDir[i] + ".png";
		QImage imageColor( fileColor );
		QImage imageMono( imageColor.size(), QImage::Format_Mono );
		const uint white=0;
		const uint black=1;
		for( int x=0; x<imageColor.width(); ++x ) {
			for( int y=0; y<imageColor.height(); ++y ) {
				if( qGray( imageColor.pixel( x, y ) ) > treshold ) {
					imageMono.setPixel( x, y, black );
				} else {
					imageMono.setPixel( x, y, white );
				}
			}
		}
		imageMono.save( fileMono, "PNG" );

		CscannedPage page( fileMono, fileColor );
		pages_.push_back( page );
	}

	return pages_.size();
}

void CscannedDocument::findWords(bool comicMode, bool findDividedWords) {
	if( ! (*work_) ) {
		return;
	}
	newProgressBarValue( Cconsts::Progress::FindDelay, tr("finding words"), Cconsts::Progress::FindNumber, Cconsts::Progress::TotalNumber );

	for( int i=0; i<pages_.size(); ++i ) {
		CscannedPage & page(pages_[i]);

		page.getLinesAndHeights( lineStats_ );
	}
	lineStats_.calculateParams();
	lineStats_.clear();
	if( comicMode ) {
		lineStats_.setComicMode();
	}

	const CstatsPack & stats=lineStats_.getStats();
	CstatsNumberHeader numHead;
	for( int i=0; i<pages_.size(); ++i ) {
		CscannedPage & page=pages_[i];

		newProgressBarValue( i*Cconsts::Progress::FindPercent/pages_.size()+Cconsts::Progress::FindDelay );
		if( ! (*work_) ) {
			return;
		}

		page.reConnectDots( stats );
		page.trimDivideLines( stats );
		page.connectDescriptionsToImages( stats );
		page.findParagraphs( stats );
		if( findDividedWords ) {
			page.findDividedWords();
		}
		page.checkNumberHeader( stats, numHead );
	}

	if( numHead.header_ >= pages_.size()*Cconsts::NumberHeader::CorrectToAllCoefficient ) {
		header_=true;
		if( numHead.numberBottom_ >= pages_.size()*Cconsts::NumberHeader::CorrectToAllCoefficient ) {
			number_=CscannedPage::Bottom;
		}
	} else if( numHead.numberTop_ >= numHead.numberBottom_ ) {
		if( numHead.numberTop_ >= pages_.size()*Cconsts::NumberHeader::CorrectToAllCoefficient ) {
			number_=CscannedPage::Top;
		}
	} else {
		if( numHead.numberBottom_ >= pages_.size()*Cconsts::NumberHeader::CorrectToAllCoefficient ) {
			number_=CscannedPage::Bottom;
		}
	}


	for( int i=0; i<pages_.size(); ++i ) {
		CscannedPage & page=pages_[i];

		page.setNumberHeader( stats, number_, header_ );

		if( DEbugState_ == true ) {
			page.printDEbugImages( stats );
		}
	}
}

int CscannedDocument::print(int width, int height, int margin, int fontHeight, bool hardMargins, bool noUpscalling, bool fullColor, bool justify, bool rotateImages, bool comicMode, QString fileNamePrefix) {
	if( ! (*work_) ) {
		return 0;
	}
	newProgressBarValue( Cconsts::Progress::PrintDelay, tr("generating images"), Cconsts::Progress::PrintNumber, Cconsts::Progress::TotalNumber );

	CprintedLine paragraph;
	CprintedLine numHead;
	const CstatsPack & stats=lineStats_.getStats();
	int number=1; //0 for titlepage
	double scalingRatio=(double)fontHeight/(double)stats.height_;
	if( noUpscalling && scalingRatio > 1 ) {
		scalingRatio=1;
		fontHeight=stats.height_;
	}
	int maxWordLength=Cconsts::MaxWordLengthInOutPageWidth*(width-2*margin)/scalingRatio;
	CprintedPage destPage( width, height, margin, fontHeight, justify, rotateImages, comicMode, DEbugState_ );

	double numTocStep=(double)pages_.size()/destPage.numTocItems();
	double numTocCurrentStep=numTocStep;
	QVector<QPair<QPair<int, int >, CprintedLine> > toc;
	QPair<QPair<int, int >, CprintedLine> tocItem;

	if( ! comicMode ) {
		pages_[0].getNumHead( stats, numHead, scalingRatio, maxWordLength, fullColor );
	}
	destPage.addNumberHeader( numHead, 0 );
	int i;
	for( i=0; i<pages_.size(); ++i ) {
		newProgressBarValue( i*Cconsts::Progress::PrintPercent/pages_.size()+Cconsts::Progress::PrintDelay );
		if( ! (*work_) ) {
			return 0;
		}

		CscannedPage & srcPage=pages_[i];

		if( number_!=CscannedPage::None || header_!=false ) {
			srcPage.getNumHead( stats, numHead, scalingRatio, maxWordLength, fullColor );
		}

		while( srcPage.getParagraph( stats, paragraph, scalingRatio, maxWordLength, fullColor ) ) {//false==page completed

			newProgressBarValue( i*Cconsts::Progress::PrintPercent/pages_.size()+Cconsts::Progress::PrintDelay );
			if( ! (*work_) ) {
				return 0;
			}

			while( destPage.addParagraph( paragraph ) ) {//true==page completed, save&clear
				++number;
				destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_"
									   + QString("%1").arg( number, 3, 10, QChar('0') ) + ".png", hardMargins );
				destPage.addNumberHeader( numHead, i );

				if( numTocCurrentStep < i ) {
					if( number_!=CscannedPage::None || header_!=false ) {
						if( numHead.size() > 0 ) {
							tocItem.first.first=number+1;
							tocItem.second=numHead;
							numTocCurrentStep+=numTocStep;
							toc.push_back( tocItem );
						}
					} else {
						tocItem.first.first=number;
						tocItem.first.second=i;
						numTocCurrentStep+=numTocStep;
						toc.push_back( tocItem );
					}
				}

				while( destPage.printImagesFromQueue() ) {
					++number;
					destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_"
										   + QString("%1").arg( number, 3, 10, QChar('0') ) + ".png", hardMargins );
					destPage.addNumberHeader( numHead, i );
				}
			}
		}

		while( destPage.addParagraph( paragraph ) ) {//true==page completed, save&clear
			++number;
			destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_"
								   + QString("%1").arg( number, 3, 10, QChar('0') ) + ".png", hardMargins );
			destPage.addNumberHeader( numHead, i );
			while( destPage.printImagesFromQueue() ) {
				++number;
				destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_"
									   + QString("%1").arg( number, 3, 10, QChar('0') ) + ".png", hardMargins );
				destPage.addNumberHeader( numHead, i );
			}
		}
	}
	while( destPage.addParagraph( paragraph ) ) {
		++number;
		destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_"
							   + QString("%1").arg( number, 3, 10, QChar('0') ) + ".png", hardMargins );
		destPage.addNumberHeader( numHead, i );
		while( destPage.printImagesFromQueue() ) {
			++number;
			destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_"
								   + QString("%1").arg( number, 3, 10, QChar('0') ) + ".png", hardMargins );
			destPage.addNumberHeader( numHead, i );
		}
	}
	if( destPage.printLastLine() ) {
		destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_"
							   + QString("%1").arg( number, 3, 10, QChar('0') ) + ".png", hardMargins );
		destPage.addNumberHeader( numHead, i );
		destPage.printLastLine();
	}

	++number;
	destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_"
						   + QString("%1").arg( number, 3, 10, QChar('0') ) + ".png", hardMargins );
	while( destPage.printImagesFromQueue( true ) ) {
		++number;
		destPage.addNumberHeader( numHead, i );
		destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_"
							   + QString("%1").arg( number, 3, 10, QChar('0') ) + ".png", hardMargins );
	}

	if( ! comicMode ) {
		newProgressBarValue( Cconsts::Progress::ProgBarDelay, tr("generating pages' progress bars"), Cconsts::Progress::ProgBarNumber, Cconsts::Progress::TotalNumber );
		destPage.addProgressBarsForAllPages();
	}

	destPage.createTitlePage( toc );
	destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_001.png", hardMargins );

	return number-1;
}
