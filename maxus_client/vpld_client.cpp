#include <iostream>
#include "vpld_client.h"
VpldClient::VpldClient(QWidget *parent):QWidget(parent)
{

    apa_plot = new QCustomPlot(parent);
    apa_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    apa_plot->setMinimumSize(500,500);
    apa_plot->xAxis->setRange(-300,300);
    apa_plot->yAxis->setRange(-300,300);
    apa_plot->yAxis->setScaleRatio(apa_plot->xAxis, 1.0);

    mImage = new QCPItemPixmap(apa_plot);
    mImage->setPixmap(QPixmap(400, 200));
    mImage->topLeft->setType(QCPItemPosition::ptPlotCoords);
    mImage->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    mImage->topLeft->setCoords(-200, 100);
    mImage->bottomRight->setCoords(200, -100);
    apa_plot->addLayer("image", apa_plot->layer("main"), QCustomPlot::limBelow);
    mImage->setLayer("image");
    mImage->setScaled(true,Qt::IgnoreAspectRatio);


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

        mp_detected_slot_desc[i]->setFont(QFont(font().family(), 16)); // make font a bit larger
        mp_detected_slot_desc[i]->setPen(QPen(Qt::black)); // show black border around text
    }
    mp_target_slot_curves = new SlotCurve(apa_plot);
    mp_target_slot_desc = new QCPItemText(apa_plot);
    mp_target_slot_desc->setVisible(false);

    mp_target_slot_desc->setFont(QFont(font().family(), 16)); // make font a bit larger
    mp_target_slot_desc->setPen(QPen(Qt::black)); // show black border around text
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


    lineEdit_apa_state_id = new QLineEdit("0");
    lineEdit_apa_state_id->setReadOnly(true);
    lineEdit_apa_state_id->setAlignment(Qt::AlignRight);
    lineEdit_apa_state_id->setMaxLength(15);
    lineEdit_apa_state_id->setMaximumWidth(100);



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
    
    pushButton_open_apa = new QPushButton("OPEN APA");
    pushButton_close_apa = new QPushButton("CLOSE APA");
    pushButton_set_target_slot = new QPushButton("SET SLOT");
    
    pushButton_lcm_log = new QPushButton("StartLog");
    pushButton_lcm_log->setCheckable(true);
    pushButton_lcm_log->setChecked(false);
 
    pushButton_video_record = new QPushButton("StartRec");
    pushButton_video_record->setCheckable(true);
    pushButton_video_record->setChecked(false);

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
    layout_groupBox_control_info->addWidget(pushButton_video_record, 2, 0);

    //YHL: output info
    layout_groupBox_output_info->addWidget(label_apa_state_id, 0, 0);
    layout_groupBox_output_info->addWidget(lineEdit_apa_state_id, 1, 0);
    QSpacerItem *spacer = new QSpacerItem(50, 50, QSizePolicy::Maximum);
    layout_groupBox_output_info->addItem(spacer, 2, 0);
    layout_groupBox_output_info->setRowStretch(0, 1);
    layout_groupBox_output_info->setRowStretch(1, 1);
    layout_groupBox_output_info->setRowStretch(2, 8);






    //YHL: connect

 

    connect(pushButton_open_apa, SIGNAL(clicked()), this, SLOT(OnOpenApaButtonClicked()));
    connect(pushButton_close_apa, SIGNAL(clicked()), this, SLOT(OnCloseApaButtonClicked()));
    connect(pushButton_set_target_slot, SIGNAL(clicked()), this, SLOT(OnSetSlotButtonClicked()));
    connect(pushButton_lcm_log, SIGNAL(clicked()), this, SLOT(OnLcmLogButtonClicked()));
    connect(pushButton_video_record, SIGNAL(clicked()), this, SLOT(OnVideoRecordButtonClicked()));





}

VpldClient::~VpldClient()
{
    if(apa_plot != NULL)
    {
        delete apa_plot;
        apa_plot = NULL;
    }


}

QWidget* VpldClient::GetWidget() const
{
    return apa_client_widget;
}

QHBoxLayout* VpldClient::GetLayout() const
{
    return layout_apa_client;
}
    #if 0

void VpldClient::OnApaResult(ApaLcmData apa_lcm_data)
{
    //size = 11536
    // std::cout<<"on apa result, type size="<<sizeof(ApaLcmData)<<"data size="<<apa_lcm_data.data_size<<std::endl;
    lineEdit_steering_angle->setText(QString::number(apa_lcm_data.canInfo.steering_angle, 'f', 3));
    lineEdit_vehicle_speed->setText(QString::number(apa_lcm_data.canInfo.vehicle_speed, 'f', 3));
    lineEdit_yaw_rate->setText(QString::number(apa_lcm_data.canInfo.yaw_rate, 'f', 3));
    lineEdit_wheel_speed_fl->setText(QString::number(apa_lcm_data.canInfo.wheel_speed_fl, 'f', 3));
    lineEdit_wheel_speed_fr->setText(QString::number(apa_lcm_data.canInfo.wheel_speed_fr, 'f', 3));
    lineEdit_wheel_speed_rl->setText(QString::number(apa_lcm_data.canInfo.wheel_speed_rl, 'f', 3));
    lineEdit_wheel_speed_rr->setText(QString::number(apa_lcm_data.canInfo.wheel_speed_rr, 'f', 3));
    lineEdit_shift_pos->setText(QString::number(apa_lcm_data.canInfo.shift_pos, 10));
    lineEdit_WRSLDWhlDistPlsCntr->setText(QString::number(apa_lcm_data.canInfo.WRSLDWhlDistPlsCntr, 'f', 3));
    lineEdit_WRSRDWhlDistPlsCntr->setText(QString::number(apa_lcm_data.canInfo.WRSRDWhlDistPlsCntr, 'f', 3));
    lineEdit_WRSLNDWhlDistPCntr->setText(QString::number(apa_lcm_data.canInfo.WRSLNDWhlDistPCntr, 'f', 3));
    lineEdit_WRSRNDWhlDistPCntr->setText(QString::number(apa_lcm_data.canInfo.WRSRNDWhlDistPCntr, 'f', 3));
    lineEdit_timestamp->setText(QString::number(apa_lcm_data.canInfo.timestamp, 10));

    lineEdit_apa_state_id->setText(QString::number(apa_lcm_data.apa_state_id, 10));


    // log file
    if(pushButton_lcm_log->isChecked()==1&&apa_lcm_log_file.is_open())
    {
        apa_lcm_log_file.write(reinterpret_cast<const char*>(&apa_lcm_data), sizeof(ApaLcmData));
    }

    // plot curve
    float cur_x = apa_lcm_data.target_vehicle_pose.coordinate.x;
    float cur_y = apa_lcm_data.target_vehicle_pose.coordinate.y;
    float cur_theta = apa_lcm_data.target_vehicle_pose.fAngle;

    vehicle_pose_x = cur_x;
    vehicle_pose_y = cur_y;
    vehicle_pose_theta = cur_theta;
//     static int count = 0;
//     count++;

//    cur_x = 0.01*count*sin(0.01*count);
//    cur_y = 0.01*count*cos(0.01*count);
//    cur_theta = 0.01*count;//YHL: temp


    if(sqrt((last_plot_x - cur_x)*(last_plot_x - cur_x) + (last_plot_y - cur_y)*(last_plot_y - cur_y))>0.1)
    {
            real_time_pose_curve->addData(cur_y, cur_x);
            last_plot_x = cur_x;
            last_plot_y = cur_y;
            point_count++;
    }
    if(point_count>4096)
    {
        point_count = 0;
        real_time_pose_curve->data().data()->clear();
    }
    vehicle_curve->PlotSlot(cur_x,cur_y,cur_theta);

    apa_plot->replot(QCustomPlot::rpQueuedReplot);
    
}
#endif
    #if 0

void VpldClient::OnApaResultMsg(AvmEvent_ApaResult apa_result)
{
    plainTextEdit_log->clear();
    plainTextEdit_log->insertPlainText("\n========current valid slot num = ");
    plainTextEdit_log->insertPlainText(QString::number(apa_result.APA_ValidSlots.valid_slot_nums, 10));
    plainTextEdit_log->insertPlainText(" ========");


    for(int i=0; i<apa_result.APA_ValidSlots.valid_slot_nums; i++)
    {
        plainTextEdit_log->insertPlainText("\n slot id = ");

        plainTextEdit_log->insertPlainText(QString::number(apa_result.APA_ValidSlots.valid_slots[i].id, 10));

        plainTextEdit_log->insertPlainText(", obj1 pos x = ");
        plainTextEdit_log->insertPlainText(QString::number(apa_result.APA_ValidSlots.valid_slots[i].point[0].x, 'f', 3));

        plainTextEdit_log->insertPlainText(", obj1 pos y = ");
        plainTextEdit_log->insertPlainText(QString::number(apa_result.APA_ValidSlots.valid_slots[i].point[0].y, 'f', 3));

        plainTextEdit_log->insertPlainText(", obj2 pos x = ");
        plainTextEdit_log->insertPlainText(QString::number(apa_result.APA_ValidSlots.valid_slots[i].point[1].x, 'f', 3));

        plainTextEdit_log->insertPlainText(", obj2 pos y = ");
        plainTextEdit_log->insertPlainText(QString::number(apa_result.APA_ValidSlots.valid_slots[i].point[1].y, 'f', 3));

    }
    ApaValidSlots& valid_slots = apa_result.APA_ValidSlots;
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

    apa_plot->replot(QCustomPlot::rpQueuedReplot);



}
#endif

void VpldClient::OnOpenApaButtonClicked()
{
    // if(!lcm_sender.good())
    // {
    //     std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    // }
    // else
    // {
    //     Ctrl_Cmd_T apa_cmd;
    //     apa_cmd.ctrl_status = 20;
    //     apa_cmd.parameter[0] = 1;
    //     lcm_sender.publish("LCM_APA_CMD_MSG", &apa_cmd, sizeof(Ctrl_Cmd_T));

    // }

}
void VpldClient::OnCloseApaButtonClicked()
{
    // if(!lcm_sender.good())
    // {
    //     std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    // }
    // else
    // {
    //     Ctrl_Cmd_T apa_cmd;
    //     apa_cmd.ctrl_status = 20;
    //     apa_cmd.parameter[0] = 2;
    //     lcm_sender.publish("LCM_APA_CMD_MSG", &apa_cmd, sizeof(Ctrl_Cmd_T));

    // }
}
void VpldClient::OnSetSlotButtonClicked()
{
    // if(!lcm_sender.good())
    // {
    //     std::cout<<"lcm sender is not ok, can not send any message right now!"<<std::endl;
    // }
    // else
    // {
    //     Ctrl_Cmd_T apa_cmd;
    //     apa_cmd.ctrl_status = 21;
    //     apa_cmd.parameter[0] = spinBox_target_slot_id->value();
    //     lcm_sender.publish("LCM_APA_CMD_MSG", &apa_cmd, sizeof(Ctrl_Cmd_T));

    // }
}

void VpldClient::OnLcmLogButtonClicked()
{
    // std::cout<<"clicked, checkbale="<< pushButton_lcm_log->isCheckable()<<"checked="<< pushButton_lcm_log->isChecked()<<std::endl;
    // if(pushButton_lcm_log->isChecked()==1)
    // {
    //     // creat log file
    //     QDateTime time = QDateTime::currentDateTime();
    //     std::cout<<time.toString("yyyy_MM_dd_HH_mm_ss").toStdString()<<std::endl;

    //     apa_lcm_log_file.open(reinterpret_cast<const char*>((time.toString("yyyy-MM-dd_HH-mm-ss").toStdString()+"_apa_lcm_data.bin").c_str()), std::ios::binary);
    //     pushButton_lcm_log->setText(QWidget::tr("StopLog"));
    // }
    // else
    // {
    //     // close log file
    //     apa_lcm_log_file.close();
    //     pushButton_lcm_log->setText(QWidget::tr("StartLog"));
    // }
}

void VpldClient::OnVideoRecordButtonClicked()
{
    std::cout<<"OnVideoRecordButtonClicked, checkbale="<< pushButton_video_record->isCheckable()<<"checked="<< pushButton_video_record->isChecked()<<std::endl;
    if(pushButton_video_record->isChecked()==1)
    {
        // creat log file
        QDateTime time = QDateTime::currentDateTime();
        std::cout<<time.toString("yyyy_MM_dd_HH_mm_ss").toStdString()<<std::endl;
        std::string file_name = time.toString("yyyy-MM-dd_HH-mm-ss").toStdString()+"_vpld_detector.avi";
        if(video_record.isOpened())
        {
            std::cout<<"video record has been opened, will be closed."<<std::endl;
            video_record.release();
        }
        video_record.open(file_name, cv::CAP_OPENCV_MJPEG, 12.0, cv::Size(500, 500), true);
        if(video_record.isOpened())
        {
            std::cout<<file_name<<" open ok"<<std::endl;
            pushButton_video_record->setText(QWidget::tr("StopRec"));
        }
        else
        {
            std::cout<<file_name<<" open failed."<<std::endl;
            pushButton_video_record->setChecked(false);
            
        }
        
    }
    else
    {
        // close log file
        if(video_record.isOpened())
        {
            video_record.release();
        }
        pushButton_video_record->setText(QWidget::tr("StartRec"));
    }   





}
void VpldClient::OnVpldResultMsg(VpldResult vpld_result)
{
    std::cout<<"on vpld result msg!"<<std::endl;

        // memcpy(freespace_img_nv12, freespace_result, 224*224*3/2);
    // sws_scale(sws_nv12_rgb32, pFrame->data, pFrame->linesize, 0, 224, pFrameRGB32->data, pFrameRGB32->linesize);
    // QImage image = QImage((uchar *)vpld_result, 400, 200, QImage::Format_RGB888).rgbSwapped();
    // mImage->setPixmap(QPixmap::fromImage(image));
    // mImage->setScaled(false);
    // mImage->setScaled(true,Qt::IgnoreAspectRatio);
    // apa_plot->replot(QCustomPlot::rpQueuedReplot);
    // std::cout<<"OnFreespaceResultMsg"<<std::endl;

    for(int k=0; k<OBJ_MAX_NUM; k++)
    {
        if(vpld_result.obj_sup_flag[k] == 0)
        {
            float top_left_x = vpld_result.object_raw_data[7*k+3]*400-200;
            float top_left_y = -vpld_result.object_raw_data[7*k+4]*200+100;
            float bot_right_x = vpld_result.object_raw_data[7*k+5]*400-200;
            float bot_right_y = -vpld_result.object_raw_data[7*k+6]*200+100;
            QVector<QCPCurveData> object_box_point(5);
            object_box_point[0] = QCPCurveData(0, top_left_x, top_left_y);
            object_box_point[1] = QCPCurveData(1, bot_right_x, top_left_y);
            object_box_point[2] = QCPCurveData(2, bot_right_x, bot_right_y);
            object_box_point[3] = QCPCurveData(3, top_left_x, bot_right_y);
            object_box_point[4] = QCPCurveData(4, top_left_x, top_left_y);
            mp_object_box[k]->PlotBox(object_box_point);
        }
        else
        {
            mp_object_box[k]->ClearPlot();

        }

    }

    for(int k=0; k<vpld_result.slot_num; k++)
    {
        // std::cout<<"slot id="<<vpld_result.slot[k].id<<std::endl;
        QVector<QCPCurveData> slot_curve_point(5);
        for(int i=0; i<4; i++)
        {
            // std::cout<<"corner_"<<i<<"v_x="<<vpld_result.slot[k].corner[i].point_v.x<<" v_y"<<vpld_result.slot[k].corner[i].point_v.y<<std::endl;
            // std::cout<<"corner_"<<i<<"p_x="<<vpld_result.slot[k].corner[i].pixel.x<<" p_y"<<vpld_result.slot[k].corner[i].pixel.y<<std::endl;

            slot_curve_point[i] = QCPCurveData(i, vpld_result.slot[k].corner[i].pixel.x-200, -vpld_result.slot[k].corner[i].pixel.y+100);
        }
        slot_curve_point[4] = slot_curve_point[0];

        mp_detected_slot_curves[k]->PlotSlot(slot_curve_point, vpld_result.slot[k].inner_obstacle);

        mp_detected_slot_desc[k]->position->setCoords(slot_curve_point[0].key, slot_curve_point[0].value);
        QString slot_desc = QString::number(vpld_result.slot[k].id, 10);
        if(vpld_result.slot[k].type == 0)
        {
            mp_detected_slot_desc[k]->setText(slot_desc+"V");
        }
        else if(vpld_result.slot[k].type == 1)
        {
            mp_detected_slot_desc[k]->setText(slot_desc+"P");
        }
        else if(vpld_result.slot[k].type == 2)
        {
            mp_detected_slot_desc[k]->setText(slot_desc+"I");

        }
        else
        {
            mp_detected_slot_desc[k]->setText(slot_desc+"?");
        }

        mp_detected_slot_desc[k]->setVisible(true);


    }
    for(int k=vpld_result.slot_num;k<24;k++)
    {

        mp_detected_slot_curves[k]->ClearPlot();
        mp_detected_slot_desc[k]->setVisible(false);

    }

    if(vpld_result.vpld_state == 2)
    {
        QVector<QCPCurveData> slot_curve_point(5);
        for(int i=0; i<4; i++)
        {
            slot_curve_point[i] = QCPCurveData(i, vpld_result.target_slot.corner[i].pixel.x-200, -vpld_result.target_slot.corner[i].pixel.y+100);
        }
        slot_curve_point[4] = slot_curve_point[0];

        mp_target_slot_curves->PlotSlot(slot_curve_point, vpld_result.target_slot.inner_obstacle);

        mp_target_slot_desc->position->setCoords(slot_curve_point[0].key, slot_curve_point[0].value);

        mp_target_slot_desc->setText("TargetSlot");

        mp_target_slot_desc->setVisible(true);

    }
    else
    {
        mp_target_slot_curves->ClearPlot();
        mp_target_slot_desc->setVisible(false);

    }
    

}

void VpldClient::OnVpldBevImageMsg(unsigned char* vpld_bev_image)
{
    std::cout<<"on vpld bev image msg!"<<std::endl;

    QImage image = QImage((uchar *)vpld_bev_image, 400, 200, QImage::Format_RGB888).rgbSwapped();
    mImage->setPixmap(QPixmap::fromImage(image));
    mImage->setScaled(false);
    mImage->setScaled(true,Qt::IgnoreAspectRatio);
    apa_plot->replot(QCustomPlot::rpQueuedReplot);
    // std::cout<<"OnFreespaceResultMsg"<<std::endl;
    // cv::Mat frame(200, 400,  16, vpld_bev_image);
    if(video_record.isOpened()&&pushButton_video_record->isChecked()==1)
    {
        QPixmap pixmap = apa_plot->toPixmap(500,500);
        QImage qimage = pixmap.toImage();
        std::cout<<"qimage format="<<qimage.format()<<std::endl;
        cv::Mat frame = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (void*)qimage.constBits(), qimage.bytesPerLine());
        cv::cvtColor(frame, frame, cv::COLOR_BGRA2BGR);
        video_record<<frame;
    }

}