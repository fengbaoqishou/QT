#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QLabel>
#include <QTextBrowser>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <fstream>

#include "example_t.hpp"
#include "lcmthread.h"
#include "qcustomplot.h"
#include "plotwindow.h"
#include "apa_client.h"
#include "p2p_client.h"
#include "vpld_client.h"
#include "vdr_client.h"

#include "freespacewindow.h"
#include "sonarcostmapwindow.h"
#include "slot_curve.h"
namespace Ui {
class PlotWindow;
class MainWindow;
}




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:

    // void UpdateP2pLcmMsg(P2pLcmData lcm_data);
    // void OnApaPldResult(ApaValidSlots valid_slots);

    // void OnDataButtonClicked();
    // void OnPlotRoute1ButtonClicked();
    // void OnPlotRoute2ButtonClicked();
    // void OnCopyP2pDataButtonClicked();
    // void OnSendP2pCmdButtonClicked();

    // //p2p cmd push button onClicked slot
    // void OnStartEnvironmentDetectionButtonClicked();
    // void OnStartRouteLearningButtonClicked();
    // void OnEndRouteLearningButtonClicked();
    // void OnCancleRouteLearningButtonClicked();
    // void OnSaveRoute1ButtonClicked();
    // void OnSaveRoute3ButtonClicked();
    // void OnStartMappingButtonClicked();
    // void OnDeleteRoute1ButtonClicked();
    // void OnDeleteRoute2ButtonClicked();
    // void OnStopRouteDeleteButtonClicked();
    // void OnMatchRoute1ButtonClicked();
    // void OnMatchRoute2ButtonClicked();
    // void OnStopRouteMatchButtonClicked();
    // void OnLockPointAButtonClicked();
    // void OnStartRouteReplayButtonClicked();
    // void OnEndRouteReplayButtonClicked();
    // void OnRelocatePointBButtonClicked();
    // void OnFinalArrieveButtonClicked();
    // void OnQuitP2PButtonClicked();
    // void OnP2pReqPldButtonClicked();
    // void OnP2ppVpCtrlButtonClicked();
    // void OnP2pParkReqButtonClicked();

    void PlotShift();
    void PlotFreespace();
    void PlotSonarCostMap();
    
private:
    Ui::MainWindow *ui;
    PlotWindow* mp_plot_window;
    QMenu* mp_plot_menu;
    QAction* mp_plot_shift;

    LcmThread* mp_lcm_thread;
    ApaClient* apa_client;
    P2pClient* p2p_client;
    VpldClient* vpld_client_;
    VdrClient* vdr_client_;

    QTabWidget* tabWidget;
    lcm::LCM lcm_sender;

    FreespaceWindow* mp_freespace_window;
    QAction* mp_plot_freespace;

    SonarCostMapWindow* mp_sonar_cost_map_window;
    QAction* mp_plot_sonar_cost_map;


};

#endif // MAINWINDOW_H
