#include <iostream>
#include "p2p_client.h"


P2pClient::P2pClient(QWidget* parent):QWidget(parent)
{
    mPlot = new QCustomPlot(parent);
    mPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    mPlot->setMinimumSize(640,480);
    mPlot->xAxis->setRange(-25,25);
    mPlot->yAxis->setRange(-25,25);
    mPlot->yAxis->setScaleRatio(mPlot->xAxis, 1.0);

    lcm_sender = lcm_create("udpm://239.255.76.67:7667?ttl=100");
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender create failed!"<<std::endl;
    }
    else
    {
        std::cout<<"lcm sender create ok!"<<std::endl;
    }
    real_time_pose_curve = new QCPCurve(mPlot->xAxis, mPlot->yAxis);
    real_time_pose_curve->setPen(QPen(Qt::green));
    real_time_pose_curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc ));

    route_1_curve = new QCPCurve(mPlot->xAxis, mPlot->yAxis);
    route_1_curve->setPen(QPen(QColor(255, 120, 0)));
    route_1_curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar ));

    route_2_curve = new QCPCurve(mPlot->xAxis, mPlot->yAxis);
    route_2_curve->setPen(QPen(QColor(255, 120, 0)));
    route_2_curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar ));


    target_slot_1_curve = new QCPCurve(mPlot->xAxis, mPlot->yAxis);
    target_slot_1_curve->setPen(QPen(QColor(255, 120, 0)));
    target_slot_1_curve->setBrush(QBrush(QColor(255, 120, 0, 50)));
    target_slot_1_curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusSquare ));

    target_slot_2_curve = new QCPCurve(mPlot->xAxis, mPlot->yAxis);
    target_slot_2_curve->setPen(QPen(QColor(255, 120, 0)));
    target_slot_2_curve->setBrush(QBrush(QColor(255, 120, 0, 50)));
    target_slot_2_curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusSquare ));


    vehicle_curve = new QCPCurve(mPlot->xAxis, mPlot->yAxis);
    vehicle_curve->setPen(QPen(Qt::darkRed));
    vehicle_curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc ));
    //slot detection curves
    for(int i=0; i<24; i++)
    {
        mp_detected_slot_curves[i] = new SlotCurve(mPlot);
        mp_detected_slot_desc[i] = new QCPItemText(mPlot);
        mp_detected_slot_desc[i]->setVisible(false);

        //mp_detected_slot_desc[i]->setFont(QFont(font().family(), 16)); // make font a bit larger
        mp_detected_slot_desc[i]->setPen(QPen(Qt::black)); // show black border around text
    }

    vehicle_pose_x = 0.0f;
    vehicle_pose_y = 0.0f;
    vehicle_pose_theta = 0.0f;

    // p2p info output
    plainTextEdit_log = new QPlainTextEdit;
    plainTextEdit_log->setReadOnly(true);
    plainTextEdit_log->setMaximumHeight(100);

    vehicle_width = 1.919;
    vehicle_length = 4.760;
    rear_wheel_to_tail = 0.858;
    vehicle_curve_point_x[0] = vehicle_length - rear_wheel_to_tail;
    vehicle_curve_point_x[1] = -rear_wheel_to_tail;
    vehicle_curve_point_x[2] = -rear_wheel_to_tail;
    vehicle_curve_point_x[3] = vehicle_length - rear_wheel_to_tail;
    vehicle_curve_point_x[4] = vehicle_length - rear_wheel_to_tail;
    vehicle_curve_point_x[5] = 0;
    vehicle_curve_point_x[6] = 0;
    vehicle_curve_point_x[7] = vehicle_length - rear_wheel_to_tail;
    vehicle_curve_point_x[8] = vehicle_length - rear_wheel_to_tail;
    vehicle_curve_point_x[9] = -rear_wheel_to_tail;

    vehicle_curve_point_y[0] = -vehicle_width/2;
    vehicle_curve_point_y[1] = -vehicle_width/2;
    vehicle_curve_point_y[2] = vehicle_width/2;
    vehicle_curve_point_y[3] = vehicle_width/2;
    vehicle_curve_point_y[4] = -vehicle_width/2;
    vehicle_curve_point_y[5] = -vehicle_width/2;
    vehicle_curve_point_y[6] = vehicle_width/2;
    vehicle_curve_point_y[7] = vehicle_width/2;
    vehicle_curve_point_y[8] = 0;
    vehicle_curve_point_y[9] = 0;

    lineEdit_steering_angle = new QLineEdit("0.0");
    lineEdit_steering_angle->setReadOnly(true);
    lineEdit_steering_angle->setAlignment(Qt::AlignRight);
    lineEdit_steering_angle->setMaxLength(15);
    lineEdit_steering_angle->setMaximumWidth(100);

    lineEdit_vehicle_speed = new QLineEdit("0.0");
    lineEdit_vehicle_speed->setReadOnly(true);
    lineEdit_vehicle_speed->setAlignment(Qt::AlignRight);
    lineEdit_vehicle_speed->setMaxLength(15);
    lineEdit_vehicle_speed->setMaximumWidth(100);

    lineEdit_yaw_rate = new QLineEdit("0.0");
    lineEdit_yaw_rate->setReadOnly(true);
    lineEdit_yaw_rate->setAlignment(Qt::AlignRight);
    lineEdit_yaw_rate->setMaxLength(15);
    lineEdit_yaw_rate->setMaximumWidth(100);

    lineEdit_wheel_speed_fl = new QLineEdit("0.0");
    lineEdit_wheel_speed_fl->setReadOnly(true);
    lineEdit_wheel_speed_fl->setAlignment(Qt::AlignRight);
    lineEdit_wheel_speed_fl->setMaxLength(15);
    lineEdit_wheel_speed_fl->setMaximumWidth(100);

    lineEdit_wheel_speed_fr = new QLineEdit("0.0");
    lineEdit_wheel_speed_fr->setReadOnly(true);
    lineEdit_wheel_speed_fr->setAlignment(Qt::AlignRight);
    lineEdit_wheel_speed_fr->setMaxLength(15);
    lineEdit_wheel_speed_fr->setMaximumWidth(100);

    lineEdit_wheel_speed_rl = new QLineEdit("0.0");
    lineEdit_wheel_speed_rl->setReadOnly(true);
    lineEdit_wheel_speed_rl->setAlignment(Qt::AlignRight);
    lineEdit_wheel_speed_rl->setMaxLength(15);
    lineEdit_wheel_speed_rl->setMaximumWidth(100);

    lineEdit_wheel_speed_rr = new QLineEdit("0.0");
    lineEdit_wheel_speed_rr->setReadOnly(true);
    lineEdit_wheel_speed_rr->setAlignment(Qt::AlignRight);
    lineEdit_wheel_speed_rr->setMaxLength(15);
    lineEdit_wheel_speed_rr->setMaximumWidth(100);

    lineEdit_shift_pos = new QLineEdit("0");
    lineEdit_shift_pos->setReadOnly(true);
    lineEdit_shift_pos->setAlignment(Qt::AlignRight);
    lineEdit_shift_pos->setMaxLength(15);
    lineEdit_shift_pos->setMaximumWidth(100);
    lineEdit_door_sts_fl = new QLineEdit("0");
    lineEdit_door_sts_fl->setReadOnly(true);
    lineEdit_door_sts_fl->setAlignment(Qt::AlignRight);
    lineEdit_door_sts_fl->setMaxLength(15);
    lineEdit_door_sts_fl->setMaximumWidth(100);

    lineEdit_door_sts_fr = new QLineEdit("0");
    lineEdit_door_sts_fr->setReadOnly(true);
    lineEdit_door_sts_fr->setAlignment(Qt::AlignRight);
    lineEdit_door_sts_fr->setMaxLength(15);
    lineEdit_door_sts_fr->setMaximumWidth(100);

    lineEdit_door_sts_rl = new QLineEdit("0");
    lineEdit_door_sts_rl->setReadOnly(true);
    lineEdit_door_sts_rl->setAlignment(Qt::AlignRight);
    lineEdit_door_sts_rl->setMaxLength(15);
    lineEdit_door_sts_rl->setMaximumWidth(100);

    lineEdit_door_sts_rr = new QLineEdit("0");
    lineEdit_door_sts_rr->setReadOnly(true);
    lineEdit_door_sts_rr->setAlignment(Qt::AlignRight);
    lineEdit_door_sts_rr->setMaxLength(15);
    lineEdit_door_sts_rr->setMaximumWidth(100);

    lineEdit_left_turn_sig = new QLineEdit("0");
    lineEdit_left_turn_sig->setReadOnly(true);
    lineEdit_left_turn_sig->setAlignment(Qt::AlignRight);
    lineEdit_left_turn_sig->setMaxLength(15);
    lineEdit_left_turn_sig->setMaximumWidth(100);

    lineEdit_right_turn_sig = new QLineEdit("0");
    lineEdit_right_turn_sig->setReadOnly(true);
    lineEdit_right_turn_sig->setAlignment(Qt::AlignRight);
    lineEdit_right_turn_sig->setMaxLength(15);
    lineEdit_right_turn_sig->setMaximumWidth(100);

    lineEdit_brake_act_flag = new QLineEdit("0");
    lineEdit_brake_act_flag->setReadOnly(true);
    lineEdit_brake_act_flag->setAlignment(Qt::AlignRight);
    lineEdit_brake_act_flag->setMaxLength(15);
    lineEdit_brake_act_flag->setMaximumWidth(100);

    lineEdit_HighBmAct = new QLineEdit("0");
    lineEdit_HighBmAct->setReadOnly(true);
    lineEdit_HighBmAct->setAlignment(Qt::AlignRight);
    lineEdit_HighBmAct->setMaxLength(15);
    lineEdit_HighBmAct->setMaximumWidth(100);

    lineEdit_LowBmAct = new QLineEdit("0");
    lineEdit_LowBmAct->setReadOnly(true);
    lineEdit_LowBmAct->setAlignment(Qt::AlignRight);
    lineEdit_LowBmAct->setMaxLength(15);
    lineEdit_LowBmAct->setMaximumWidth(100);

    lineEdit_radar_rear_left_side_distance = new QLineEdit("0.0");
    lineEdit_radar_rear_left_side_distance->setReadOnly(true);
    lineEdit_radar_rear_left_side_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_rear_left_side_distance->setMaxLength(15);
    lineEdit_radar_rear_left_side_distance->setMaximumWidth(100);

    lineEdit_radar_rear_left_coner_distance = new QLineEdit("0.0");
    lineEdit_radar_rear_left_coner_distance->setReadOnly(true);
    lineEdit_radar_rear_left_coner_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_rear_left_coner_distance->setMaxLength(15);
    lineEdit_radar_rear_left_coner_distance->setMaximumWidth(100);

    lineEdit_radar_rear_left_middle_distance = new QLineEdit("0.0");
    lineEdit_radar_rear_left_middle_distance->setReadOnly(true);
    lineEdit_radar_rear_left_middle_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_rear_left_middle_distance->setMaxLength(15);
    lineEdit_radar_rear_left_middle_distance->setMaximumWidth(100);

    lineEdit_radar_rear_right_side_distance = new QLineEdit("0.0");
    lineEdit_radar_rear_right_side_distance->setReadOnly(true);
    lineEdit_radar_rear_right_side_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_rear_right_side_distance->setMaxLength(15);
    lineEdit_radar_rear_right_side_distance->setMaximumWidth(100);

    lineEdit_radar_rear_right_coner_distance = new QLineEdit("0.0");
    lineEdit_radar_rear_right_coner_distance->setReadOnly(true);
    lineEdit_radar_rear_right_coner_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_rear_right_coner_distance->setMaxLength(15);
    lineEdit_radar_rear_right_coner_distance->setMaximumWidth(100);

    lineEdit_radar_rear_right_middle_distance = new QLineEdit("0.0");
    lineEdit_radar_rear_right_middle_distance->setReadOnly(true);
    lineEdit_radar_rear_right_middle_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_rear_right_middle_distance->setMaxLength(15);
    lineEdit_radar_rear_right_middle_distance->setMaximumWidth(100);

    lineEdit_radar_front_left_side_distance = new QLineEdit("0.0");
    lineEdit_radar_front_left_side_distance->setReadOnly(true);
    lineEdit_radar_front_left_side_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_front_left_side_distance->setMaxLength(15);
    lineEdit_radar_front_left_side_distance->setMaximumWidth(100);

    lineEdit_radar_front_left_coner_distance = new QLineEdit("0.0");
    lineEdit_radar_front_left_coner_distance->setReadOnly(true);
    lineEdit_radar_front_left_coner_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_front_left_coner_distance->setMaxLength(15);
    lineEdit_radar_front_left_coner_distance->setMaximumWidth(100);

    lineEdit_radar_front_left_middle_distance = new QLineEdit("0.0");
    lineEdit_radar_front_left_middle_distance->setReadOnly(true);
    lineEdit_radar_front_left_middle_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_front_left_middle_distance->setMaxLength(15);
    lineEdit_radar_front_left_middle_distance->setMaximumWidth(100);

    lineEdit_radar_front_right_side_distance = new QLineEdit("0.0");
    lineEdit_radar_front_right_side_distance->setReadOnly(true);
    lineEdit_radar_front_right_side_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_front_right_side_distance->setMaxLength(15);
    lineEdit_radar_front_right_side_distance->setMaximumWidth(100);

    lineEdit_radar_front_right_coner_distance = new QLineEdit("0.0");
    lineEdit_radar_front_right_coner_distance->setReadOnly(true);
    lineEdit_radar_front_right_coner_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_front_right_coner_distance->setMaxLength(15);
    lineEdit_radar_front_right_coner_distance->setMaximumWidth(100);

    lineEdit_radar_front_right_middle_distance = new QLineEdit("0.0");
    lineEdit_radar_front_right_middle_distance->setReadOnly(true);
    lineEdit_radar_front_right_middle_distance->setAlignment(Qt::AlignRight);
    lineEdit_radar_front_right_middle_distance->setMaxLength(15);
    lineEdit_radar_front_right_middle_distance->setMaximumWidth(100);
    
    lineEdit_WRSLDWhlDistPlsCntr = new QLineEdit("0.0");
    lineEdit_WRSLDWhlDistPlsCntr->setReadOnly(true);
    lineEdit_WRSLDWhlDistPlsCntr->setAlignment(Qt::AlignRight);
    lineEdit_WRSLDWhlDistPlsCntr->setMaxLength(15);
    lineEdit_WRSLDWhlDistPlsCntr->setMaximumWidth(100);

    lineEdit_WRSRDWhlDistPlsCntr = new QLineEdit("0.0");
    lineEdit_WRSRDWhlDistPlsCntr->setReadOnly(true);
    lineEdit_WRSRDWhlDistPlsCntr->setAlignment(Qt::AlignRight);
    lineEdit_WRSRDWhlDistPlsCntr->setMaxLength(15);
    lineEdit_WRSRDWhlDistPlsCntr->setMaximumWidth(100);

    lineEdit_WRSLNDWhlDistPCntr = new QLineEdit("0.0");
    lineEdit_WRSLNDWhlDistPCntr->setReadOnly(true);
    lineEdit_WRSLNDWhlDistPCntr->setAlignment(Qt::AlignRight);
    lineEdit_WRSLNDWhlDistPCntr->setMaxLength(15);
    lineEdit_WRSLNDWhlDistPCntr->setMaximumWidth(100);

    lineEdit_WRSRNDWhlDistPCntr = new QLineEdit("0.0");
    lineEdit_WRSRNDWhlDistPCntr->setReadOnly(true);
    lineEdit_WRSRNDWhlDistPCntr->setAlignment(Qt::AlignRight);
    lineEdit_WRSRNDWhlDistPCntr->setMaxLength(15);
    lineEdit_WRSRNDWhlDistPCntr->setMaximumWidth(100);

    lineEdit_Electrical_Power_Steering_Availability_Status = new QLineEdit("0.0");
    lineEdit_Electrical_Power_Steering_Availability_Status->setReadOnly(true);
    lineEdit_Electrical_Power_Steering_Availability_Status->setAlignment(Qt::AlignRight);
    lineEdit_Electrical_Power_Steering_Availability_Status->setMaxLength(15);
    lineEdit_Electrical_Power_Steering_Availability_Status->setMaximumWidth(100);

    lineEdit_timestamp = new QLineEdit("0.0");
    lineEdit_timestamp->setReadOnly(true);
    lineEdit_timestamp->setAlignment(Qt::AlignRight);
    lineEdit_timestamp->setMaxLength(15);
    lineEdit_timestamp->setMaximumWidth(100);

    lineEdit_working_state = new QLineEdit("0.0");
    lineEdit_working_state->setReadOnly(true);
    lineEdit_working_state->setAlignment(Qt::AlignRight);
    lineEdit_working_state->setMaxLength(15);
    lineEdit_working_state->setMaximumWidth(100);

    lineEdit_system_state = new QLineEdit("0.0");
    lineEdit_system_state->setReadOnly(true);
    lineEdit_system_state->setAlignment(Qt::AlignRight);
    lineEdit_system_state->setMaxLength(15);
    lineEdit_system_state->setMaximumWidth(100);

    lineEdit_failure_state = new QLineEdit("0.0");
    lineEdit_failure_state->setReadOnly(true);
    lineEdit_failure_state->setAlignment(Qt::AlignRight);
    lineEdit_failure_state->setMaxLength(15);
    lineEdit_failure_state->setMaximumWidth(100);

    lineEdit_route_save_percentage = new QLineEdit("0.0");
    lineEdit_route_save_percentage->setReadOnly(true);
    lineEdit_route_save_percentage->setAlignment(Qt::AlignRight);
    lineEdit_route_save_percentage->setMaxLength(15);
    lineEdit_route_save_percentage->setMaximumWidth(100);

    lineEdit_route_learning_state = new QLineEdit("0.0");
    lineEdit_route_learning_state->setReadOnly(true);
    lineEdit_route_learning_state->setAlignment(Qt::AlignRight);
    lineEdit_route_learning_state->setMaxLength(15);
    lineEdit_route_learning_state->setMaximumWidth(100);

    lineEdit_route1_save_result = new QLineEdit("0.0");
    lineEdit_route1_save_result->setReadOnly(true);
    lineEdit_route1_save_result->setAlignment(Qt::AlignRight);
    lineEdit_route1_save_result->setMaxLength(15);
    lineEdit_route1_save_result->setMaximumWidth(100);

    lineEdit_route1_delete_result = new QLineEdit("0.0");
    lineEdit_route1_delete_result->setReadOnly(true);
    lineEdit_route1_delete_result->setAlignment(Qt::AlignRight);
    lineEdit_route1_delete_result->setMaxLength(15);
    lineEdit_route1_delete_result->setMaximumWidth(100);


    lineEdit_route2_save_result = new QLineEdit("0.0");
    lineEdit_route2_save_result->setReadOnly(true);
    lineEdit_route2_save_result->setAlignment(Qt::AlignRight);
    lineEdit_route2_save_result->setMaxLength(15);
    lineEdit_route2_save_result->setMaximumWidth(100);

    lineEdit_route2_delete_result = new QLineEdit("0.0");
    lineEdit_route2_delete_result->setReadOnly(true);
    lineEdit_route2_delete_result->setAlignment(Qt::AlignRight);
    lineEdit_route2_delete_result->setMaxLength(15);
    lineEdit_route2_delete_result->setMaximumWidth(100);


    lineEdit_radar_obs_state = new QLineEdit("0.0");
    lineEdit_radar_obs_state->setReadOnly(true);
    lineEdit_radar_obs_state->setAlignment(Qt::AlignRight);
    lineEdit_radar_obs_state->setMaxLength(15);
    lineEdit_radar_obs_state->setMaximumWidth(100);

    lineEdit_route_plan_state = new QLineEdit("0.0");
    lineEdit_route_plan_state->setReadOnly(true);
    lineEdit_route_plan_state->setAlignment(Qt::AlignRight);
    lineEdit_route_plan_state->setMaxLength(15);
    lineEdit_route_plan_state->setMaximumWidth(100);

    lineEdit_reloc_slam_state = new QLineEdit("0.0");
    lineEdit_reloc_slam_state->setReadOnly(true);
    lineEdit_reloc_slam_state->setAlignment(Qt::AlignRight);
    lineEdit_reloc_slam_state->setMaxLength(15);
    lineEdit_reloc_slam_state->setMaximumWidth(100);

    lineEdit_rout1_exist_state = new QLineEdit("0.0");
    lineEdit_rout1_exist_state->setReadOnly(true);
    lineEdit_rout1_exist_state->setAlignment(Qt::AlignRight);
    lineEdit_rout1_exist_state->setMaxLength(15);
    lineEdit_rout1_exist_state->setMaximumWidth(100);

    lineEdit_rout2_exist_state = new QLineEdit("0.0");
    lineEdit_rout2_exist_state->setReadOnly(true);
    lineEdit_rout2_exist_state->setAlignment(Qt::AlignRight);
    lineEdit_rout2_exist_state->setMaxLength(15);
    lineEdit_rout2_exist_state->setMaximumWidth(100);


    lineEdit_route_replay_state = new QLineEdit("0.0");
    lineEdit_route_replay_state->setReadOnly(true);
    lineEdit_route_replay_state->setAlignment(Qt::AlignRight);
    lineEdit_route_replay_state->setMaxLength(15);
    lineEdit_route_replay_state->setMaximumWidth(100);
    
    lineEdit_mod_x = new QLineEdit("0.0");
    lineEdit_mod_x->setReadOnly(true);
    lineEdit_mod_x->setAlignment(Qt::AlignRight);
    lineEdit_mod_x->setMaxLength(15);
    lineEdit_mod_x->setMaximumWidth(100);


    lineEdit_mod_y = new QLineEdit("0.0");
    lineEdit_mod_y->setReadOnly(true);
    lineEdit_mod_y->setAlignment(Qt::AlignRight);
    lineEdit_mod_y->setMaxLength(15);
    lineEdit_mod_y->setMaximumWidth(100);


    lineEdit_mod_dist = new QLineEdit("0.0");
    lineEdit_mod_dist->setReadOnly(true);
    lineEdit_mod_dist->setAlignment(Qt::AlignRight);
    lineEdit_mod_dist->setMaxLength(15);
    lineEdit_mod_dist->setMaximumWidth(100);

    lineEdit_a_point_available_result = new QLineEdit("0.0");
    lineEdit_a_point_available_result->setReadOnly(true);
    lineEdit_a_point_available_result->setAlignment(Qt::AlignRight);
    lineEdit_a_point_available_result->setMaxLength(15);
    lineEdit_a_point_available_result->setMaximumWidth(100);

    lineEdit_pos_valid = new QLineEdit("0.0");
    lineEdit_pos_valid->setReadOnly(true);
    lineEdit_pos_valid->setAlignment(Qt::AlignRight);
    lineEdit_pos_valid->setMaxLength(15);
    lineEdit_pos_valid->setMaximumWidth(100);

    lineEdit_pos_x = new QLineEdit("0.0");
    lineEdit_pos_x->setReadOnly(true);
    lineEdit_pos_x->setAlignment(Qt::AlignRight);
    lineEdit_pos_x->setMaxLength(15);
    lineEdit_pos_x->setMaximumWidth(100);

    lineEdit_pos_y = new QLineEdit("0.0");
    lineEdit_pos_y->setReadOnly(true);
    lineEdit_pos_y->setAlignment(Qt::AlignRight);
    lineEdit_pos_y->setMaxLength(15);
    lineEdit_pos_y->setMaximumWidth(100);

    lineEdit_pos_theta = new QLineEdit("0.0");
    lineEdit_pos_theta->setReadOnly(true);
    lineEdit_pos_theta->setAlignment(Qt::AlignRight);
    lineEdit_pos_theta->setMaxLength(15);
    lineEdit_pos_theta->setMaximumWidth(100);

    lineEdit_dist2path = new QLineEdit("0.0");
    lineEdit_dist2path->setReadOnly(true);
    lineEdit_dist2path->setAlignment(Qt::AlignRight);
    lineEdit_dist2path->setMaxLength(15);
    lineEdit_dist2path->setMaximumWidth(100);


    lineEdit_angle2path = new QLineEdit("0.0");
    lineEdit_angle2path->setReadOnly(true);
    lineEdit_angle2path->setAlignment(Qt::AlignRight);
    lineEdit_angle2path->setMaxLength(15);
    lineEdit_angle2path->setMaximumWidth(100);

    lineEdit_dist2pointB = new QLineEdit("0.0");
    lineEdit_dist2pointB->setReadOnly(true);
    lineEdit_dist2pointB->setAlignment(Qt::AlignRight);
    lineEdit_dist2pointB->setMaxLength(15);
    lineEdit_dist2pointB->setMaximumWidth(100);

    lineEdit_angle2pointB = new QLineEdit("0.0");
    lineEdit_angle2pointB->setReadOnly(true);
    lineEdit_angle2pointB->setAlignment(Qt::AlignRight);
    lineEdit_angle2pointB->setMaxLength(15);
    lineEdit_angle2pointB->setMaximumWidth(100);

    lineEdit_lengthAB = new QLineEdit("0.0");
    lineEdit_lengthAB->setReadOnly(true);
    lineEdit_lengthAB->setAlignment(Qt::AlignRight);
    lineEdit_lengthAB->setMaxLength(15);
    lineEdit_lengthAB->setMaximumWidth(100);

    // QLabel
    label_steering_angle = new QLabel("Steer(deg)");
    label_steering_angle->setBuddy(lineEdit_steering_angle);

    label_yaw_rate = new QLabel("YawRate(deg/s)");
    label_yaw_rate->setBuddy(lineEdit_yaw_rate);

    label_vehicle_speed = new QLabel("Speed(km/h)");
    label_vehicle_speed->setBuddy(lineEdit_vehicle_speed);

    label_wheel_speed_fl = new QLabel("FL(km/h)");
    label_wheel_speed_fl->setBuddy(lineEdit_wheel_speed_fl);
    label_wheel_speed_fr = new QLabel("FR(km/h)");
    label_wheel_speed_fr->setBuddy(lineEdit_wheel_speed_fr);
    label_wheel_speed_rl = new QLabel("RL(km/h)");
    label_wheel_speed_rl->setBuddy(lineEdit_wheel_speed_rl);
    label_wheel_speed_rr = new QLabel("RR(km/h)");
    label_wheel_speed_rr->setBuddy(lineEdit_wheel_speed_rr);

    label_shift_pos = new QLabel("Shift");
    label_shift_pos->setBuddy(lineEdit_shift_pos);

    label_door_sts_fl = new QLabel("FL");
    label_door_sts_fl->setBuddy(lineEdit_door_sts_fl);
    label_door_sts_fr = new QLabel("FR");
    label_door_sts_fr->setBuddy(lineEdit_door_sts_fr);
    label_door_sts_rl = new QLabel("RL");
    label_door_sts_rl->setBuddy(lineEdit_door_sts_rl);
    label_door_sts_rr = new QLabel("RR");
    label_door_sts_rr->setBuddy(lineEdit_door_sts_rr);

    label_left_turn_sig = new QLabel("LeftTurn");
    label_left_turn_sig->setBuddy(lineEdit_left_turn_sig);
    label_right_turn_sig = new QLabel("RightTurn");
    label_right_turn_sig->setBuddy(lineEdit_right_turn_sig);
    label_brake_act_flag = new QLabel("Brake");
    label_brake_act_flag->setBuddy(lineEdit_brake_act_flag);
    label_HighBmAct = new QLabel("HighBm");
    label_HighBmAct->setBuddy(lineEdit_HighBmAct);
    label_LowBmAct = new QLabel("LowBm");
    label_LowBmAct->setBuddy(lineEdit_LowBmAct);

    label_WRSLDWhlDistPlsCntr = new QLabel("FL(cont)");
    label_WRSLDWhlDistPlsCntr->setBuddy(lineEdit_WRSLDWhlDistPlsCntr);
    label_WRSRDWhlDistPlsCntr = new QLabel("FR(cont)");
    label_WRSRDWhlDistPlsCntr->setBuddy(lineEdit_WRSRDWhlDistPlsCntr);
    label_WRSLNDWhlDistPCntr = new QLabel("RL(cont)");
    label_WRSLNDWhlDistPCntr->setBuddy(lineEdit_WRSLNDWhlDistPCntr);
    label_WRSRNDWhlDistPCntr = new QLabel("RR(cont)");
    label_WRSRNDWhlDistPCntr->setBuddy(lineEdit_WRSRNDWhlDistPCntr);

    label_radar_rear_left_side_distance = new QLabel("RLS(cm)");
    label_radar_rear_left_side_distance->setBuddy(lineEdit_WRSLDWhlDistPlsCntr);
    label_radar_rear_left_coner_distance = new QLabel("RLC(cm)");
    label_radar_rear_left_coner_distance->setBuddy(lineEdit_WRSRDWhlDistPlsCntr);
    label_radar_rear_left_middle_distance = new QLabel("RLM(cm)");
    label_radar_rear_left_middle_distance->setBuddy(lineEdit_WRSLNDWhlDistPCntr);

    label_radar_rear_right_side_distance = new QLabel("RRS(cm)");
    label_radar_rear_right_side_distance->setBuddy(lineEdit_WRSRNDWhlDistPCntr);
    label_radar_rear_right_coner_distance = new QLabel("RRC(cm)");
    label_radar_rear_right_coner_distance->setBuddy(lineEdit_WRSLDWhlDistPlsCntr);
    label_radar_rear_right_middle_distance = new QLabel("RRM(cm)");
    label_radar_rear_right_middle_distance->setBuddy(lineEdit_WRSRDWhlDistPlsCntr);
    label_radar_front_left_side_distance = new QLabel("FLS(cm)");
    label_radar_front_left_side_distance->setBuddy(lineEdit_WRSLNDWhlDistPCntr);
    label_radar_front_left_coner_distance = new QLabel("FLC(cm)");
    label_radar_front_left_coner_distance->setBuddy(lineEdit_WRSRNDWhlDistPCntr);
    label_radar_front_left_middle_distance = new QLabel("FLM(cm)");
    label_radar_front_left_middle_distance->setBuddy(lineEdit_WRSLDWhlDistPlsCntr);
    label_radar_front_right_side_distance = new QLabel("FRS(cm)");
    label_radar_front_right_side_distance->setBuddy(lineEdit_WRSRDWhlDistPlsCntr);
    label_radar_front_right_coner_distance = new QLabel("FRC(cm)");
    label_radar_front_right_coner_distance->setBuddy(lineEdit_WRSLNDWhlDistPCntr);
    label_radar_front_right_middle_distance = new QLabel("FRM(cm)");
    label_radar_front_right_middle_distance->setBuddy(lineEdit_WRSRNDWhlDistPCntr);

    label_Electrical_Power_Steering_Availability_Status = new QLabel("EPS");
    label_Electrical_Power_Steering_Availability_Status->setBuddy(lineEdit_Electrical_Power_Steering_Availability_Status);
    label_timestamp = new QLabel("Timestamp(us)");
    label_timestamp->setBuddy(lineEdit_timestamp);

    label_working_state = new QLabel("WorkState");
    label_working_state->setBuddy(lineEdit_working_state);
    label_system_state = new QLabel("SystemState");
    label_system_state->setBuddy(lineEdit_system_state);
    label_failure_state = new QLabel("FailureState");
    label_failure_state->setBuddy(lineEdit_failure_state);
    label_route_save_percentage = new QLabel("SavePercentage");
    label_route_save_percentage->setBuddy(lineEdit_route_save_percentage);
    label_route_learning_state = new QLabel("LearnState");
    label_route_learning_state->setBuddy(lineEdit_route_learning_state);
    label_route1_save_result = new QLabel("SaveRst1");
    label_route1_save_result->setBuddy(lineEdit_route1_save_result);
    label_route1_delete_result = new QLabel("DeleteRst1");
    label_route1_delete_result->setBuddy(lineEdit_route1_delete_result);
    label_route2_save_result = new QLabel("SaveRst2");
    label_route2_save_result->setBuddy(lineEdit_route2_save_result);
    label_route2_delete_result = new QLabel("DeleteRst2");
    label_route2_delete_result->setBuddy(lineEdit_route2_delete_result);
    label_radar_obs_state = new QLabel("ObsState");
    label_radar_obs_state->setBuddy(lineEdit_radar_obs_state);
    label_route_plan_state = new QLabel("RoutePlan");
    label_route_plan_state->setBuddy(lineEdit_route_plan_state);

    label_reloc_slam_state = new QLabel("RelocState");
    label_reloc_slam_state->setBuddy(lineEdit_reloc_slam_state);
    label_rout1_exist_state = new QLabel("Route1State");
    label_rout1_exist_state->setBuddy(lineEdit_rout1_exist_state);
    label_rout2_exist_state = new QLabel("Route2State");
    label_rout2_exist_state->setBuddy(lineEdit_rout2_exist_state);
    label_route_replay_state = new QLabel("ReplayState");
    label_route_replay_state->setBuddy(lineEdit_route_replay_state);
    label_mod_x = new QLabel("ModX");
    label_mod_x->setBuddy(lineEdit_mod_x);
    label_mod_y = new QLabel("ModY");
    label_mod_y->setBuddy(lineEdit_mod_y);
    label_mod_dist = new QLabel("ModD");
    label_mod_dist->setBuddy(lineEdit_mod_dist);
    label_a_point_available_result = new QLabel("APoint");
    label_a_point_available_result->setBuddy(lineEdit_a_point_available_result);
    label_pos_valid = new QLabel("PoseValid");
    label_pos_valid->setBuddy(lineEdit_pos_valid);
    label_pos_x = new QLabel("PoseX");
    label_pos_x->setBuddy(lineEdit_pos_x);
    label_pos_y = new QLabel("PoseY");
    label_pos_y->setBuddy(lineEdit_pos_y);
    label_pos_theta = new QLabel("PoseTheta");
    label_pos_theta->setBuddy(lineEdit_pos_theta);

    label_dist2path = new QLabel("Dist2Path");
    label_dist2path->setBuddy(lineEdit_dist2path);

    label_angle2path = new QLabel("Angle2Path");
    label_angle2path->setBuddy(lineEdit_angle2path);

    label_dist2pointB = new QLabel("Dist2PointB");
    label_dist2pointB->setBuddy(lineEdit_dist2pointB);

    label_angle2pointB = new QLabel("Angle2PointB");
    label_angle2pointB->setBuddy(lineEdit_angle2pointB);

    label_lengthAB = new QLabel("LengthAB");
    label_lengthAB->setBuddy(lineEdit_lengthAB);

    // QPushButton
    pushButton_data_recorde = new QPushButton("StartLogP2pLcmData");
    pushButton_data_recorde->setCheckable(true);
    pushButton_data_recorde->setChecked(false);

    pushButton_plot_route_1 = new QPushButton("PlotRoute1");
    pushButton_plot_route_2 = new QPushButton("PlotRoute2");

    pushButton_copy_p2p_data = new QPushButton("CopyP2pData");
    pushButton_send_p2p_cmd = new QPushButton("SendP2pCmd");

    //p2p cmd
    pushButton_StartEnvironmentDetection = new QPushButton(QWidget::tr("EnvDetect"));
    pushButton_StartRouteLearning = new QPushButton(QWidget::tr("StartLearning"));
    pushButton_EndRouteLearning = new QPushButton(QWidget::tr("EndLearning"));
    pushButton_CancleRouteLearning = new QPushButton(QWidget::tr("CancleLearning"));
    pushButton_SaveRoute1 = new QPushButton(QWidget::tr("SaveRoute1"));
    pushButton_SaveRoute2 = new QPushButton(QWidget::tr("SaveRoute2"));
    pushButton_StartMapping = new QPushButton(QWidget::tr("StartMapping"));
    pushButton_DeleteRoute1 = new QPushButton(QWidget::tr("DeleteRoute1"));
    pushButton_DeleteRoute2 = new QPushButton(QWidget::tr("DeleteRoute2"));
    pushButton_StopRouteDelete = new QPushButton(QWidget::tr("StopDelete"));
    pushButton_MatchRoute1 = new QPushButton(QWidget::tr("MatchRoute1"));
    pushButton_MatchRoute2 = new QPushButton(QWidget::tr("MatchRoute2"));
    pushButton_StopRouteMatch = new QPushButton(QWidget::tr("StopRouteMatch"));
    pushButton_LockPointA = new QPushButton(QWidget::tr("LockPointA"));
    pushButton_StartRouteReplay = new QPushButton(QWidget::tr("StartReplay"));
    pushButton_EndRouteReplay = new QPushButton(QWidget::tr("EndReplay"));
    pushButton_RelocatePointB = new QPushButton(QWidget::tr("RelocatePointB"));
    pushButton_FinalArrieve = new QPushButton(QWidget::tr("FinalArrieve"));
    pushButton_QuitP2P = new QPushButton(QWidget::tr("QuitP2P"));
    pushButton_P2pReqPld = new QPushButton(QWidget::tr("P2pOpenPld"));
    pushButton_P2pReqPld->setCheckable(true);
    pushButton_P2pReqPld->setChecked(false);

    pushButton_P2pVpCtrl = new QPushButton(QWidget::tr("P2pVpCtrl"));
    pushButton_P2pVpCtrl->setCheckable(true);
    pushButton_P2pVpCtrl->setChecked(false);

    pushButton_P2pParkReq = new QPushButton(QWidget::tr("P2pParkReq"));
    pushButton_P2pParkReq->setCheckable(true);
    pushButton_P2pParkReq->setChecked(false);




    //YHL: 1. create widget
    groupBox_input_info = new QGroupBox("InputInfo");
    groupBox_input_info->setMaximumWidth(400);

    groupBox_control_info = new QGroupBox("ControlInfo");
    groupBox_control_info->setMaximumWidth(400);

    groupBox_output_info = new QGroupBox("OutputInfo");
    groupBox_output_info->setMaximumWidth(400);

    p2p_client_widget = new QWidget;

    //YHL: 2. create layout

    layout_p2p_client = new QHBoxLayout;
    layout_p2p_client->setSpacing(1);

    layout_p2p_client_left = new QVBoxLayout;
    layout_p2p_client_left->setSpacing(1);

    layout_p2p_client_right = new QVBoxLayout;
    layout_p2p_client_right->setSpacing(1);

    layout_groupBox_input_info = new QGridLayout;
    layout_groupBox_input_info->setSpacing(1);

    layout_groupBox_control_info = new QGridLayout;
    layout_groupBox_control_info->setSpacing(1);

    layout_groupBox_output_info = new QGridLayout;
    layout_groupBox_output_info->setSpacing(1);
    
    //YHL: 3. assemble
    p2p_client_widget->setLayout(layout_p2p_client);

    layout_p2p_client->addLayout(layout_p2p_client_left);
    layout_p2p_client->addLayout(layout_p2p_client_right);

    layout_p2p_client_left->addWidget(mPlot);
    layout_p2p_client_left->addWidget(plainTextEdit_log);

    layout_p2p_client_right->addWidget(groupBox_input_info);
    layout_p2p_client_right->addWidget(groupBox_control_info);
    layout_p2p_client_right->addWidget(groupBox_output_info);

    groupBox_input_info->setLayout(layout_groupBox_input_info);
    groupBox_control_info->setLayout(layout_groupBox_control_info);
    groupBox_output_info->setLayout(layout_groupBox_output_info);
    
    //YHL: input info group
    layout_groupBox_input_info->addWidget(label_timestamp, 0, 0);
    layout_groupBox_input_info->addWidget(lineEdit_timestamp, 1, 0);
    layout_groupBox_input_info->addWidget(label_steering_angle, 2, 0);
    layout_groupBox_input_info->addWidget(lineEdit_steering_angle, 3, 0);

    layout_groupBox_input_info->addWidget(label_WRSLDWhlDistPlsCntr, 4, 0);
    layout_groupBox_input_info->addWidget(lineEdit_WRSLDWhlDistPlsCntr, 5, 0);
    layout_groupBox_input_info->addWidget(label_WRSRDWhlDistPlsCntr, 6, 0);
    layout_groupBox_input_info->addWidget(lineEdit_WRSRDWhlDistPlsCntr, 7, 0);
    layout_groupBox_input_info->addWidget(label_WRSLNDWhlDistPCntr, 8, 0);
    layout_groupBox_input_info->addWidget(lineEdit_WRSLNDWhlDistPCntr, 9, 0);
    layout_groupBox_input_info->addWidget(label_WRSRNDWhlDistPCntr, 10, 0);
    layout_groupBox_input_info->addWidget(lineEdit_WRSRNDWhlDistPCntr, 11, 0);

    layout_groupBox_input_info->addWidget(label_shift_pos, 0, 1);
    layout_groupBox_input_info->addWidget(lineEdit_shift_pos, 1, 1);

    layout_groupBox_input_info->addWidget(label_vehicle_speed, 2, 1);
    layout_groupBox_input_info->addWidget(lineEdit_vehicle_speed, 3, 1);

    layout_groupBox_input_info->addWidget(label_wheel_speed_fl, 4, 1);
    layout_groupBox_input_info->addWidget(lineEdit_wheel_speed_fl, 5, 1);
    layout_groupBox_input_info->addWidget(label_wheel_speed_fr, 6, 1);
    layout_groupBox_input_info->addWidget(lineEdit_wheel_speed_fr, 7, 1);
    layout_groupBox_input_info->addWidget(label_wheel_speed_rl, 8, 1);
    layout_groupBox_input_info->addWidget(lineEdit_wheel_speed_rl, 9, 1);
    layout_groupBox_input_info->addWidget(label_wheel_speed_rr, 10, 1);
    layout_groupBox_input_info->addWidget(lineEdit_wheel_speed_rr, 11, 1);

    layout_groupBox_input_info->addWidget(label_yaw_rate, 12, 0);
    layout_groupBox_input_info->addWidget(lineEdit_yaw_rate, 13, 0);


    //YHL: control cmd
    layout_groupBox_control_info->addWidget(pushButton_StartEnvironmentDetection, 0, 0);
    layout_groupBox_control_info->addWidget(pushButton_StartRouteLearning, 1, 0);
    layout_groupBox_control_info->addWidget(pushButton_EndRouteLearning, 2, 0);
    layout_groupBox_control_info->addWidget(pushButton_CancleRouteLearning, 3, 0);
    layout_groupBox_control_info->addWidget(pushButton_SaveRoute1, 4, 0);
    layout_groupBox_control_info->addWidget(pushButton_SaveRoute2, 5, 0);
    layout_groupBox_control_info->addWidget(pushButton_StartMapping, 6, 0);
    layout_groupBox_control_info->addWidget(pushButton_DeleteRoute1, 7, 0);
    layout_groupBox_control_info->addWidget(pushButton_DeleteRoute2, 8, 0);
    layout_groupBox_control_info->addWidget(pushButton_StopRouteDelete, 9, 0);

    layout_groupBox_control_info->addWidget(pushButton_MatchRoute1, 0, 1);
    layout_groupBox_control_info->addWidget(pushButton_MatchRoute2, 1, 1);
    layout_groupBox_control_info->addWidget(pushButton_StopRouteMatch, 2, 1);
    layout_groupBox_control_info->addWidget(pushButton_LockPointA, 3, 1);
    layout_groupBox_control_info->addWidget(pushButton_StartRouteReplay, 4, 1);
    layout_groupBox_control_info->addWidget(pushButton_EndRouteReplay, 5, 1);
    layout_groupBox_control_info->addWidget(pushButton_RelocatePointB, 6, 1);
    layout_groupBox_control_info->addWidget(pushButton_FinalArrieve, 7, 1);
    layout_groupBox_control_info->addWidget(pushButton_QuitP2P, 8, 1);
    layout_groupBox_control_info->addWidget(pushButton_P2pReqPld, 9, 1);
    layout_groupBox_control_info->addWidget(pushButton_P2pVpCtrl, 8, 2);
    layout_groupBox_control_info->addWidget(pushButton_P2pParkReq, 9, 2);

    layout_groupBox_control_info->addWidget(pushButton_data_recorde, 0, 2);
    layout_groupBox_control_info->addWidget(pushButton_plot_route_1, 1, 2);
    layout_groupBox_control_info->addWidget(pushButton_plot_route_2, 2, 2);
    layout_groupBox_control_info->addWidget(pushButton_copy_p2p_data, 3, 2);
    layout_groupBox_control_info->addWidget(pushButton_send_p2p_cmd, 4, 2);


    //YHL: output info

    layout_groupBox_output_info->addWidget(label_working_state, 0, 0);
    layout_groupBox_output_info->addWidget(lineEdit_working_state, 1, 0);

    layout_groupBox_output_info->addWidget(label_system_state, 2, 0);
    layout_groupBox_output_info->addWidget(lineEdit_system_state, 3, 0);

    layout_groupBox_output_info->addWidget(label_failure_state, 4, 0);
    layout_groupBox_output_info->addWidget(lineEdit_failure_state, 5, 0);

    layout_groupBox_output_info->addWidget(label_route_save_percentage, 6, 0);
    layout_groupBox_output_info->addWidget(lineEdit_route_save_percentage, 7, 0);

    layout_groupBox_output_info->addWidget(label_route_learning_state, 8, 0);
    layout_groupBox_output_info->addWidget(lineEdit_route_learning_state, 9, 0);

    layout_groupBox_output_info->addWidget(label_route1_save_result, 10, 0);
    layout_groupBox_output_info->addWidget(lineEdit_route1_save_result, 11, 0);

    layout_groupBox_output_info->addWidget(label_route1_delete_result, 12, 0);
    layout_groupBox_output_info->addWidget(lineEdit_route1_delete_result, 13, 0);


    layout_groupBox_output_info->addWidget(label_route2_save_result, 0, 1);
    layout_groupBox_output_info->addWidget(lineEdit_route2_save_result, 1, 1);

    layout_groupBox_output_info->addWidget(label_route2_delete_result, 2, 1);
    layout_groupBox_output_info->addWidget(lineEdit_route2_delete_result, 3, 1);

    layout_groupBox_output_info->addWidget(label_radar_obs_state, 4, 1);
    layout_groupBox_output_info->addWidget(lineEdit_radar_obs_state, 5, 1);

    layout_groupBox_output_info->addWidget(label_route_plan_state, 6, 1);
    layout_groupBox_output_info->addWidget(lineEdit_route_plan_state, 7, 1);
    
    layout_groupBox_output_info->addWidget(label_reloc_slam_state, 8, 1);
    layout_groupBox_output_info->addWidget(lineEdit_reloc_slam_state, 9, 1);

    layout_groupBox_output_info->addWidget(label_rout1_exist_state, 10, 1);
    layout_groupBox_output_info->addWidget(lineEdit_rout1_exist_state, 11, 1);

    layout_groupBox_output_info->addWidget(label_rout2_exist_state, 12, 1);
    layout_groupBox_output_info->addWidget(lineEdit_rout2_exist_state, 13, 1);

    layout_groupBox_output_info->addWidget(label_route_replay_state, 0, 2);
    layout_groupBox_output_info->addWidget(lineEdit_route_replay_state, 1, 2);

    layout_groupBox_output_info->addWidget(label_mod_x, 2, 2);
    layout_groupBox_output_info->addWidget(lineEdit_mod_x, 3, 2);

    layout_groupBox_output_info->addWidget(label_mod_y, 4, 2);
    layout_groupBox_output_info->addWidget(lineEdit_mod_y, 5, 2);

    layout_groupBox_output_info->addWidget(label_mod_dist, 6, 2);
    layout_groupBox_output_info->addWidget(lineEdit_mod_dist, 7, 2);

    layout_groupBox_output_info->addWidget(label_a_point_available_result, 8, 2);
    layout_groupBox_output_info->addWidget(lineEdit_a_point_available_result, 9, 2);

    layout_groupBox_output_info->addWidget(label_pos_valid, 10, 2);
    layout_groupBox_output_info->addWidget(lineEdit_pos_valid, 11, 2);

    layout_groupBox_output_info->addWidget(label_pos_x, 12, 2);
    layout_groupBox_output_info->addWidget(lineEdit_pos_x, 13, 2);

    layout_groupBox_output_info->addWidget(label_pos_y, 0, 3);
    layout_groupBox_output_info->addWidget(lineEdit_pos_y, 1, 3);

    layout_groupBox_output_info->addWidget(label_pos_theta, 2, 3);
    layout_groupBox_output_info->addWidget(lineEdit_pos_theta, 3, 3);

    layout_groupBox_output_info->addWidget(label_dist2path, 4, 3);
    layout_groupBox_output_info->addWidget(lineEdit_dist2path, 5, 3);

    layout_groupBox_output_info->addWidget(label_angle2path, 6, 3);
    layout_groupBox_output_info->addWidget(lineEdit_angle2path, 7, 3);

    layout_groupBox_output_info->addWidget(label_dist2pointB, 8, 3);
    layout_groupBox_output_info->addWidget(lineEdit_dist2pointB, 9, 3);

    layout_groupBox_output_info->addWidget(label_angle2pointB, 10, 3);
    layout_groupBox_output_info->addWidget(lineEdit_angle2pointB, 11, 3);

    layout_groupBox_output_info->addWidget(label_lengthAB, 12, 3);
    layout_groupBox_output_info->addWidget(lineEdit_lengthAB, 13, 3);

    //YHL: conncet
    connect(pushButton_data_recorde, SIGNAL(clicked()), this, SLOT(OnDataButtonClicked()));
    connect(pushButton_plot_route_1, SIGNAL(clicked()), this, SLOT(OnPlotRoute1ButtonClicked()));
    connect(pushButton_plot_route_2, SIGNAL(clicked()), this, SLOT(OnPlotRoute2ButtonClicked()));
    connect(pushButton_copy_p2p_data, SIGNAL(clicked()), this, SLOT(OnCopyP2pDataButtonClicked()));
    connect(pushButton_send_p2p_cmd, SIGNAL(clicked()), this, SLOT(OnSendP2pCmdButtonClicked()));

    //p2p cmd push button slot set
    connect(pushButton_StartEnvironmentDetection, SIGNAL(clicked()), this, SLOT(OnStartEnvironmentDetectionButtonClicked()));
    connect(pushButton_StartRouteLearning, SIGNAL(clicked()), this, SLOT(OnStartRouteLearningButtonClicked()));
    connect(pushButton_EndRouteLearning, SIGNAL(clicked()), this, SLOT(OnEndRouteLearningButtonClicked()));
    connect(pushButton_CancleRouteLearning, SIGNAL(clicked()), this, SLOT(OnCancleRouteLearningButtonClicked()));
    connect(pushButton_SaveRoute1, SIGNAL(clicked()), this, SLOT(OnSaveRoute1ButtonClicked()));
    connect(pushButton_SaveRoute2, SIGNAL(clicked()), this, SLOT(OnSaveRoute3ButtonClicked()));
    connect(pushButton_StartMapping, SIGNAL(clicked()), this, SLOT(OnStartMappingButtonClicked()));
    connect(pushButton_DeleteRoute1, SIGNAL(clicked()), this, SLOT(OnDeleteRoute1ButtonClicked()));
    connect(pushButton_DeleteRoute2, SIGNAL(clicked()), this, SLOT(OnDeleteRoute2ButtonClicked()));
    connect(pushButton_StopRouteDelete, SIGNAL(clicked()), this, SLOT(OnStopRouteDeleteButtonClicked()));
    connect(pushButton_MatchRoute1, SIGNAL(clicked()), this, SLOT(OnMatchRoute1ButtonClicked()));
    connect(pushButton_MatchRoute2, SIGNAL(clicked()), this, SLOT(OnMatchRoute2ButtonClicked()));
    connect(pushButton_StopRouteMatch, SIGNAL(clicked()), this, SLOT(OnStopRouteMatchButtonClicked()));
    connect(pushButton_LockPointA, SIGNAL(clicked()), this, SLOT(OnLockPointAButtonClicked()));
    connect(pushButton_StartRouteReplay, SIGNAL(clicked()), this, SLOT(OnStartRouteReplayButtonClicked()));
    connect(pushButton_EndRouteReplay, SIGNAL(clicked()), this, SLOT(OnEndRouteReplayButtonClicked()));
    connect(pushButton_RelocatePointB, SIGNAL(clicked()), this, SLOT(OnRelocatePointBButtonClicked()));
    connect(pushButton_FinalArrieve, SIGNAL(clicked()), this, SLOT(OnFinalArrieveButtonClicked()));
    connect(pushButton_QuitP2P, SIGNAL(clicked()), this, SLOT(OnQuitP2PButtonClicked()));
    connect(pushButton_P2pReqPld, SIGNAL(clicked()), this, SLOT(OnP2pReqPldButtonClicked()));
    connect(pushButton_P2pVpCtrl, SIGNAL(clicked()), this, SLOT(OnP2ppVpCtrlButtonClicked()));
    connect(pushButton_P2pParkReq, SIGNAL(clicked()), this, SLOT(OnP2pParkReqButtonClicked()));
}

P2pClient::~P2pClient()
{
}

void P2pClient::UpdateP2pLcmMsg(P2pLcmData lcm_data)
{
    //std::cout<<"get signal p2p lcm msg "<<lcm_data.timestamp<<std::endl;
    if(pushButton_data_recorde->isChecked()==1)
    {
        log_file_.write(reinterpret_cast<const char*>(&lcm_data), sizeof(P2pLcmData));
    }
    static int count = 0;
    count++;



    static int point_count = 0;
    static float last_x = 0.0;
    static float last_y = 0.0;
//    lcm_data.pos_x = 0.01*count*sin(0.01*count);
//    lcm_data.pos_y = 0.01*count*cos(0.01*count);
//    lcm_data.pos_theta = 0.01*count;//YHL: temp
    vehicle_pose_x = lcm_data.pos_x;
    vehicle_pose_y = lcm_data.pos_y;
    vehicle_pose_theta = lcm_data.pos_theta;
    if(sqrt((last_x - lcm_data.pos_x)*(last_x - lcm_data.pos_x) + (last_y - lcm_data.pos_y)*(last_y - lcm_data.pos_y))>0.1)
    {
            real_time_pose_curve->addData(lcm_data.pos_y, lcm_data.pos_x);
            last_x = lcm_data.pos_x;
            last_y = lcm_data.pos_y;
            point_count++;
    }
    if(point_count>4096)
    {
        point_count = 0;
        real_time_pose_curve->data().data()->clear();
    }

    QVector<QCPCurveData> vehicle_curve_point(10);
    for(int i=0; i<10; i++)
    {
        vehicle_curve_point[i] = QCPCurveData(i,vehicle_curve_point_x[i]*sin(lcm_data.pos_theta)+vehicle_curve_point_y[i]*cos(lcm_data.pos_theta)+lcm_data.pos_y, vehicle_curve_point_x[i]*cos(lcm_data.pos_theta)-vehicle_curve_point_y[i]*sin(lcm_data.pos_theta)+lcm_data.pos_x);
    }
    vehicle_curve->data().data()->clear();
    vehicle_curve->data()->set(vehicle_curve_point, true);

    lineEdit_pos_x->setText(QString::number(lcm_data.pos_x, 'f', 3));
    lineEdit_pos_y->setText(QString::number(lcm_data.pos_y, 'f', 3));
    lineEdit_pos_theta->setText(QString::number(lcm_data.pos_theta, 'f', 3));

    lineEdit_timestamp->setText(QString::number(point_count, 10));
    lineEdit_steering_angle->setText(QString::number(lcm_data.steering_angle, 'f', 3));
    lineEdit_vehicle_speed->setText(QString::number(lcm_data.vehicle_speed, 'f', 3));
    lineEdit_wheel_speed_fl->setText(QString::number(lcm_data.wheel_speed_fl, 'f', 3));
    lineEdit_wheel_speed_fl->setText(QString::number(lcm_data.wheel_speed_fr, 'f', 3));
    lineEdit_wheel_speed_fl->setText(QString::number(lcm_data.wheel_speed_rl, 'f', 3));
    lineEdit_wheel_speed_fl->setText(QString::number(lcm_data.wheel_speed_rr, 'f', 3));
    lineEdit_shift_pos->setText(QString::number(lcm_data.shift_pos, 10));
    lineEdit_WRSLDWhlDistPlsCntr->setText(QString::number(lcm_data.WRSLDWhlDistPlsCntr, 10));
    lineEdit_WRSRDWhlDistPlsCntr->setText(QString::number(lcm_data.WRSRDWhlDistPlsCntr, 10));
    lineEdit_WRSLNDWhlDistPCntr->setText(QString::number(lcm_data.WRSLNDWhlDistPCntr, 10));
    lineEdit_WRSRNDWhlDistPCntr->setText(QString::number(lcm_data.WRSRNDWhlDistPCntr, 10));


    lineEdit_yaw_rate->setText(QString::number(lcm_data.yaw_rate, 'f', 3));

    lineEdit_door_sts_fl->setText(QString::number(lcm_data.door_sts_fl, 10));
    lineEdit_door_sts_fr->setText(QString::number(lcm_data.door_sts_fr, 10));
    lineEdit_door_sts_rl->setText(QString::number(lcm_data.door_sts_rl, 10));
    lineEdit_door_sts_rr->setText(QString::number(lcm_data.door_sts_rr, 10));
    lineEdit_left_turn_sig->setText(QString::number(lcm_data.left_turn_sig, 10));
    lineEdit_right_turn_sig->setText(QString::number(lcm_data.right_turn_sig, 10));
    lineEdit_brake_act_flag->setText(QString::number(lcm_data.brake_act_flag, 10));
    lineEdit_HighBmAct->setText(QString::number(lcm_data.HighBmAct, 10));
    lineEdit_LowBmAct->setText(QString::number(lcm_data.LowBmAct, 10));





    lineEdit_radar_rear_left_side_distance->setText(QString::number(lcm_data.radar_rear_left_side_distance, 'f', 3));
    lineEdit_radar_rear_left_coner_distance->setText(QString::number(lcm_data.radar_rear_left_coner_distance, 'f', 3));
    lineEdit_radar_rear_left_middle_distance->setText(QString::number(lcm_data.radar_rear_left_middle_distance, 'f', 3));
    lineEdit_radar_rear_right_side_distance->setText(QString::number(lcm_data.radar_rear_right_side_distance, 'f', 3));
    lineEdit_radar_rear_right_coner_distance->setText(QString::number(lcm_data.radar_rear_right_coner_distance, 'f', 3));
    lineEdit_radar_rear_right_middle_distance->setText(QString::number(lcm_data.radar_rear_right_middle_distance, 'f', 3));
    lineEdit_radar_front_left_side_distance->setText(QString::number(lcm_data.radar_front_left_side_distance, 'f', 3));
    lineEdit_radar_front_left_coner_distance->setText(QString::number(lcm_data.radar_front_left_coner_distance, 'f', 3));
    lineEdit_radar_front_left_middle_distance->setText(QString::number(lcm_data.radar_front_left_middle_distance, 'f', 3));
    lineEdit_radar_front_right_side_distance->setText(QString::number(lcm_data.radar_front_right_side_distance, 'f', 3));
    lineEdit_radar_front_right_coner_distance->setText(QString::number(lcm_data.radar_front_right_coner_distance, 'f', 3));
    lineEdit_radar_front_right_middle_distance->setText(QString::number(lcm_data.radar_front_right_middle_distance, 'f', 3));


    lineEdit_Electrical_Power_Steering_Availability_Status->setText(QString::number(lcm_data.Electrical_Power_Steering_Availability_Status, 10));
    lineEdit_timestamp->setText(QString::number(lcm_data.timestamp, 10));

    lineEdit_working_state->setText(QString::number(lcm_data.working_state, 16));
    lineEdit_system_state->setText(QString::number(lcm_data.system_state, 16));
    lineEdit_failure_state->setText(QString::number(lcm_data.failure_state, 16));
    lineEdit_route_save_percentage->setText(QString::number(lcm_data.route_save_percentage, 10));
    lineEdit_route_learning_state->setText(QString::number(lcm_data.route_learning_state, 16));
    lineEdit_route1_save_result->setText(QString::number(lcm_data.route1_save_result, 16));
    lineEdit_route1_delete_result->setText(QString::number(lcm_data.route1_delete_result, 16));
    lineEdit_route2_save_result->setText(QString::number(lcm_data.route2_save_result, 16));
    lineEdit_route2_delete_result->setText(QString::number(lcm_data.route2_delete_result, 16));
    lineEdit_radar_obs_state->setText(QString::number(lcm_data.radar_obs_state, 16));
    lineEdit_route_plan_state->setText(QString::number(lcm_data.route_plan_state, 16));
    lineEdit_reloc_slam_state->setText(QString::number(lcm_data.reloc_slam_state, 16));
    lineEdit_rout1_exist_state->setText(QString::number(lcm_data.rout1_exist_state, 16));
    lineEdit_rout2_exist_state->setText(QString::number(lcm_data.rout2_exist_state, 16));
    lineEdit_route_replay_state->setText(QString::number(lcm_data.route_replay_state, 16));
    lineEdit_mod_x->setText(QString::number(lcm_data.mod_x, 16));
    lineEdit_mod_y->setText(QString::number(lcm_data.mod_y, 16));
    lineEdit_mod_dist->setText(QString::number(lcm_data.mod_dist, 16));
    lineEdit_a_point_available_result->setText(QString::number(lcm_data.a_point_available_result, 16));

    lineEdit_pos_valid->setText(QString::number(lcm_data.pos_valid, 16));



    lineEdit_pos_x->setText(QString::number(lcm_data.pos_x, 'f', 3));
    lineEdit_pos_y->setText(QString::number(lcm_data.pos_y, 'f', 3));
    lineEdit_pos_theta->setText(QString::number(lcm_data.pos_theta, 'f', 3));


    lineEdit_dist2path->setText(QString::number(lcm_data.dist2path, 'f', 3));
    lineEdit_angle2path->setText(QString::number(lcm_data.angle2path, 'f', 3));
    lineEdit_dist2pointB->setText(QString::number(lcm_data.dist2pointB, 'f', 3));
    lineEdit_angle2pointB->setText(QString::number(lcm_data.angle2pointB, 'f', 3));
    lineEdit_lengthAB->setText(QString::number(lcm_data.lengthAB, 'f', 3));




    mPlot->replot(QCustomPlot::rpQueuedReplot);
    return;





}


void P2pClient::OnDataButtonClicked()
{
#if 1
    std::cout<<"clicked, checkbale="<< pushButton_data_recorde->isCheckable()<<"checked="<< pushButton_data_recorde->isChecked()<<std::endl;
    if(pushButton_data_recorde->isChecked()==1)
    {
        // creat log file
        QDateTime time = QDateTime::currentDateTime();
        std::cout<<time.toString("yyyy_MM_dd_HH_mm_ss").toStdString()<<std::endl;

        log_file_.open(reinterpret_cast<const char*>((time.toString("yyyy-MM-dd_HH-mm-ss").toStdString()+"_lcm_data.bin").c_str()), std::ios::binary);
        pushButton_data_recorde->setText(QWidget::tr("StopLogP2pLcmData"));
    }
    else
    {
        // close log file
        log_file_.close();
        pushButton_data_recorde->setText(QWidget::tr("StartLogP2pLcmData"));
    }
#endif
}

void P2pClient::OnPlotRoute1ButtonClicked()
{

    std::ifstream route_file;
    route_file.open(reinterpret_cast<const char*>("p2p\\route_1\\path.bin"),std::ifstream::binary);
    if(!route_file.is_open())
    {
        std::cout<<"open path error"<<std::endl;
        return;
    }
    route_file.read(reinterpret_cast<char *>(&path_data_),10012);
    route_file.close();

    //YHL: draw learning route
    int num = path_data_.path_data[9].x;
    QVector<double> x(num), y(num);
    for (int i=0; i<num; ++i)
    {
        x[i] = path_data_.path_data[i+10].x;
        y[i] = path_data_.path_data[i+10].y;
    }
    for(int i=0;i<num+10;i++)
    {
        std::cout<<i<<"x="<<path_data_.path_data[i].x<<" y="<<path_data_.path_data[i].y<<"theta="<<path_data_.path_data[i].theta<<"shift="<<path_data_.path_data[i].gear_shift<<std::endl;
    }
    const int pointCount = num;
    QVector<QCPCurveData> curve_data(pointCount);
    for (int i=0; i<pointCount; ++i)
    {
        curve_data[i] = QCPCurveData(i, path_data_.path_data[i+10].y, path_data_.path_data[i+10].x);
    }
    // pass the data to the curves; we know t (i in loop above) is ascending, so set alreadySorted=true (saves an extra internal sort):
    route_1_curve->data()->set(curve_data, true);

    //YHL: draw target slot
    QVector<QCPCurveData> slot_curve_point(5);
    for(int i=0; i<4; i++)
    {
        slot_curve_point[i] = QCPCurveData(i,path_data_.path_data[i+3].y, path_data_.path_data[i+3].x);
    }
    slot_curve_point[4] = QCPCurveData(4,path_data_.path_data[3].y, path_data_.path_data[3].x);
    target_slot_1_curve->data()->set(slot_curve_point, true);

    mPlot->yAxis->setScaleRatio(mPlot->xAxis, mPlot->size().width() / mPlot->size().height());//YHL: axis equal
    mPlot->replot();
}

void P2pClient::OnPlotRoute2ButtonClicked()
{
    std::ifstream route_file;
    route_file.open(reinterpret_cast<const char*>("p2p\\route_2\\path.bin"),std::ifstream::binary);
    if(!route_file.is_open())
    {
        std::cout<<"open path error"<<std::endl;
        return;
    }
    route_file.read(reinterpret_cast<char *>(&path_data_),10012);
    route_file.close();

    //YHL: draw learning route
    int num = path_data_.path_data[9].x;
    QVector<double> x(num), y(num);
    for (int i=0; i<num; ++i)
    {
        x[i] = path_data_.path_data[i+10].x;
        y[i] = path_data_.path_data[i+10].y;
    }
    for(int i=0;i<num+10;i++)
    {
        std::cout<<i<<"x="<<path_data_.path_data[i].x<<" y="<<path_data_.path_data[i].y<<"theta="<<path_data_.path_data[i].theta<<"shift="<<path_data_.path_data[i].gear_shift<<std::endl;
    }
    const int pointCount = num;
    QVector<QCPCurveData> curve_data(pointCount);
    for (int i=0; i<pointCount; ++i)
    {
        curve_data[i] = QCPCurveData(i, path_data_.path_data[i+10].y, path_data_.path_data[i+10].x);
    }
    // pass the data to the curves; we know t (i in loop above) is ascending, so set alreadySorted=true (saves an extra internal sort):
    route_2_curve->data()->set(curve_data, true);

    //YHL: draw target slot
    QVector<QCPCurveData> slot_curve_point(5);
    for(int i=0; i<4; i++)
    {
        slot_curve_point[i] = QCPCurveData(i,path_data_.path_data[i+3].y, path_data_.path_data[i+3].x);
    }
    slot_curve_point[4] = QCPCurveData(4,path_data_.path_data[3].y, path_data_.path_data[3].x);
    target_slot_2_curve->data()->set(slot_curve_point, true);

//    for(int i=0;i<24;i++)
//    {
//        for(int j=0; j<5; j++)
//        {
//            slot_curve_point[j].key +=3;
//        }
//        mp_detected_slot_curves[i]->PlotSlot(slot_curve_point);
//    }
//    int id = 0xf;
//    QString slot_desc = QString::number(id, 10);
//    for(int i=0; i<24; i++)
//    {
//        mp_detected_slot_desc[i]->position->setCoords(i, 0);
//        mp_detected_slot_desc[i]->setText(slot_desc+"∠⊥//");
//        mp_detected_slot_desc[i]->setVisible(true);
//    }


    for(int i=0; i<12; i++)
    {

        mp_detected_slot_desc[i]->setVisible(false);
    }

    mPlot->yAxis->setScaleRatio(mPlot->xAxis, mPlot->size().width() / mPlot->size().height());//YHL: axis equal
    mPlot->replot();
}


void P2pClient::OnCopyP2pDataButtonClicked()
{
    #ifdef WIN_MSVC_X86_64
    ShellExecute(NULL, NULL, "cmd.exe","/c scp -r root@192.168.1.235:/opt/vision_sdk/project/app_cfg/p2p .", NULL,SW_SHOWNA);
    #endif
    #ifdef LINUX_GCC_X86_64
    system("scp -r root@192.168.1.235:/opt/vision_sdk/project/app_cfg/p2p .");
    #endif
    std::cout<<"copy p2p data finished!"<<std::endl;
}



void P2pClient::OnSendP2pCmdButtonClicked()
{

}


void P2pClient::OnStartEnvironmentDetectionButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 1;
        p2p_cmd.parameter[0] = 1;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnStartRouteLearningButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 1;
        p2p_cmd.parameter[0] = 2;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnEndRouteLearningButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 1;
        p2p_cmd.parameter[0] = 3;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnCancleRouteLearningButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 1;
        p2p_cmd.parameter[0] = 4;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnSaveRoute1ButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 1;
        p2p_cmd.parameter[0] = 5;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnSaveRoute3ButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 1;
        p2p_cmd.parameter[0] = 6;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}
void P2pClient::OnStartMappingButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 1;
        p2p_cmd.parameter[0] = 7;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnDeleteRoute1ButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 2;
        p2p_cmd.parameter[0] = 1;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnDeleteRoute2ButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 2;
        p2p_cmd.parameter[0] = 2;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnStopRouteDeleteButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 2;
        p2p_cmd.parameter[0] = 3;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnMatchRoute1ButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 3;
        p2p_cmd.parameter[0] = 1;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnMatchRoute2ButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 3;
        p2p_cmd.parameter[0] = 2;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnStopRouteMatchButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 3;
        p2p_cmd.parameter[0] = 3;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnLockPointAButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 4;
        p2p_cmd.parameter[0] = 1;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnStartRouteReplayButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 5;
        p2p_cmd.parameter[0] = 1;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnEndRouteReplayButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 5;
        p2p_cmd.parameter[0] = 2;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnRelocatePointBButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 6;
        p2p_cmd.parameter[0] = 1;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnFinalArrieveButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 8;
        p2p_cmd.parameter[0] = 1;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnQuitP2PButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;
        p2p_cmd.ctrl_status = 12;
        p2p_cmd.parameter[0] = 1;
        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnP2pReqPldButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;

        if(pushButton_P2pReqPld->isChecked()==1)
        {
            p2p_cmd.ctrl_status = 10;
            p2p_cmd.parameter[0] = 1;
            pushButton_P2pReqPld->setText(QWidget::tr("P2pClosePld"));
        }
        else
        {
            p2p_cmd.ctrl_status = 10;
            p2p_cmd.parameter[0] = 2;
            pushButton_P2pReqPld->setText(QWidget::tr("P2pOpenPld"));
        }

        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnP2ppVpCtrlButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;

        if(pushButton_P2pVpCtrl->isChecked()==1)
        {
            p2p_cmd.ctrl_status = 9;
            p2p_cmd.parameter[0] = 1;
            pushButton_P2pVpCtrl->setText(QWidget::tr("DisallowVpCtrl"));
        }
        else
        {
            p2p_cmd.ctrl_status = 9;
            p2p_cmd.parameter[0] = 2;
            pushButton_P2pVpCtrl->setText(QWidget::tr("AllowVpCtrl"));
        }

        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}

void P2pClient::OnP2pParkReqButtonClicked()
{
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    }
    else
    {
        Ctrl_Cmd_T p2p_cmd;

        if(pushButton_P2pParkReq->isChecked()==1)
        {
            p2p_cmd.ctrl_status = 11;
            p2p_cmd.parameter[0] = 1;
            pushButton_P2pParkReq->setText(QWidget::tr("P2pStopPark"));
        }
        else
        {
            p2p_cmd.ctrl_status = 11;
            p2p_cmd.parameter[0] = 0;
            pushButton_P2pParkReq->setText(QWidget::tr("P2pStartPark"));
        }

        lcm_sender.publish("LCM_P2P_CMD_MSG", &p2p_cmd, sizeof(Ctrl_Cmd_T));

    }
}
void P2pClient::OnApaPldResult(ApaValidSlots valid_slots)
{
    std::cout<<"valid slot num="<<valid_slots.valid_slot_nums<<std::endl;

    for(int k=0; k<valid_slots.valid_slot_nums; k++)
    {
        std::cout<<"slot id="<<valid_slots.valid_slots[k].id<<" type="<<int(valid_slots.valid_slots[k].slot_type)<<"location="<<int(valid_slots.valid_slots[k].location)<<std::endl;
        QVector<QCPCurveData> slot_curve_point(5);
        for(int i=0; i<4; i++)
        {
            slot_curve_point[i] = QCPCurveData(i,valid_slots.valid_slots[k].point[i].x*sin(vehicle_pose_theta)+valid_slots.valid_slots[k].point[i].y*cos(vehicle_pose_theta)+vehicle_pose_y, valid_slots.valid_slots[k].point[i].x*cos(vehicle_pose_theta)-valid_slots.valid_slots[k].point[i].y*sin(vehicle_pose_theta)+vehicle_pose_x);
        }
        slot_curve_point[4] = QCPCurveData(4,valid_slots.valid_slots[k].point[0].x*sin(vehicle_pose_theta)+valid_slots.valid_slots[k].point[0].y*cos(vehicle_pose_theta)+vehicle_pose_y, valid_slots.valid_slots[k].point[0].x*cos(vehicle_pose_theta)-valid_slots.valid_slots[k].point[0].y*sin(vehicle_pose_theta)+vehicle_pose_x);

        mp_detected_slot_curves[k]->PlotSlot(slot_curve_point);

        mp_detected_slot_desc[k]->position->setCoords(slot_curve_point[0].key, slot_curve_point[0].value);

        QString slot_desc = QString::number(valid_slots.valid_slots[k].id, 10);
        if(valid_slots.valid_slots[k].slot_type == 1)
        {
            mp_detected_slot_desc[k]->setText(slot_desc+"//");
        }
        else if(valid_slots.valid_slots[k].slot_type == 3)
        {
            mp_detected_slot_desc[k]->setText(slot_desc+"∠⊥");
        }
        else
        {
            mp_detected_slot_desc[k]->setText(slot_desc+QString::number(valid_slots.valid_slots[k].slot_type, 10)+"?");
        }

        mp_detected_slot_desc[k]->setVisible(true);
    }



    for(int k=valid_slots.valid_slot_nums;k<24;k++)
    {

        mp_detected_slot_curves[k]->ClearPlot();
        mp_detected_slot_desc[k]->setVisible(false);
    }

    mPlot->replot(QCustomPlot::rpQueuedReplot);
}

QWidget* P2pClient::GetWidget() const
{
    return p2p_client_widget;
}

QHBoxLayout* P2pClient::GetLayout() const
{
    return layout_p2p_client;
}