#ifndef VPLD_CLIENT_H_
#define VPLD_CLIENT_H_

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
#include <lcm/lcm-cpp.hpp>

#include "qcustomplot.h"

#include "vehicle_curve.h"
#include "slot_curve.h"
#include "vpld_data_type.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class VpldClient:public QWidget
{
    Q_OBJECT

public:
    explicit VpldClient(QWidget *parent = 0);
    ~VpldClient();
    QWidget* GetWidget() const;
    QHBoxLayout* GetLayout() const;
private slots:
    // void OnApaResult(ApaLcmData apa_lcm_data);
    // void OnApaResultMsg(AvmEvent_ApaResult apa_result);
    void OnVpldResultMsg(VpldResult vpld_result);
    void OnOpenApaButtonClicked();
    void OnCloseApaButtonClicked();
    void OnSetSlotButtonClicked();
    void OnLcmLogButtonClicked();
    void OnVpldBevImageMsg(unsigned char* vpld_bev_image);
    void OnVideoRecordButtonClicked();

private:
    QCustomPlot* apa_plot;
    QPlainTextEdit* plainTextEdit_log;
    //line edit
    QLineEdit* lineEdit_steering_angle;
    QLineEdit* lineEdit_vehicle_speed;
    QLineEdit* lineEdit_yaw_rate;
    QLineEdit* lineEdit_wheel_speed_fl;
    QLineEdit* lineEdit_wheel_speed_fr;
    QLineEdit* lineEdit_wheel_speed_rl;
    QLineEdit* lineEdit_wheel_speed_rr;
    QLineEdit* lineEdit_shift_pos;
    QLineEdit* lineEdit_WRSLDWhlDistPlsCntr;
    QLineEdit* lineEdit_WRSRDWhlDistPlsCntr;
    QLineEdit* lineEdit_WRSLNDWhlDistPCntr;
    QLineEdit* lineEdit_WRSRNDWhlDistPCntr;
    QLineEdit* lineEdit_timestamp;

    QLineEdit* lineEdit_apa_state_id;

    //QLabel
    QLabel* label_steering_angle;
    QLabel* label_vehicle_speed;
    QLabel* label_yaw_rate;
    QLabel* label_wheel_speed_fl;
    QLabel* label_wheel_speed_fr;
    QLabel* label_wheel_speed_rl;
    QLabel* label_wheel_speed_rr;
    QLabel* label_shift_pos;

    QLabel* label_WRSLDWhlDistPlsCntr;
    QLabel* label_WRSRDWhlDistPlsCntr;
    QLabel* label_WRSLNDWhlDistPCntr;
    QLabel* label_WRSRNDWhlDistPCntr;
    QLabel* label_timestamp;
    QLabel* label_apa_state_id;

    //spinbox
    QSpinBox* spinBox_target_slot_id;

    QPushButton* pushButton_open_apa;
    QPushButton* pushButton_close_apa;
    QPushButton* pushButton_set_target_slot;
    QPushButton* pushButton_lcm_log;
    QPushButton* pushButton_video_record;

    // layout
    QHBoxLayout* layout_apa_client;
    QVBoxLayout* layout_apa_client_left;
    QVBoxLayout* layout_apa_client_right;

    QGridLayout* layout_groupBox_input_info;
    QGridLayout* layout_groupBox_control_info;
    QGridLayout* layout_groupBox_output_info;

    QGroupBox* groupBox_input_info;
    QGroupBox* groupBox_control_info;
    QGroupBox* groupBox_output_info;

    QWidget* apa_client_widget;

    lcm::LCM lcm_sender;
    std::ofstream apa_lcm_log_file;
    QCPCurve* real_time_pose_curve;
    SlotCurve* mp_detected_slot_curves[24];
    QCPItemText* mp_detected_slot_desc[24];

    SlotCurve* mp_target_slot_curves;
    QCPItemText* mp_target_slot_desc;

    VehicleCurve* vehicle_curve;
    float last_plot_x;
    float last_plot_y;
    int point_count;
    float vehicle_pose_x;
    float vehicle_pose_y;
    float vehicle_pose_theta;
    QCPItemPixmap* mImage;

    ObjectBox* mp_object_box[20];
    cv::VideoWriter video_record;
};




#endif