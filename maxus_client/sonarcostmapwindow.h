#ifndef SONARCOSTMAPWINDOW_H
#define SONARCOSTMAPWINDOW_H

#include <QDialog>
#include "qcustomplot.h"
#include "p2p_lcm_data.h"

namespace Ui {
class SonarCostMapWindow;
}

class SonarCostMapWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SonarCostMapWindow(QWidget *parent = 0);
    ~SonarCostMapWindow();
private slots:
    void OnP2pLcmMsg(P2pLcmData lcm_data);
    void OnFreespaceResultMsg(unsigned char* freespace_result);
private:
    Ui::SonarCostMapWindow *ui;
    QCustomPlot *mPlot;
    QWidget *central_widget;
    QHBoxLayout* layout_central_widget;
    int count;
    QCPGraph* graph;

    QCPItemPixmap* mImage;
    unsigned char* sonar_cost_map_data;


};

#endif // PLOTWINDOW_H
