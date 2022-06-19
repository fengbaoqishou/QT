#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <iostream>

#ifdef WIN_MSVC_X86_64
#include <windows.h>
#include "shellapi.h "
#endif

#ifdef LINUX_GCC_X86_64
#include <unistd.h>
#endif

#include "plotwindow.h"
#include "ui_plotwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("MonsterClient");
    setGeometry(400,100,1280,720);

    
    // QFont font  = MainWindow::font();
    // font.setPointSize(8);
    // MainWindow::setFont(font);

    //YHL: create menu
    mp_plot_shift = new QAction(this);
    mp_plot_shift->setText(tr("PlotShift"));
    mp_plot_shift->setStatusTip("plot shift");
    mp_plot_shift->setShortcut(QKeySequence("Ctrl+1")); //YHL: create shortcut

    // FreespaceWindow* mp_freespace_window;
    mp_plot_freespace = new QAction(this);
    mp_plot_freespace->setText(tr("PlotFreespace"));
    mp_plot_freespace->setStatusTip("plot freespace");
    mp_plot_freespace->setShortcut(QKeySequence("Ctrl+2")); //YHL: create shortcut

    mp_plot_sonar_cost_map = new QAction(this);
    mp_plot_sonar_cost_map->setText(tr("PlotSonarCostMap"));
    mp_plot_sonar_cost_map->setStatusTip("plot sonar cost map");
    mp_plot_sonar_cost_map->setShortcut(QKeySequence("Ctrl+3")); 

    ui->mainToolBar->addAction(mp_plot_shift);           //YHL: create tool bar
    ui->mainToolBar->addAction(mp_plot_freespace);           //YHL: create tool bar
    ui->mainToolBar->addAction(mp_plot_sonar_cost_map);

    connect(mp_plot_shift, SIGNAL(triggered()), this, SLOT(PlotShift()));
    connect(mp_plot_freespace, SIGNAL(triggered()), this, SLOT(PlotFreespace()));
    connect(mp_plot_sonar_cost_map, SIGNAL(triggered()), this, SLOT(PlotSonarCostMap()));


    // ui->mainToolBar->hide();




    //YHL: get lcm thread instance
    mp_lcm_thread = LcmThread::GetInstance();


    //YHL: tab wid
    tabWidget = new QTabWidget();
    apa_client = new ApaClient(this);
    p2p_client = new P2pClient(this);
    vpld_client_ = new VpldClient(this);
    vdr_client_ = new VdrClient(this);

    // central_widget = new QWidget;
    // setCentralWidget(central_widget);
    setCentralWidget(tabWidget);

    tabWidget->addTab(apa_client->GetWidget(), "APA");
    tabWidget->addTab(p2p_client->GetWidget(), "P2P");
    tabWidget->addTab(vpld_client_->GetWidget(), "VPLD");
    tabWidget->addTab(vdr_client_->GetWidget(), "VDR");


    mp_plot_menu = menuBar()->addMenu(tr("Plot"));
    mp_plot_menu->addAction(mp_plot_shift);
    mp_plot_menu->addAction(mp_plot_freespace);
    mp_plot_menu->addAction(mp_plot_sonar_cost_map);


    lcm_sender = lcm_create("udpm://239.255.76.67:7667?ttl=100");
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender create failed!"<<std::endl;
    }
    else
    {
        std::cout<<"lcm sender create ok!"<<std::endl;
    }





    qRegisterMetaType<P2pLcmData>("P2pLcmData");
    qRegisterMetaType<ApaValidSlots>("ApaValidSlots");
    qRegisterMetaType<ApaLcmData>("ApaLcmData");
    qRegisterMetaType<AvmEvent_ApaResult>("AvmEvent_ApaResult");

    qRegisterMetaType<VpldResult>("VpldResult");
    qRegisterMetaType<VehicleDeadReckoning>("VehicleDeadReckoning");
    qRegisterMetaType<VehicleModelParam>("VehicleModelParam");



    connect(mp_lcm_thread, SIGNAL(P2pLcmMsgReceived(P2pLcmData)), p2p_client, SLOT(UpdateP2pLcmMsg(P2pLcmData)));
    connect(mp_lcm_thread, SIGNAL(ApaPldResultReceived(ApaValidSlots)), p2p_client, SLOT(OnApaPldResult(ApaValidSlots)));
    connect(mp_lcm_thread, SIGNAL(ApaResultReceived(ApaLcmData)), apa_client, SLOT(OnApaResult(ApaLcmData)));
    connect(mp_lcm_thread, SIGNAL(ApaResultMsgReceived(AvmEvent_ApaResult)), apa_client, SLOT(OnApaResultMsg(AvmEvent_ApaResult)));

    connect(mp_lcm_thread, SIGNAL(VpldResultMsgReceived(VpldResult)), vpld_client_, SLOT(OnVpldResultMsg(VpldResult)));
    connect(mp_lcm_thread, SIGNAL(VpldBevImageMsgReceived(unsigned char*)), vpld_client_, SLOT(OnVpldBevImageMsg(unsigned char*)));
    connect(mp_lcm_thread, SIGNAL(VdrVehiclePoseReceived(VehicleDeadReckoning)), vdr_client_, SLOT(OnVdrVehiclePose(VehicleDeadReckoning)));
    
    connect(mp_lcm_thread, SIGNAL(VehicleModelParamReceived(VehicleModelParam)),vdr_client_,SLOT(OnVdrVehicleParam(VehicleModelParam)));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::PlotShift()
{
    mp_plot_window = new PlotWindow;
    connect(mp_lcm_thread, SIGNAL(P2pLcmMsgReceived(P2pLcmData)), mp_plot_window, SLOT(OnP2pLcmMsg(P2pLcmData)));
    mp_plot_window->show();
}

void MainWindow::PlotFreespace()
{
    mp_freespace_window = new FreespaceWindow;
    connect(mp_lcm_thread, SIGNAL(FreespaceResultMsgReceived(unsigned char*)), mp_freespace_window, SLOT(OnFreespaceResultMsg(unsigned char*)));

    // connect(mp_lcm_thread, SIGNAL(P2pLcmMsgReceived(P2pLcmData)), mp_plot_window, SLOT(OnP2pLcmMsg(P2pLcmData)));
    mp_freespace_window->show();
}

void MainWindow::PlotSonarCostMap()
{
    mp_sonar_cost_map_window = new SonarCostMapWindow;
    mp_sonar_cost_map_window->show();
}