/* 用于界面图像显示
 * 不需要改动
 */

#ifndef DRAWIMAGE_H
#define DRAWIMAGE_H

#include <QWidget>
#include <QPainter>
#include <QPalette>      //设置背景颜色等
#include <QImage>

class DrawImage : public QWidget
{
    Q_OBJECT
public:
    explicit DrawImage(QString backPath, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void PassImage(QImage imagedata);
signals:

public slots:

private:
    QPixmap  Back_ground;
    QImage   qimage;
    QVector<QRgb> vcolorTable;
};

#endif // DRAWIMAGE_H
