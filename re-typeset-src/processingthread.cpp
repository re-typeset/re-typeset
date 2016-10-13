#include "processingthread.hpp"

#include "scanneddocument.hpp"

ProcessingThread::ProcessingThread() {
	;//NOOP
}

ProcessingThread::~ProcessingThread() {
	;//NOOP
}

void ProcessingThread::run() {
	ScannedDocument doc( srcDir, destDir, &work, DEbugState );

	connect( &doc, SIGNAL( newProgressBarValue(int,QString,int,int) ), progressBarSignalReciever, SLOT( setProgressBar(int,QString,int,int) ) );

	doc.loadPages( treshold );
	doc.findWords( comicMode, findDividedWords  );

	status=doc.print( width, height, margin, fontHeight, hardMargins, noUpscalling, fullColor,
					  justify, rotateImages, comicMode, fileNamePrefix, author, title, equalizeHistogram, createHelperScript );
}

