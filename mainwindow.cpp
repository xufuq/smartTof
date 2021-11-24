#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MyTof=new Tof(1, "D:/Programs/Qt/QtFile/smartTof2/Resource/test.oni");
    CapTimer=new QTimer();
    connect(CapTimer, SIGNAL(timeout()),this,SLOT(timerCall()));
    connect(ui->StartButton,SIGNAL(clicked()),this,SLOT(startClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startClicked()
{
    if(!IfStarted)
    {
        MyTof->myDmcam_Init();
        CapTimer->start(40);
        ui->StartButton->setText("停止程序");
        isFirstImage=true;
    }
    else
    {
        MyTof->myDmcam_Uninit();
        CapTimer->stop();
        ui->StartButton->setText("开始程序");
        isFirstImage=false;
    }
    IfStarted=!IfStarted;
}

void MainWindow::timerCall()
{
    timeCount++;
    MyTof->getFrame();
    /*
    cv::Mat GrayImage(240,320, CV_16UC1, MyTof->gray_u16), temp;
    int height=GrayImage.rows, width=GrayImage.cols;
    cv::Mat dst = cv::Mat::zeros(height, width, CV_8UC1);//先生成空的目标图片
    double minv = 0.0, maxv = 0.0;
    double* minp = &minv;
    double* maxp = &maxv;
    cv::minMaxIdx(GrayImage, minp, maxp);  //取得像素值最大值和最小值
    //用指针访问像素，速度更快
   ushort *p_img;
   uchar *p_dst;
   for (int i = 0; i < height; i++)
   {
        p_img = GrayImage.ptr<ushort>(i);//获取每行首地址
        p_dst = dst.ptr<uchar>(i);
        for (int j = 0; j < width; ++j)
            p_dst[j] = (p_img[j] - minv) / (maxv - minv) * 255;
    } // 最终只要访问dst就可以了
    const uchar *pSrc = (const uchar*)dst.data;
    QImage *GrayQImage=new QImage(pSrc, 320, 240, dst.step, QImage::Format_Grayscale8);
    ui->GrayLabel->setPixmap(QPixmap::fromImage(*GrayQImage));*/

    // 使用官方SDK可以得到灰度图
    // 使用官方SDK可以得到灰度图
    cv::Mat GrayImage(240,320, CV_8UC1, MyTof->IrGray);
    // 这里深度图用灰度形式显示，计算
    cv::Mat DepthImage(240,320,CV_8UC1, MyTof->DistGrayU8);
    cv::Mat dest=DepthImage.clone();
//    cv::imshow("dest",dest);

    if(isFirstImage)
    {
        MyVibe=new Vibe(dest);
        isFirstImage=false;
        MyVibe->SetRadius(9);
    }
    else
    {
        qDebug() << "正在检测" << endl;
        MyVibe->Detect(dest);
        MyVibe->ForegroundCombineEdge();
        cv::Mat SegImage=MyVibe->GetSegMask();

        cv::Mat src = cv::Mat(SegImage), FinalImage = src.clone(),  hiearchy;
        std::vector<cv::Mat> contours;

        cv::Mat kernal=cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
        cv::erode(src, src, kernal, cv::Point(-1,-1), 2);
        cv::dilate(src,src, kernal, cv::Point(-1,-1), 20);
        cv::Canny(src, src, 0, 255);
        cv::findContours(src, contours, hiearchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));
        if(contours.size()>0)
            ui->TextOut->setText("检测到入侵");
        else
            ui->TextOut->setText("");
        cv::cvtColor(DepthImage, DepthImage, cv::COLOR_GRAY2BGR);
        cv::cvtColor(GrayImage, GrayImage, cv::COLOR_GRAY2BGR);
        if(ui->ShowDepth->isChecked())
            cv::drawContours(DepthImage, contours, -1, cv::Scalar(0,0,255), 2);
        if(ui->ShowGray->isChecked())
            cv::drawContours(GrayImage, contours, -1, cv::Scalar(0,0,255), 2);
    }

    const uchar *pSrc = (const uchar*)GrayImage.data;
    QImage *GrayQImage=new QImage(pSrc, 320, 240, GrayImage.step, QImage::Format_BGR888);
    ui->GrayLabel->setPixmap(QPixmap::fromImage(*GrayQImage));

    pSrc = (const uchar*)DepthImage.data;
    QImage *DistGrayQImage=new QImage(pSrc, 320, 240, DepthImage.step, QImage::Format_BGR888);
    ui->DepthLabel->setPixmap(QPixmap::fromImage(*DistGrayQImage));
}
