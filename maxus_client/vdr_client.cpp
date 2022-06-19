#include <iostream>
#include "vdr_client.h"
VdrClient::VdrClient(QWidget *parent):QWidget(parent)
{

    apa_plot = new QCustomPlot(parent);
    apa_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    apa_plot->setMinimumSize(500,500);
    apa_plot->xAxis->setRange(-25,25);
    apa_plot->yAxis->setRange(-25,25);
    apa_plot->yAxis->setScaleRatio(apa_plot->xAxis, 1.0);

    // mImage = new QCPItemPixmap(apa_plot);
    // mImage->setPixmap(QPixmap(400, 200));
    // mImage->topLeft->setType(QCPItemPosition::ptPlotCoords);
    // mImage->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    // mImage->topLeft->setCoords(-200, 100);
    // mImage->bottomRight->setCoords(200, -100);
    // apa_plot->addLayer("image", apa_plot->layer("main"), QCustomPlot::limBelow);
    // mImage->setLayer("image");
    // mImage->setScaled(true,Qt::IgnoreAspectRatio);


    vehicle_curve = new VehicleCurve(apa_plot);
    vehicle_curve->PlotSlot(0,0,0);
    last_plot_x = 0.0;
    last_plot_y = 0.0;;
    point_count = 0;
    vehicle_pose_x =0.0;
    vehicle_pose_y = 0.0;
    vehicle_pose_theta = 0.0;
    real_time_pose_curve = new QCPCurve(apa_plot->xAxis, apa_plot->yAxis);
    real_time_pose_curve->setPen(QPen(Qt::green));
    real_time_pose_curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc ));
    for(int i=0; i<24; i++)
    {
        mp_detected_slot_curves[i] = new SlotCurve(apa_plot);
        mp_detected_slot_desc[i] = new QCPItemText(apa_plot);
        mp_detected_slot_desc[i]->setVisible(false);

        //mp_detected_slot_desc[i]->setFont(QFont(font().family(), 16)); // make font a bit larger
        mp_detected_slot_desc[i]->setPen(QPen(Qt::black)); // show black border around text
    }
    for(int i=0;i<20;i++)
    {
        mp_object_box[i] = new ObjectBox(apa_plot);
    }

    

    lcm_sender = lcm_create("udpm://239.255.76.67:7667?ttl=100");
    if(!lcm_sender.good())
    {
        std::cout<<"lcm sender create failed!"<<std::endl;
    }
    else
    {
        std::cout<<"lcm sender create ok!"<<std::endl;
    }



    plainTextEdit_log = new QPlainTextEdit;
    plainTextEdit_log->setReadOnly(true);
    plainTextEdit_log->setMaximumHeight(100);

    
    lineEdit_timestamp = new QLineEdit("0");
    lineEdit_timestamp->setReadOnly(true);
    lineEdit_timestamp->setAlignment(Qt::AlignRight);
    lineEdit_timestamp->setMaxLength(15);
    lineEdit_timestamp->setMaximumWidth(100);

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


    lineEdit_apa_state_id = new QLineEdit("0.0");
    lineEdit_apa_state_id->setReadOnly(true);
    lineEdit_apa_state_id->setAlignment(Qt::AlignRight);
    lineEdit_apa_state_id->setMaxLength(15);
    lineEdit_apa_state_id->setMaximumWidth(100);

    // LLL
    lineEdit_VehicleModelParam = new QLineEdit("0.0");
    lineEdit_VehicleModelParam->setReadOnly(true);
    lineEdit_VehicleModelParam->setAlignment(Qt::AlignRight);
    lineEdit_VehicleModelParam->setMaxLength(15);
    lineEdit_VehicleModelParam->setMaximumWidth(100);

    label_VehicleModelParam = new QLabel("VehicleModelParam");
    label_VehicleModelParam->setBuddy(lineEdit_VehicleModelParam);

    label_timestamp = new QLabel("Timestamp(us)");
    label_timestamp->setBuddy(lineEdit_timestamp);
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
    label_WRSLDWhlDistPlsCntr = new QLabel("FL(cont)");
    label_WRSLDWhlDistPlsCntr->setBuddy(lineEdit_WRSLDWhlDistPlsCntr);
    label_WRSRDWhlDistPlsCntr = new QLabel("FR(cont)");
    label_WRSRDWhlDistPlsCntr->setBuddy(lineEdit_WRSRDWhlDistPlsCntr);
    label_WRSLNDWhlDistPCntr = new QLabel("RL(cont)");
    label_WRSLNDWhlDistPCntr->setBuddy(lineEdit_WRSLNDWhlDistPCntr);
    label_WRSRNDWhlDistPCntr = new QLabel("RR(cont)");
    label_WRSRNDWhlDistPCntr->setBuddy(lineEdit_WRSRNDWhlDistPCntr);
    label_apa_state_id = new QLabel("SM State");
    label_apa_state_id->setBuddy(lineEdit_apa_state_id);



    // label_shift_pos = new QLabel("Shift");
    // label_shift_pos->setBuddy(lineEdit_shift_pos);
    // label_WRSLDWhlDistPlsCntr = new QLabel("FL(cont)");
    // label_WRSLDWhlDistPlsCntr->setBuddy(lineEdit_WRSLDWhlDistPlsCntr);
    // label_WRSRDWhlDistPlsCntr = new QLabel("FR(cont)");
    // label_WRSRDWhlDistPlsCntr->setBuddy(lineEdit_WRSRDWhlDistPlsCntr);
    // label_WRSLNDWhlDistPCntr = new QLabel("RL(cont)");
    // label_WRSLNDWhlDistPCntr->setBuddy(lineEdit_WRSLNDWhlDistPCntr);
    // label_WRSRNDWhlDistPCntr = new QLabel("RR(cont)");
    // label_WRSRNDWhlDistPCntr->setBuddy(lineEdit_WRSRNDWhlDistPCntr);
    // label_apa_state_id = new QLabel("SM State");
    // label_apa_state_id->setBuddy(lineEdit_apa_state_id);



    
    pushButton_open_apa = new QPushButton("OPEN APA");
    pushButton_close_apa = new QPushButton("CLOSE APA");
    pushButton_set_target_slot = new QPushButton("SET SLOT");
    
    pushButton_lcm_log = new QPushButton("StartLog");
    pushButton_lcm_log->setCheckable(true);
    pushButton_lcm_log->setChecked(false);

    spinBox_target_slot_id = new QSpinBox;
    spinBox_target_slot_id->setRange(0, 12);
    
    groupBox_input_info = new QGroupBox("InputInfo");
    groupBox_input_info->setMaximumWidth(200);

    groupBox_control_info = new QGroupBox("ControlInfo");
    groupBox_control_info->setMaximumWidth(200);

    groupBox_output_info = new QGroupBox("OutputInfo");
    groupBox_output_info->setMaximumWidth(200);
    apa_client_widget = new QWidget;


    // layout
    layout_apa_client = new QHBoxLayout;
    layout_apa_client->setSpacing(1);

    layout_apa_client_left = new QVBoxLayout;
    layout_apa_client_left->setSpacing(1);
    layout_apa_client_right = new QVBoxLayout;
    layout_apa_client_right->setSpacing(1);

    layout_groupBox_input_info = new QGridLayout;
    layout_groupBox_input_info->setSpacing(1);
    layout_groupBox_control_info = new QGridLayout;
    layout_groupBox_control_info->setSpacing(1);
    layout_groupBox_output_info = new QGridLayout;
    layout_groupBox_output_info->setSpacing(1);

    apa_client_widget->setLayout(layout_apa_client);

    layout_apa_client->addLayout(layout_apa_client_left);
    layout_apa_client->addLayout(layout_apa_client_right);

    layout_apa_client_left->addWidget(apa_plot);
    layout_apa_client_left->addWidget(plainTextEdit_log);

    layout_apa_client_right->addWidget(groupBox_input_info);
    layout_apa_client_right->addWidget(groupBox_control_info);
    layout_apa_client_right->addWidget(groupBox_output_info);

    groupBox_input_info->setLayout(layout_groupBox_input_info);
    groupBox_control_info->setLayout(layout_groupBox_control_info);
    groupBox_output_info->setLayout(layout_groupBox_output_info);

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
    //YHL: control info
    layout_groupBox_control_info->addWidget(pushButton_open_apa, 0, 0);
    layout_groupBox_control_info->addWidget(pushButton_close_apa, 0, 1);
    layout_groupBox_control_info->addWidget(pushButton_set_target_slot, 1, 0);
    layout_groupBox_control_info->addWidget(spinBox_target_slot_id, 1, 1);
    layout_groupBox_control_info->addWidget(pushButton_lcm_log, 2, 0);

    //YHL: output info
    layout_groupBox_output_info->addWidget(label_apa_state_id, 2, 0);
    layout_groupBox_output_info->addWidget(lineEdit_apa_state_id, 3, 0);
    layout_groupBox_output_info->addWidget(label_VehicleModelParam, 0, 0);
    layout_groupBox_output_info->addWidget(lineEdit_VehicleModelParam, 1, 0);

    QSpacerItem *spacer = new QSpacerItem(30, 30, QSizePolicy::Maximum);
    layout_groupBox_output_info->addItem(spacer, 2, 0);
    layout_groupBox_output_info->setRowStretch(0, 1);
    layout_groupBox_output_info->setRowStretch(1, 1);
    layout_groupBox_output_info->setRowStretch(2, 8);






    //YHL: connect

 






}

VdrClient::~VdrClient()
{
    if(apa_plot != NULL)
    {
        delete apa_plot;
        apa_plot = NULL;
    }


}

QWidget* VdrClient::GetWidget() const
{
    return apa_client_widget;
}

QHBoxLayout* VdrClient::GetLayout() const
{
    return layout_apa_client;
}







void VdrClient::OnVdrVehiclePose(VehicleDeadReckoning vdr)
{
    std::cout<<"on OnVdrVehiclePose!"<<vdr.current_input.timestamp<<std::endl;
    lineEdit_timestamp->setText(QString::number(vdr.current_input.timestamp, 10));
    lineEdit_shift_pos->setText(QString::number(vdr.current_input.gear_shift, 10));
    lineEdit_steering_angle->setText(QString::number(vdr.current_input.steering_wheel_angle, 'f', 3));

    lineEdit_WRSLDWhlDistPlsCntr->setText(QString::number(vdr.current_input.wheel_pulse_rl, 10));
    lineEdit_WRSRDWhlDistPlsCntr->setText(QString::number(vdr.current_input.wheel_pulse_rr, 10));
    lineEdit_WRSLNDWhlDistPCntr->setText(QString::number(vdr.current_input.wheel_pulse_rl, 10));
    lineEdit_WRSRNDWhlDistPCntr->setText(QString::number(vdr.current_input.wheel_pulse_rr, 10));


//
    static int point_count = 0;
    static float last_x = 0.0;
    static float last_y = 0.0;


    if(sqrt((last_x - vdr.global_pose.x)*(last_x - vdr.global_pose.x) + (last_y - vdr.global_pose.y)*(last_y - vdr.global_pose.y))>0.1)
    {
            real_time_pose_curve->addData(vdr.global_pose.y, vdr.global_pose.x);
            last_x = vdr.global_pose.x;
            last_y = vdr.global_pose.y;
            point_count++;
    }
    if(point_count>4096)
    {
        point_count = 0;
        real_time_pose_curve->data().data()->clear();
    }

//

    vehicle_curve->PlotSlot(vdr.global_pose.x,vdr.global_pose.y,vdr.global_pose.theta);

    apa_plot->replot(QCustomPlot::rpQueuedReplot);

}

void VdrClient::OnVdrVehicleParam(VehicleModelParam vpm)
{
    lineEdit_VehicleModelParam->setText(QString::number(vpm.max_pulse, 10));
}