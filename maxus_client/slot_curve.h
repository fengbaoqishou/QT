#ifndef SLOT_CURVE_H_
#define SLOT_CURVE_H_

class SlotCurve
{
public:
    SlotCurve(QCustomPlot* p_plot)
    {
        mp_plot = p_plot;
        mp_curve = new QCPCurve(mp_plot->xAxis, mp_plot->yAxis);
        mp_curve->setPen(QPen(Qt::blue));
        mp_curve->setBrush(QBrush(QColor(0, 0, 255, 20)));
        mp_curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusSquare ));
    }
    void PlotSlot(QVector<QCPCurveData>& curve_point)
    {
        mp_curve->data()->set(curve_point, true);
    }
    void PlotSlot(QVector<QCPCurveData>& curve_point, int obstacle_type)
    {
        if(obstacle_type == 0)
        {
            mp_curve->setPen(QPen(Qt::green));
            mp_curve->setBrush(QBrush(QColor(0, 255, 0, 20)));
        }
        else
        {
            mp_curve->setPen(QPen(Qt::red));
            mp_curve->setBrush(QBrush(QColor(255, 0, 0, 20)));
        }
        
        mp_curve->data()->set(curve_point, true);
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
};

class ObjectBox
{
public:
    ObjectBox(QCustomPlot* p_plot)
    {
        mp_plot = p_plot;
        mp_curve = new QCPCurve(mp_plot->xAxis, mp_plot->yAxis);
        mp_curve->setPen(QPen(Qt::blue));
        mp_curve->setBrush(QBrush(QColor(0, 0, 255, 20)));
        mp_curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusSquare ));
    }
    void PlotBox(QVector<QCPCurveData>& curve_point)
    {
        mp_curve->data()->set(curve_point, true);
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
};
#endif