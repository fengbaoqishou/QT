#include "freespacewindow.h"
#include "ui_freespacewindow.h"
#include <iostream>
FreespaceWindow::FreespaceWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FreespaceWindow)
{
    ui->setupUi(this);
    setWindowTitle("VisualFreespace");
    mPlot = new QCustomPlot(this);
    mPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    mPlot->yAxis->setRange(0, 448);
    mPlot->xAxis->setRange(0, 448);
    // mPlot->axisRect()->setBackground(QPixmap("./solarpanels.jpg"));

    mImage = new QCPItemPixmap(mPlot);
    mImage->setPixmap(QPixmap(224, 224));
    mImage->topLeft->setType(QCPItemPosition::ptPlotCoords);
    mImage->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    mImage->topLeft->setCoords(0, 224);
    mImage->bottomRight->setCoords(224, 0);
        mPlot->addLayer("image", mPlot->layer("main"), QCustomPlot::limBelow);
    mImage->setLayer("image");
    mImage->setScaled(true,Qt::IgnoreAspectRatio);




    layout_central_widget = new QHBoxLayout;
    setLayout(layout_central_widget);
    layout_central_widget->addWidget(mPlot);
    graph = mPlot->addGraph();
    count = 0;
    freespace_img_nv12 = new unsigned char[224*224*3/2];
    memset(freespace_img_nv12, 0x0, 224*224*3/2);
    freespace_img_rgb32 = new unsigned char[224*224*4];
    memset(freespace_img_rgb32, 0x0, 224*224*4);

    pFrame = av_frame_alloc();
    avpicture_fill((AVPicture *)pFrame, freespace_img_nv12, AV_PIX_FMT_NV12, 224, 224);
    pFrameRGB32 = av_frame_alloc();
    avpicture_fill((AVPicture *)pFrameRGB32, freespace_img_rgb32, AV_PIX_FMT_RGB32, 224, 224);

    sws_nv12_rgb32 = sws_getContext(224, 224, AV_PIX_FMT_NV12, 224, 224, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);


}

FreespaceWindow::~FreespaceWindow()
{
    av_frame_free(&pFrame);
    av_frame_free(&pFrameRGB32);
    sws_freeContext(sws_nv12_rgb32);

    delete ui;
}

void FreespaceWindow::OnP2pLcmMsg(P2pLcmData lcm_data)
{
    if(count%10240==0)
    {
        graph->data().data()->clear();
    }
    count++;
    graph->addData(count, lcm_data.shift_pos);
    mPlot->xAxis->setRange(0, 224);
    mPlot->replot(QCustomPlot::rpQueuedReplot);
    // std::cout<<"plot window get msg"<<std::endl;



}

void FreespaceWindow::OnFreespaceResultMsg(unsigned char* freespace_result)
{
    memcpy(freespace_img_nv12, freespace_result, 224*224*3/2);
    sws_scale(sws_nv12_rgb32, pFrame->data, pFrame->linesize, 0, 224, pFrameRGB32->data, pFrameRGB32->linesize);
    // fwrite(rgbBuffer,1,numBytes,fp_rgb);
    QImage image = QImage((uchar *)freespace_img_rgb32, 224, 224, QImage::Format_RGB32);
    mImage->setPixmap(QPixmap::fromImage(image));
    mImage->setScaled(false);
    mImage->setScaled(true,Qt::IgnoreAspectRatio);
    mPlot->replot(QCustomPlot::rpQueuedReplot);
    // std::cout<<"OnFreespaceResultMsg"<<std::endl;


}
