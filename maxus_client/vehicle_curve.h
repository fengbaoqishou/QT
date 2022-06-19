#ifndef VEHICLE_CURVE_H_
#define VEHICLE_CURVE_H_
#include <math.h>
#include <QVector>
class VehicleCurve
{
public:
    VehicleCurve(QCustomPlot* p_plot)
    {
        mp_plot = p_plot;
        mp_curve = new QCPCurve(mp_plot->xAxis, mp_plot->yAxis);
        mp_curve->setPen(QPen(Qt::darkRed));
        mp_curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc ));

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


    }
    void PlotSlot(QVector<QCPCurveData>& curve_point)
    {
        mp_curve->data()->set(curve_point, true);
    }

    void PlotSlot(float vehicle_pose_x, float vehicle_pose_y, float vehicle_pose_theta)
    {
        QVector<QCPCurveData> vehicle_curve_point(10);
        for(int i=0; i<10; i++)
        {
            vehicle_curve_point[i] = QCPCurveData(i,vehicle_curve_point_x[i]*sin(vehicle_pose_theta)+vehicle_curve_point_y[i]*cos(vehicle_pose_theta)+vehicle_pose_y, vehicle_curve_point_x[i]*cos(vehicle_pose_theta)-vehicle_curve_point_y[i]*sin(vehicle_pose_theta)+vehicle_pose_x);
        }

        mp_curve->data()->set(vehicle_curve_point, true);
    }

    void ClearPlot()
    {
        if(mp_curve != NULL)
            mp_curve->data().data()->clear();
    }


private:
    QCPCurve* mp_curve;
    QCustomPlot* mp_plot;
    QVector<QCPCurveData>* mp_curve_point;
    float vehicle_curve_point_x[10];
    float vehicle_curve_point_y[10];
    float vehicle_width;
    float vehicle_length;
    float rear_wheel_to_tail;
};
#endif