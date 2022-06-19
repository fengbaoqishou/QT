#include "plotwindow.h"
#include "ui_plotwindow.h"
#include <iostream>
PlotWindow::PlotWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);
    setWindowTitle("RealTimePlot");
    mPlot = new QCustomPlot(this);
    mPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    mPlot->yAxis->setRange(-1, 10);

    layout_central_widget = new QHBoxLayout;
    setLayout(layout_central_widget);
    layout_central_widget->addWidget(mPlot);
    graph = mPlot->addGraph();
    count = 0;

}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::OnP2pLcmMsg(P2pLcmData lcm_data)
{
    if(count%10240==0)
    {
        graph->data().data()->clear();
    }
    count++;
    graph->addData(count, lcm_data.shift_pos);
    mPlot->xAxis->setRange(count+100, 1024, Qt::AlignRight);
    mPlot->replot(QCustomPlot::rpQueuedReplot);
    //std::cout<<"plot window get msg"<<std::endl;
}