#include "lcmthread.h"
#include "zmq.h"

#include <iostream>
#if 1
LcmThread* LcmThread::s_lcm_thread = NULL;
LcmThread* LcmThread::GetInstance()
{
    if(s_lcm_thread == NULL)
        s_lcm_thread = new LcmThread;
    return s_lcm_thread;
}
#endif


// exlcm::example_t g_msg;

zmq_msg_t msg;
// zmq_msg_t msg;
// zmq_msg_init_data(&msg,sizeof(g_vmp));
void *context = zmq_ctx_new();
void *subscriber = zmq_socket(context, ZMQ_SUB);
int rc = zmq_connect(subscriber, "tcp://localhost:5555");
int rb = zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,"",0);


//zmq_msg_init_size(g_vmp);

//char p2p_raw_data[256];
P2pLcmData p2p_data;
ApaValidSlots apa_pld_result;
ApaLcmData apa_lcm_data;
AvmEvent_ApaResult apa_result;

unsigned char freespace_nv12_result[224*224*3/2];
unsigned char vpld_bev_image[400*200*3];
VpldResult g_vpld_result;
float32_t  g_vdr_pose[3];
VehicleDeadReckoning g_vdr;
VehicleModelParam g_vmp;


#if 0
class Handler {
  public:
    ~Handler() {}
    void handleMessage(const lcm::ReceiveBuffer *rbuf, const std::string &chan,
                       const exlcm::example_t *msg)
    {
        memcpy(&g_msg, msg, sizeof(exlcm::example_t));
        int i;
        std::cout<<"Received message on channel"<< chan<<std::endl;
        printf("  timestamp   = %lld\n", (long long) msg->timestamp);
        printf("  position    = (%f, %f, %f)\n", msg->position[0], msg->position[1],
               msg->position[2]);
        printf("  orientation = (%f, %f, %f, %f)\n", msg->orientation[0], msg->orientation[1],
               msg->orientation[2], msg->orientation[3]);
        printf("  ranges:");
        for (i = 0; i < msg->num_ranges; i++)
            printf(" %d", msg->ranges[i]);
        printf("\n");
        printf("  name        = '%s'\n", msg->name.c_str());
        printf("  enabled     = %d\n", msg->enabled);
    }

    void handleP2pMsg(const lcm::ReceiveBuffer *rbuf, const std::string &chan)
    {
        P2pLcmData* lcm_data = (P2pLcmData*)rbuf->data;
        memcpy(&p2p_data, rbuf->data, sizeof(P2pLcmData));
        LcmThread::GetInstance()->emitP2pLcmMsgReceived(p2p_data);
        //std::cout << "get a new raw p2p msg, timestamp=" <<lcm_data->timestamp<< std::endl;
    }
    void handlePldResult(const lcm::ReceiveBuffer *rbuf, const std::string &chan)
    {
        ApaValidSlots* lcm_data = (ApaValidSlots*)rbuf->data;
        memcpy(&apa_pld_result, rbuf->data, sizeof(ApaValidSlots));
        LcmThread::GetInstance()->emitApaPldResultReceived(apa_pld_result);
//        std::cout << "get a new pld" << apa_pld_result.valid_slot_nums<<std::endl;
    }
    void handleApaDebug(const lcm::ReceiveBuffer *rbuf, const std::string &chan)
    {
        ApaLcmData* lcm_data = (ApaLcmData*)rbuf->data;
        memcpy(&apa_lcm_data, rbuf->data, sizeof(ApaLcmData));
        LcmThread::GetInstance()->emitApaResultReceived(apa_lcm_data);
    }

    void handleApaResult(const lcm::ReceiveBuffer *rbuf, const std::string &chan)
    {
        AvmEvent_ApaResult* lcm_data = (AvmEvent_ApaResult*)rbuf->data;
        memcpy(&apa_result, rbuf->data, sizeof(AvmEvent_ApaResult));
        LcmThread::GetInstance()->emitApaResultMsgReceived(apa_result);
    }
    void handleFreespaceResult(const lcm::ReceiveBuffer *rbuf, const std::string &chan)
    {
        unsigned char* lcm_data = (unsigned char*)rbuf->data;
        memcpy(freespace_nv12_result, rbuf->data, 224*224*3/2);
        LcmThread::GetInstance()->emitFreespaceResultMsgReceived(freespace_nv12_result);

    }
    void handleVpldResult(const lcm::ReceiveBuffer *rbuf, const std::string &chan)
    {
        unsigned char* lcm_data = (unsigned char*)rbuf->data;
        memcpy(&g_vpld_result, rbuf->data, sizeof(VpldResult));
        LcmThread::GetInstance()->emitVpldResultMsgReceived(g_vpld_result);
        std::cout<<"get vpld result lcm data!"<<std::endl;

    }
    void handleVpldBevImage(const lcm::ReceiveBuffer *rbuf, const std::string &chan)
    {
        unsigned char* lcm_data = (unsigned char*)rbuf->data;
        memcpy(vpld_bev_image, rbuf->data, 400*200*3);
        LcmThread::GetInstance()->emitVpldBevImageMsgReceived(vpld_bev_image);
        std::cout<<"get vpld bev image lcm data!"<<std::endl;

    }
    void handleVdrVehiclePose(const lcm::ReceiveBuffer *rbuf, const std::string &chan)
    {
        unsigned char* lcm_data = (unsigned char*)rbuf->data;
        memcpy(&g_vdr, rbuf->data, sizeof(VehicleDeadReckoning));
        std::cout<<"get vdr vehicle pose lcm data!"<<g_vdr.current_input.timestamp<<" "<<g_vdr.global_pose.x<<" "<<g_vdr.global_pose.y<<" "<<g_vdr.global_pose.theta<<std::endl;
        LcmThread::GetInstance()->emitVdrVehiclePoseReceived(g_vdr);

    }
    void handleVehicleModelParam(const zmq_msg_data(&msg))
    {
        memcpy(&g_vmp,zmq_msg_data(&msg),sizeof(VehicleModelParam));
        std::cout<<"get vehicle model Param!"<<std::endl;
        LcmThread::GetInstance()->emitVehicleModelParamReceived(g_vmp);
    }
};

#endif

LcmThread::LcmThread(QObject *parent)
    : QThread(parent)
{   
#if 0
    if(!lcm.good())
    {
        std::cout<<"lcm not good"<<std::endl;
    }
    Handler handlerObject;

    //lcm.subscribe("EXAMPLE", &Handler::handleMessage, &handlerObject);

    lcm.subscribe("LCM_P2P_DEBUG_MSG", &Handler::handleP2pMsg, &handlerObject);
    lcm.subscribe("LCM_APA_PLD_RESULT", &Handler::handlePldResult, &handlerObject);
    lcm.subscribe("LCM_APA_DEBUG_MSG", &Handler::handleApaDebug, &handlerObject);
    lcm.subscribe("LCM_APA_RESULT_MSG", &Handler::handleApaResult, &handlerObject);
    lcm.subscribe("LCM_FREESPACE_RESULT_MSG", &Handler::handleFreespaceResult, &handlerObject);
    lcm.subscribe("LCM_VPLD_RESULT", &Handler::handleVpldResult, &handlerObject);
    lcm.subscribe("LCM_VPLD_BEV_IMAGE", &Handler::handleVpldBevImage, &handlerObject);
    lcm.subscribe("LCM_VDR_RESULT", &Handler::handleVdrVehiclePose, &handlerObject);

#endif
    // std::cout<<"lcm construct finished"<<std::endl;
    // if (!isRunning()) {
    //     start(LowPriority);}
//    restart = false;
//    abort = false;

//    for (int i = 0; i < ColormapSize; ++i)
//        colormap[i] = rgbFromWaveLength(380.0 + (i * 400.0 / ColormapSize));
}



LcmThread::~LcmThread()
{
//    mutex.lock();
//    abort = true;
//    condition.wakeOne();
//    mutex.unlock();

//    wait();
}

void LcmThread::run()
{
    // std::cout<<"lcm thread start run"<<std::endl;
    // std::cout<<"sizeof(ApaValidSlots)="<<sizeof(ApaSlot)<<std::endl;
    std::cout<<"sizeof(VehicleModelParam)="<<sizeof(VehicleModelParam)<<std::endl;

#if 0
    while (0 == lcm.handle())
    {
//        emit ReceivedNewLcmMsg(g_msg);
//        emit P2pLcmMsgReceived(p2p_data);
        //std::cout<<"lcm posx="<<p2p_data.pos_x<<"posy="<<p2p_data.pos_y<<std::endl;
        // Do nothing
    }
#endif
#if 1
    while(1)
    {
        if(zmq_msg_recv(&msg,subscriber,0) == -1)
        {
            printf("recv was error\n");
            // return -1;
        }
        printf("jieshoushuju\n");
        // VehicleModelParamReceived(zmq_msg_data(&msg));
        memcpy(&msg,zmq_msg_data(&msg),sizeof(VehicleModelParam));
        std::cout<<"get vehicle model Param!"<<std::endl;
        LcmThread::GetInstance()->emitVehicleModelParamReceived(g_vmp);
        // handle::handleVehicleModelParam(const zmq_msg_data(&msg));
        //handle::handleVdrVehiclePose();
    }
}
#endif

