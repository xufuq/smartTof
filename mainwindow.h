#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QString>
#include <QImage>
#include <QDebug>
#include <QTextEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFileDialog>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs/legacy/constants_c.h>

#include "drawimage.h"
#include "tof.h"
#include "Vibe.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Vibe *MyVibe;
private:
    Ui::MainWindow *ui;
    QTimer *CapTimer;
    bool IfStarted=false;
    Tof *MyTof;
    bool isFirstImage=true;
    int timeCount=0;
public slots:
    void startClicked();
    void timerCall();
};



#endif // MAINWINDOW_H
