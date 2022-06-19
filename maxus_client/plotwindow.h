#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QDialog>
#include "qcustomplot.h"
#include "p2p_lcm_data.h"
namespace Ui {
class PlotWindow;
}

class PlotWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = 0);
    ~PlotWindow();
private slots:
    void OnP2pLcmMsg(P2pLcmData lcm_data);
private:
    Ui::PlotWindow *ui;
    QCustomPlot *mPlot;
    QWidget *central_widget;
    QHBoxLayout* layout_central_widget;
    int count;
    QCPGraph* graph;
};

#endif // PLOTWINDOW_H
