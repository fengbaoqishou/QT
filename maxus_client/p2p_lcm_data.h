#ifndef P2P_LCM_DATA_H_
#define P2P_LCM_DATA_H_
#if 0
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;
#endif
typedef float float32_t;

typedef struct PathHead_
{
    uint8_t  path_num;
    uint8_t  path_valid_point;
    uint16_t path_date_year;

    uint8_t  path_date_month;
    uint8_t  path_date_day;
    uint8_t  path_date_hour;
    uint8_t  path_date_min;

    uint8_t  path_date_sec;
    uint8_t  rsv[3];
}PathHead;

typedef struct PathPoint_
{
    float x;
    float y;
    uint32_t  gear_shift;
    float theta;
    uint32_t  rsv;
}PathPoint;

typedef struct PathData_
{
    PathHead path_head;
    PathPoint path_data[500];
}PathData;

typedef struct
{
	uint16_t MsgType;
	uint16_t MsgSize;

}Ofilm_MsgHead;
typedef struct
{
    Ofilm_MsgHead MsgHead;

    uint32_t ctrl_status;
    uint32_t parameter[4];

}Ctrl_Cmd_T;
typedef struct SlotPoint_
{
        float x;
        float y;
}SlotPoint;
typedef struct ApaSlot_
{
        int16_t id;
        int8_t slot_type;
        uint8_t location;
        int8_t path_num;
        SlotPoint point[4];
}ApaSlot;
typedef struct ApaValidSlots_
{
        unsigned int valid_slot_nums;
        ApaSlot valid_slots[24]; //max slot nums=24
        int16_t default_slot_id;
        uint16_t rsv;
}ApaValidSlots;

typedef struct ApaOutPutSignal_
{
	uint8_t req_shake_hand_signal;
	uint8_t req_continue_parking_in_signal;
	uint8_t req_continue_parking_out_signal;
	uint8_t rsv;
}ApaOutPutSignal;

typedef struct
{
	Ofilm_MsgHead MsgHead;
	uint8_t	 APA_ConnectReq;    /**< 0:False, 1:True */
	uint8_t APA_EmergencyBrakeReq;	/**< 0:NnRequest, 1:EmergencyBrake, 2:ConfortBrake*/
	uint8_t  APA_TgtGearReq;    /**< 0:P, 1:R, 2:N, 3:D, 7:Invalid */
	uint16_t APA_StopDistance;  /**< unit: cm , factor: 1*/    
	float32_t APA_SpdLimitReq;   /**< unit: km/s, factor: 0.1 */
	float32_t APA_WheelAngleReq; /**< '-':clockwise, '+':anti-clockwise, factor: 0.1 */
	
	ApaValidSlots APA_ValidSlots; //to be displayed
    uint16_t APA_ParkoutVaildDir;   //vertical slot(front=1; rear=2); horizontal slot(left=3; right=4; left_and_right=5);

	ApaOutPutSignal Apa_OutPutSignal; //apa output signal
	uint8_t rsv;

    	//VP APA Output
	// target slot info
    float32_t APA_target_slot_obj2_pose_x;//factor 1000
    float32_t APA_target_slot_obj2_pose_y;//factor 1000
    float32_t APA_target_slot_sublane_objX_theta;//factor 1000
    float32_t APA_target_slot_end_pose_x;//factor 1000
    float32_t APA_target_slot_end_pose_y;//factor 1000
    // locked slot info
    float32_t APA_locked_slot_end_pose_x;//factor 1000
	float32_t APA_locked_slot_end_pose_y;//factor 1000
    float32_t APA_locked_slot_end_pose_angle;//factor 1000

    float32_t APA_locked_slot_obj1_pose_x;//factor 1000
    float32_t APA_locked_slot_obj1_pose_y;//factor 1000
    float32_t APA_locked_slot_obj2_pose_x;//factor 1000
    float32_t APA_locked_slot_obj2_pose_y;//factor 1000
    uint8_t APA_locked_slot_obj1_detected;
    uint8_t APA_locked_slot_obj2_detected;
    uint8_t APA_locked_slot_type;

    uint8_t APA_parking_path_point_num;
    uint8_t APA_vp_apa_failure_status;
    uint8_t APA_vp_apa_sm_state;
    //path plan info
    uint8_t APA_parking_path_plan_status;
    uint8_t APA_parking_path_transit_status;
    uint8_t APA_parking_path_plan_count; //init:0, new path:+1
    uint8_t APA_is_parking_slot_locked;  //init:0, locked:1
    // parking out
    float32_t APA_parking_out_end_pose_x;//factor 1000
    float32_t APA_parking_out_end_pose_y;//factor 1000
    float32_t APA_parking_out_end_pose_theta;//factor 1000
}AvmEvent_ApaResult;

typedef struct P2pLcmData_
{

	union
	{
                char raw_data[256];
		struct
		{
			Ofilm_MsgHead  MsgHead;

			// input can data
			float steering_angle; //StrWhAng -2048~2048   deg
			float vehicle_speed; //VehSpdAvgDrvn
			float yaw_rate; //VehDynYawRate -127~127 deg/sec
			float wheel_speed_fl; //
			float wheel_speed_fr; //
			float wheel_speed_rl; //WhlGrndVlctyLtNnDrvn  km/h
			float wheel_speed_rr; //WhlGrndVlctyRtNnDrvn  km/h
			int shift_pos; //VehMovState //0:parked; 0x1:Neutral; 0x2: Forward; 0x3 Rward; 0x4 Invalid
			bool door_sts_fl; // DDAjirSwAtv
			bool door_sts_fr; // PAjirSwAtv
			bool door_sts_rl; //RRDoorAjarSwAtct
			bool door_sts_rr; //RLDoorAjarSwAtct
			unsigned char left_turn_sig;
			unsigned char right_turn_sig;
			unsigned char brake_act_flag;
			int HighBmAct;
			int LowBmAct;
			//NEW ADD :shenyujie 2017-05-04
			float    radar_rear_left_side_distance;//各雷达距离信息
			float    radar_rear_left_coner_distance;
			float    radar_rear_left_middle_distance;
			float    radar_rear_right_side_distance;
			float    radar_rear_right_coner_distance;
			float    radar_rear_right_middle_distance;
			float    radar_front_left_side_distance;
			float    radar_front_left_coner_distance;
			float    radar_front_left_middle_distance;
			float    radar_front_right_side_distance;
			float    radar_front_right_coner_distance;
			float    radar_front_right_middle_distance;
			int WRSLDWhlDistPlsCntr;//左前轮转动脉冲
			int WRSRDWhlDistPlsCntr;//右前轮转动脉冲
			int WRSLNDWhlDistPCntr;//左后轮转动脉冲
			int WRSRNDWhlDistPCntr;//右后轮转动脉冲
			int Electrical_Power_Steering_Availability_Status;//方向盘是否被司机控制
			uint64_t timestamp;


			uint8_t working_state;
			uint8_t system_state;
			uint8_t failure_state;
			uint8_t route_save_percentage;

			uint8_t route_learning_state;
			uint8_t route1_save_result;
			uint8_t route1_delete_result;
			uint8_t route2_save_result;

			uint8_t route2_delete_result;
			uint8_t radar_obs_state;
			uint8_t route_plan_state;
			uint8_t reloc_slam_state;


			uint8_t rout1_exist_state;
			uint8_t rout2_exist_state;
			uint8_t route_replay_state;


			uint32_t mod_x;
			uint32_t mod_y;
			uint16_t mod_dist;
			uint8_t  a_point_available_result;
			uint8_t  pos_valid;

			float pos_x;
			float pos_y;
			float pos_theta;
                        float dist2path;
                        float angle2path;
                        float dist2pointB;
                        float angle2pointB;
                        float lengthAB;
		};
	};


}P2pLcmData;

#endif
