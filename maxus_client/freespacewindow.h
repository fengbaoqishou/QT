#ifndef FREESPACEWINDOW_H
#define FREESPACEWINDOW_H

#include <QDialog>
#include "qcustomplot.h"
#include "p2p_lcm_data.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
};
namespace Ui {
class FreespaceWindow;
}

class FreespaceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FreespaceWindow(QWidget *parent = 0);
    ~FreespaceWindow();
private slots:
    void OnP2pLcmMsg(P2pLcmData lcm_data);
    void OnFreespaceResultMsg(unsigned char* freespace_result);
private:
    Ui::FreespaceWindow *ui;
    QCustomPlot *mPlot;
    QWidget *central_widget;
    QHBoxLayout* layout_central_widget;
    int count;
    QCPGraph* graph;

    QCPItemPixmap* mImage;
    unsigned char* freespace_img_nv12;
    unsigned char* freespace_img_rgb32;

    struct SwsContext* sws_nv12_rgb32;
    AVFrame* pFrame;
    AVFrame* pFrameRGB32;


};

#endif // PLOTWINDOW_H
