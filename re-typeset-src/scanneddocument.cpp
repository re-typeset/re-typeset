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
#include "extendedimage.hpp"

ScannedDocument::ScannedDocument() {
	;//NOOP
}

ScannedDocument::ScannedDocument(QString srcDir , QString destDir, bool * work, bool DEbugState):
	srcDir_( srcDir ), destDir_( destDir ), work_(work), DEbugState_( DEbugState ),
	number_(ScannedPage::None), header_(false) { //logic about headers
	;//NOOP
}

ScannedDocument::~ScannedDocument() {
	if( DEbugState_ == false ) {
		for( int i=0; i<pages_.size(); ++i ) {
			pages_[i].removeMonoImage();
		}
	}
}

int ScannedDocument::loadPages(int treshold) {
	QDir dir(srcDir_);
	QStringList filesInDir=dir.entryList(
					  QDir::Files | QDir::NoDotAndDotDot,
					  QDir::Name );

	newProgressBarValue( 0, tr("loading images"), Consts::Progress::LoadNumber, Consts::Progress::TotalNumber );
	if( ! (*work_) ) {
		return 0;
	}

	for( int i=0, size=filesInDir.size(); i<size; ++i ) {

		newProgressBarValue( i*Consts::Progress::LoadPercent/size+Consts::Progress::LoadDelay );
		if( ! (*work_) ) {
			return 0;
		}

		QString fileColor = srcDir_ + "/" + filesInDir[i];
		QString fileMono = destDir_ + "/TMP_" + filesInDir[i] + ".png";
		ExtendedImage imageColor( fileColor );
		QImage imageMono = imageColor.convertToMonoImage( treshold );

		imageMono.save( fileMono, "PNG" );

		ScannedPage page( fileMono, fileColor );
		pages_.push_back( page );
	}

	return pages_.size();
}

void ScannedDocument::findWords(bool comicMode, bool findDividedWords) {
	if( ! (*work_) ) {
		return;
	}
	newProgressBarValue( Consts::Progress::FindDelay, tr("finding words"), Consts::Progress::FindNumber, Consts::Progress::TotalNumber );

	for( int i=0; i<pages_.size(); ++i ) {
		ScannedPage & page(pages_[i]);

		page.getLinesAndHeights( lineStats_ );
	}
	lineStats_.calculateParams();
	lineStats_.clear();
	if( comicMode ) {
		lineStats_.setComicMode();
	}

	const StatsPack & stats=lineStats_.getStats();
	StatsNumberHeader numHead;
	for( int i=0; i<pages_.size(); ++i ) {
		ScannedPage & page=pages_[i];

		newProgressBarValue( i*Consts::Progress::FindPercent/pages_.size()+Consts::Progress::FindDelay );
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

	if( numHead.header_ >= pages_.size()*Consts::NumberHeader::CorrectToAllCoefficient ) {
		header_=true;
		if( numHead.numberBottom_ >= pages_.size()*Consts::NumberHeader::CorrectToAllCoefficient ) {
			number_=ScannedPage::Bottom;
		}
	} else if( numHead.numberTop_ >= numHead.numberBottom_ ) {
		if( numHead.numberTop_ >= pages_.size()*Consts::NumberHeader::CorrectToAllCoefficient ) {
			number_=ScannedPage::Top;
		}
	} else {
		if( numHead.numberBottom_ >= pages_.size()*Consts::NumberHeader::CorrectToAllCoefficient ) {
			number_=ScannedPage::Bottom;
		}
	}


	for( int i=0; i<pages_.size(); ++i ) {
		ScannedPage & page=pages_[i];

		page.setNumberHeader( stats, number_, header_ );

		if( DEbugState_ == true ) {
			page.printDEbugImages( stats );
		}
	}
}

int ScannedDocument::print(int width, int height, int margin, int fontHeight, bool hardMargins, bool noUpscalling, bool fullColor, bool justify, bool rotateImages, bool comicMode, QString fileNamePrefix, QString author, QString title, bool equalizeHistogram ) {
	if( ! (*work_) ) {
		return 0;
	}
	newProgressBarValue( Consts::Progress::PrintDelay, tr("generating images"), Consts::Progress::PrintNumber, Consts::Progress::TotalNumber );

	PrintedLine paragraph;
	PrintedLine numHead;
	const StatsPack & stats=lineStats_.getStats();
	int number=2; //0 for titlepage, 1 for toc
	double scalingRatio=(double)fontHeight/(double)stats.height_;
	if( noUpscalling && scalingRatio > 1 ) {
		scalingRatio=1;
		fontHeight=stats.height_;
	}
	int maxWordLength=Consts::MaxWordLengthInOutPageWidth*(width-2*margin)/scalingRatio;
    PrintedPage destPage( width, height, margin, fontHeight, justify, rotateImages, comicMode, equalizeHistogram, author, title, DEbugState_ );

	double numTocStep=(double)pages_.size()/destPage.numTocItems();
	double numTocCurrentStep=numTocStep;
	QVector<QPair<QPair<int, int >, PrintedLine> > toc;
	QPair<QPair<int, int >, PrintedLine> tocItem;

	if( ! comicMode ) {
		pages_[0].getNumHead( stats, numHead, scalingRatio, maxWordLength, fullColor );
	}
	destPage.addNumberHeader( numHead, 0 );
	int i;
	for( i=0; i<pages_.size(); ++i ) {
		newProgressBarValue( i*Consts::Progress::PrintPercent/pages_.size()+Consts::Progress::PrintDelay );
		if( ! (*work_) ) {
			return 0;
		}

		ScannedPage & srcPage=pages_[i];

		if( number_!=ScannedPage::None || header_!=false ) {
			srcPage.getNumHead( stats, numHead, scalingRatio, maxWordLength, fullColor );
		}

		while( srcPage.getParagraph( stats, paragraph, scalingRatio, maxWordLength, fullColor ) ) {//false==page completed

			newProgressBarValue( i*Consts::Progress::PrintPercent/pages_.size()+Consts::Progress::PrintDelay );
			if( ! (*work_) ) {
				return 0;
			}

			while( destPage.addParagraph( paragraph ) ) {//true==page completed, save&clear
				++number;
				destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_"
									   + QString("%1").arg( number, 3, 10, QChar('0') ) + ".png", hardMargins );
				destPage.addNumberHeader( numHead, i );

				if( numTocCurrentStep < i ) {
					if( number_!=ScannedPage::None || header_!=false ) {
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


	newProgressBarValue( Consts::Progress::ProgBarDelay, tr("generating pages' progress bars"), Consts::Progress::ProgBarNumber, Consts::Progress::TotalNumber );
	destPage.addProgressBarsForAllPages( comicMode );


	destPage.createTocPage( toc );
	destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_002.png", hardMargins );

    destPage.createTitlePage();
	destPage.saveAndClear( destDir_ + "/" + fileNamePrefix + "_001.png", hardMargins );


    return number-1;
}
