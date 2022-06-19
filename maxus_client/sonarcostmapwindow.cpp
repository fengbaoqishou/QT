#include "sonarcostmapwindow.h"
#include "ui_sonarcostmapwindow.h"
#include <iostream>
#include <fstream>
SonarCostMapWindow::SonarCostMapWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SonarCostMapWindow)
{
    ui->setupUi(this);
    setWindowTitle("SonarCostMap");
    sonar_cost_map_data = new unsigned char[160*160];
    memset(sonar_cost_map_data, 0x80, 160*160);
    std::ifstream file_cost_map("p2p\\map\\sonar_cost_map.bin", std::ifstream::binary);
    if(file_cost_map.is_open())
    {
        file_cost_map.read(reinterpret_cast<char*>(sonar_cost_map_data), 160*160);
        file_cost_map.close();
        std::cout<<"read cost map ok"<<std::endl;
    }


    mPlot = new QCustomPlot(this);
    mPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    mPlot->yAxis->setRange(-80, 80);
    mPlot->xAxis->setRange(-80, 80);
    // mPlot->axisRect()->setBackground(QPixmap("./solarpanels.jpg"));

    mImage = new QCPItemPixmap(mPlot);
    // mImage->setPixmap(QPixmap(160, 160));

    QImage image = QImage((uchar *)sonar_cost_map_data, 160, 160, QImage::Format_Grayscale8);
    mImage->setPixmap(QPixmap::fromImage(image));

    mImage->topLeft->setType(QCPItemPosition::ptPlotCoords);
    mImage->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    mImage->topLeft->setCoords(-80, 80);
    mImage->bottomRight->setCoords(80, -80);
    mPlot->addLayer("image", mPlot->layer("main"), QCustomPlot::limBelow);
    mImage->setLayer("image");
    mImage->setScaled(true,Qt::IgnoreAspectRatio);




    layout_central_widget = new QHBoxLayout;
    setLayout(layout_central_widget);
    layout_central_widget->addWidget(mPlot);
    graph = mPlot->addGraph();
    count = 0;




}

SonarCostMapWindow::~SonarCostMapWindow()
{

    delete ui;
}

void SonarCostMapWindow::OnP2pLcmMsg(P2pLcmData lcm_data)
{


}

void SonarCostMapWindow::OnFreespaceResultMsg(unsigned char* freespace_result)
{
    memcpy(sonar_cost_map_data, freespace_result, 160*160);
    // fwrite(rgbBuffer,1,numBytes,fp_rgb);
    QImage image = QImage((uchar *)sonar_cost_map_data, 160, 160, QImage::Format_Grayscale8);
    mImage->setPixmap(QPixmap::fromImage(image));
    mImage->setScaled(false);
    mImage->setScaled(true,Qt::IgnoreAspectRatio);
    mPlot->replot(QCustomPlot::rpQueuedReplot);
    // std::cout<<"OnFreespaceResultMsg"<<std::endl;


}
