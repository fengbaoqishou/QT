#ifndef LCMTHREAD_H
#define LCMTHREAD_H
#include <QThread>
#include <lcm/lcm-cpp.hpp>
#include "example_t.hpp"
#include "p2p_lcm_data.h"
#include "apa_data_type.h"
#include "vpld_data_type.h"
#include  "zmq.h"
class LcmThread : public QThread
{
    Q_OBJECT

public:
    LcmThread(QObject* parent = 0);
    ~LcmThread();
    static LcmThread* GetInstance();
    void emitApaPldResultReceived(ApaValidSlots& pld_result)
    {
        emit ApaPldResultReceived(pld_result);
    }

    void emitP2pLcmMsgReceived(P2pLcmData& p2p_data)
    {
        emit P2pLcmMsgReceived(p2p_data);
    }
    void emitApaResultReceived(ApaLcmData& apa_lcm_data)
    {
        emit ApaResultReceived(apa_lcm_data);
    }

    void emitApaResultMsgReceived(AvmEvent_ApaResult& apa_result)
    {
        emit ApaResultMsgReceived(apa_result);
    } 

    void emitFreespaceResultMsgReceived(unsigned char* freespace_result)
    {
        emit FreespaceResultMsgReceived(freespace_result);
    }
    void emitVpldResultMsgReceived(VpldResult vpld_result)
    {
        emit VpldResultMsgReceived(vpld_result);
    }
    void emitVpldBevImageMsgReceived(unsigned char* vpld_bev_image)
    {
        emit VpldBevImageMsgReceived(vpld_bev_image);
    }
    void emitVdrVehiclePoseReceived(VehicleDeadReckoning vdr)
    {
        emit VdrVehiclePoseReceived(vdr);
    }
    void emitVehicleModelParamReceived(VehicleModelParam vmp)
    {
        emit VehicleModelParamReceived(vmp);
    }

signals:
    void ReceivedNewLcmMsg(exlcm::example_t msg);
    void P2pLcmMsgReceived(P2pLcmData p2p_data);
    void ApaPldResultReceived(ApaValidSlots pld_result);
    void ApaResultReceived(ApaLcmData apa_lcm_data);
    void ApaResultMsgReceived(AvmEvent_ApaResult apa_result);
    void FreespaceResultMsgReceived(unsigned char* freespace_result);
    void VpldResultMsgReceived(VpldResult vpld_result);
    void VpldBevImageMsgReceived(unsigned char* vpld_bev_image);
    void VdrVehiclePoseReceived(VehicleDeadReckoning vdr);
    void VehicleModelParamReceived(VehicleModelParam vmp);

protected:
    void run() Q_DECL_OVERRIDE;
private:
    static LcmThread* s_lcm_thread;
    // static LcmThread* s_zmq_thread;
    lcm::LCM lcm;


};

#endif // LCMTHREAD_H
