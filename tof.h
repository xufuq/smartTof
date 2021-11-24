#ifndef TOF_H
#define TOF_H

#include <QObject>
#include <QDebug>
extern "C"
{
    #include "dmcam.h"
}

class Tof : public QObject
{
    Q_OBJECT
public:
    explicit Tof(bool capvideo, QString filename, QObject *parent = nullptr);

    void myDmcam_Init();
    void getFrame();
    void myDmcam_Uninit();

    uint8_t *fbuf;
    float *dist_f32;
    uint16_t *gray_u16, *dist_u16;
    uint8_t *IrGray, *DistGrayU8;

signals:

private:
    dmcam_dev_t *dev;
    bool CapVideo=false;
    QString FileName;
    dmcam_cap_cfg_t cap_cfg;
};

#endif // TOF_H
