#ifndef CALIBRATEWINDOW_HPP
#define CALIBRATEWINDOW_HPP

#include <QDialog>
#include <QFormLayout>
#include <QImage>

namespace Ui {
class CalibrateWindow;
}

class CalibrateWindow : public QDialog
{
	Q_OBJECT

public:
	explicit CalibrateWindow(QWidget *parent = 0);
	~CalibrateWindow();

	QFormLayout * getParametersFormLayout();

    void setDirPath(const QString &dirPath);

    void setTreshold(int treshold);
    void connectSpinBoxes();

public slots:
    void runAlgortithm();
    void setZoomFit();

private slots:
    void on_previousImageButton_clicked();

    void on_nextImageButton_clicked();    

private:
    void reloadImage();



    Ui::CalibrateWindow *ui;
    int imageNum_;
    int treshold_;
    QString dirPath_;
    QImage inputImage_;
    QImage outputImage_;

};

#endif // CALIBRATEWINDOW_HPP
