#ifndef CPROCESSINGTHREAD_HPP
#define CPROCESSINGTHREAD_HPP
#include <QThread>

class ProcessingThread : public QThread
{
	//Q_OBJECT

public:
	ProcessingThread();
	virtual ~ProcessingThread();

	void run();

	QString srcDir;
	QString destDir;
	bool DEbugState;
	int treshold;
	bool comicMode;
	bool findDividedWords;
	int width;
	int height;
	int margin;
	int fontHeight;
	bool hardMargins;
	bool noUpscalling;
	bool fullColor;
	bool justify;
	bool rotateImages;
	bool equalizeHistogram;
	QString fileNamePrefix;
	QString author;
	QString title;
	int status;
	bool work;
	bool createHelperScript;

	QObject * progressBarSignalReciever;

};

#endif // CPROCESSINGTHREAD_HPP
