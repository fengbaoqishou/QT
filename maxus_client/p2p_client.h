#ifndef P2P_CLIENT_H_
#define P2P_CLIENT_H_

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
#include "qcustomplot.h"
#include <lcm/lcm-cpp.hpp>

#include "p2p_lcm_data.h"
#include "slot_curve.h"

class P2pClient:public QWidget
{
    Q_OBJECT

public:
    explicit P2pClient(QWidget *parent = 0);
    ~P2pClient();
    QWidget* GetWidget() const;
    QHBoxLayout* GetLayout() const;
private slots:

    void UpdateP2pLcmMsg(P2pLcmData lcm_data);
    void OnApaPldResult(ApaValidSlots valid_slots);
    void OnDataButtonClicked();
    void OnPlotRoute1ButtonClicked();
    void OnPlotRoute2ButtonClicked();
    void OnCopyP2pDataButtonClicked();
    void OnSendP2pCmdButtonClicked();

    //p2p cmd push button onClicked slot
    void OnStartEnvironmentDetectionButtonClicked();
    void OnStartRouteLearningButtonClicked();
    void OnEndRouteLearningButtonClicked();
    void OnCancleRouteLearningButtonClicked();
    void OnSaveRoute1ButtonClicked();
    void OnSaveRoute3ButtonClicked();
    void OnStartMappingButtonClicked();
    void OnDeleteRoute1ButtonClicked();
    void OnDeleteRoute2ButtonClicked();
    void OnStopRouteDeleteButtonClicked();
    void OnMatchRoute1ButtonClicked();
    void OnMatchRoute2ButtonClicked();
    void OnStopRouteMatchButtonClicked();
    void OnLockPointAButtonClicked();
    void OnStartRouteReplayButtonClicked();
    void OnEndRouteReplayButtonClicked();
    void OnRelocatePointBButtonClicked();
    void OnFinalArrieveButtonClicked();
    void OnQuitP2PButtonClicked();
    void OnP2pReqPldButtonClicked();
    void OnP2ppVpCtrlButtonClicked();
    void OnP2pParkReqButtonClicked();

private:
    QCustomPlot *mPlot;
    QPlainTextEdit* plainTextEdit_log;
    QGroupBox* status_group_box_;
    //line edit
    QLineEdit* lineEdit_steering_angle;
    QLineEdit* lineEdit_vehicle_speed;
    QLineEdit* lineEdit_yaw_rate;
    QLineEdit* lineEdit_wheel_speed_fl;
    QLineEdit* lineEdit_wheel_speed_fr;
    QLineEdit* lineEdit_wheel_speed_rl;
    QLineEdit* lineEdit_wheel_speed_rr;
    QLineEdit* lineEdit_shift_pos;
    QLineEdit* lineEdit_door_sts_fl;
    QLineEdit* lineEdit_door_sts_fr;
    QLineEdit* lineEdit_door_sts_rl;
    QLineEdit* lineEdit_door_sts_rr;
    QLineEdit* lineEdit_left_turn_sig;
    QLineEdit* lineEdit_right_turn_sig;
    QLineEdit* lineEdit_brake_act_flag;
    QLineEdit* lineEdit_HighBmAct;
    QLineEdit* lineEdit_LowBmAct;
    QLineEdit* lineEdit_radar_rear_left_side_distance;
    QLineEdit* lineEdit_radar_rear_left_coner_distance;
    QLineEdit* lineEdit_radar_rear_left_middle_distance;
    QLineEdit* lineEdit_radar_rear_right_side_distance;
    QLineEdit* lineEdit_radar_rear_right_coner_distance;
    QLineEdit* lineEdit_radar_rear_right_middle_distance;
    QLineEdit* lineEdit_radar_front_left_side_distance;
    QLineEdit* lineEdit_radar_front_left_coner_distance;
    QLineEdit* lineEdit_radar_front_left_middle_distance;
    QLineEdit* lineEdit_radar_front_right_side_distance;
    QLineEdit* lineEdit_radar_front_right_coner_distance;
    QLineEdit* lineEdit_radar_front_right_middle_distance;
    QLineEdit* lineEdit_WRSLDWhlDistPlsCntr;
    QLineEdit* lineEdit_WRSRDWhlDistPlsCntr;
    QLineEdit* lineEdit_WRSLNDWhlDistPCntr;
    QLineEdit* lineEdit_WRSRNDWhlDistPCntr;
    QLineEdit* lineEdit_Electrical_Power_Steering_Availability_Status;
    QLineEdit* lineEdit_timestamp;
    QLineEdit* lineEdit_working_state;
    QLineEdit* lineEdit_system_state;
    QLineEdit* lineEdit_failure_state;
    QLineEdit* lineEdit_route_save_percentage;
    QLineEdit* lineEdit_route_learning_state;
    QLineEdit* lineEdit_route1_save_result;
    QLineEdit* lineEdit_route1_delete_result;
    QLineEdit* lineEdit_route2_save_result;
    QLineEdit* lineEdit_route2_delete_result;
    QLineEdit* lineEdit_radar_obs_state;
    QLineEdit* lineEdit_route_plan_state;
    QLineEdit* lineEdit_reloc_slam_state;
    QLineEdit* lineEdit_rout1_exist_state;
    QLineEdit* lineEdit_rout2_exist_state;
    QLineEdit* lineEdit_route_replay_state;
    QLineEdit* lineEdit_mod_x;
    QLineEdit* lineEdit_mod_y;
    QLineEdit* lineEdit_mod_dist;
    QLineEdit* lineEdit_a_point_available_result;
    QLineEdit* lineEdit_pos_valid;
    QLineEdit* lineEdit_pos_x;
    QLineEdit* lineEdit_pos_y;
    QLineEdit* lineEdit_pos_theta;

    QLineEdit* lineEdit_dist2path;
    QLineEdit* lineEdit_angle2path;
    QLineEdit* lineEdit_dist2pointB;
    QLineEdit* lineEdit_angle2pointB;
    QLineEdit* lineEdit_lengthAB;

    //QLabel
    QLabel* label_steering_angle;
    QLabel* label_vehicle_speed;
    QLabel* label_yaw_rate;
    QLabel* label_wheel_speed_fl;
    QLabel* label_wheel_speed_fr;
    QLabel* label_wheel_speed_rl;
    QLabel* label_wheel_speed_rr;
    QLabel* label_shift_pos;
    QLabel* label_door_sts_fl;
    QLabel* label_door_sts_fr;
    QLabel* label_door_sts_rl;
    QLabel* label_door_sts_rr;
    QLabel* label_left_turn_sig;
    QLabel* label_right_turn_sig;
    QLabel* label_brake_act_flag;
    QLabel* label_HighBmAct;
    QLabel* label_LowBmAct;
    QLabel* label_radar_rear_left_side_distance;
    QLabel* label_radar_rear_left_coner_distance;
    QLabel* label_radar_rear_left_middle_distance;
    QLabel* label_radar_rear_right_side_distance;
    QLabel* label_radar_rear_right_coner_distance;
    QLabel* label_radar_rear_right_middle_distance;
    QLabel* label_radar_front_left_side_distance;
    QLabel* label_radar_front_left_coner_distance;
    QLabel* label_radar_front_left_middle_distance;
    QLabel* label_radar_front_right_side_distance;
    QLabel* label_radar_front_right_coner_distance;
    QLabel* label_radar_front_right_middle_distance;
    QLabel* label_WRSLDWhlDistPlsCntr;
    QLabel* label_WRSRDWhlDistPlsCntr;
    QLabel* label_WRSLNDWhlDistPCntr;
    QLabel* label_WRSRNDWhlDistPCntr;
    QLabel* label_Electrical_Power_Steering_Availability_Status;
    QLabel* label_timestamp;
    QLabel* label_working_state;
    QLabel* label_system_state;
    QLabel* label_failure_state;
    QLabel* label_route_save_percentage;
    QLabel* label_route_learning_state;
    QLabel* label_route1_save_result;
    QLabel* label_route1_delete_result;
    QLabel* label_route2_save_result;
    QLabel* label_route2_delete_result;
    QLabel* label_radar_obs_state;
    QLabel* label_route_plan_state;
    QLabel* label_reloc_slam_state;
    QLabel* label_rout1_exist_state;
    QLabel* label_rout2_exist_state;
    QLabel* label_route_replay_state;
    QLabel* label_mod_x;
    QLabel* label_mod_y;
    QLabel* label_mod_dist;
    QLabel* label_a_point_available_result;
    QLabel* label_pos_valid;
    QLabel* label_pos_x;
    QLabel* label_pos_y;
    QLabel* label_pos_theta;
    QLabel* label_dist2path;
    QLabel* label_angle2path;
    QLabel* label_dist2pointB;
    QLabel* label_angle2pointB;
    QLabel* label_lengthAB;
        //QGroupBox
    QGroupBox* groupBox_vehicle_status;
    QGroupBox* groupBox_wheel_speed;
    QGroupBox* groupBox_wheel_pulse;
    QGroupBox* groupBox_p2p_status;
    QGroupBox* groupBox_ultrasonic;
    QGroupBox* groupBox_debug_info;
    //QPushButton
    QPushButton* pushButton_data_recorde;
    QPushButton* pushButton_plot_route_1;
    QPushButton* pushButton_plot_route_2;

    QPushButton* pushButton_copy_p2p_data;
    QPushButton* pushButton_send_p2p_cmd;
    QComboBox* comboBox_p2p_cmd_type;

    //p2p cmd pushbutton
    QPushButton* pushButton_StartEnvironmentDetection;
    QPushButton* pushButton_StartRouteLearning;
    QPushButton* pushButton_EndRouteLearning;
    QPushButton* pushButton_CancleRouteLearning;
    QPushButton* pushButton_SaveRoute1;
    QPushButton* pushButton_SaveRoute2;
    QPushButton* pushButton_StartMapping;
    QPushButton* pushButton_DeleteRoute1;
    QPushButton* pushButton_DeleteRoute2;
    QPushButton* pushButton_StopRouteDelete;
    QPushButton* pushButton_MatchRoute1;
    QPushButton* pushButton_MatchRoute2;
    QPushButton* pushButton_StopRouteMatch;
    QPushButton* pushButton_LockPointA;
    QPushButton* pushButton_StartRouteReplay;
    QPushButton* pushButton_EndRouteReplay;
    QPushButton* pushButton_RelocatePointB;
    QPushButton* pushButton_FinalArrieve;
    QPushButton* pushButton_QuitP2P;
    QPushButton* pushButton_P2pReqPld;
    QPushButton* pushButton_P2pVpCtrl;
    QPushButton* pushButton_P2pParkReq;

    QGroupBox*   groupBox_p2p_cmd;
    QGridLayout* layout_groupBox_p2p_cmd;

    
    QWidget *central_widget;
    //QLayout
    QHBoxLayout* layout_central_widget;
    QVBoxLayout* layout_left_central_widget;
    QVBoxLayout* layout_right_central_widget;
    QVBoxLayout* layout_middle_central_widget;
    QGridLayout* layout_groupBox_vehicle_status;
    QGridLayout* layout_groupBox_wheel_speed;
    QGridLayout* layout_groupBox_wheel_pulse;
    QGridLayout* layout_groupBox_p2p_status;
    QGridLayout* layout_groupBox_ultrasonic;
    QGridLayout* layout_groupBox_debug_info;

    lcm::LCM lcm_sender;

    //file
    std::ofstream log_file_;

    //path data
    PathData path_data_;

    QCPCurve* real_time_pose_curve;
    float vehicle_width;
    float vehicle_length;
    float rear_wheel_to_tail;
    QCPCurve* vehicle_curve;
    float vehicle_curve_point_x[10];
    float vehicle_curve_point_y[10];

    QCPCurve* route_1_curve;
    QCPCurve* route_2_curve;
    QCPCurve* target_slot_1_curve;
    QCPCurve* target_slot_2_curve;
    float vehicle_pose_x;
    float vehicle_pose_y;
    float vehicle_pose_theta;

    SlotCurve* mp_detected_slot_curves[24];
    QCPItemText* mp_detected_slot_desc[24];


    QWidget* p2p_client_widget;

    QHBoxLayout* layout_p2p_client;
    QVBoxLayout* layout_p2p_client_left;
    QVBoxLayout* layout_p2p_client_right;

    QGridLayout* layout_groupBox_input_info;
    QGridLayout* layout_groupBox_control_info;
    QGridLayout* layout_groupBox_output_info;

    QGroupBox* groupBox_input_info;
    QGroupBox* groupBox_control_info;
    QGroupBox* groupBox_output_info;
};




#endif