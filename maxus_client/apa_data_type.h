#ifndef APA_DATA_TYPE_H_
#define APA_DATA_TYPE_H_

#define  PldVisionMaxNum   12
#define SONAR_PLD_SLOT_MAX_NUM (6U)
typedef float float32_t;
typedef int16_t	APA_DISTANCE_TYPE;
typedef int32_t APA_INT_TYPE;
typedef float APA_SPEED_TYPE;
typedef uint16_t APA_SPEED_WORD_TYPE;
typedef float	APA_SPEED_CAL_FLOAT_TYPE;
typedef float32_t APA_ANGLE_TYPE;
typedef uint16_t APA_MS_TIME_TYPE;
typedef int16_t APA_TIMES_TYPE;

typedef float32_t APA_S_TIME_CAL_FLOAT_TYPE;
typedef int8_t APA_ENUM_TYPE;
typedef float32_t APA_RATIO_TYPE;
typedef int32_t APA_DISTANCE_CAL_INT_TYPE;
typedef float32_t	APA_DISTANCE_CAL_FLOAT_TYPE;
typedef float32_t	APA_ANGLE_CAL_FLOAT_TYPE;
typedef int8_t APA_INDEX_TYPE;
typedef float32_t APA_CAL_FLOAT_TYPE;
typedef int32_t APA_CAL_INT_TYPE;
typedef uint8_t APA_SPEAKER_CHIME_DELAY_TIMER_TYPE;
typedef int16_t APA_WEIGHT_COEFFICIENT_TYPE;
typedef uint16_t SIA_ERROR_FLAG_TYPE;
typedef uint16_t APA_CAR_POS_WHEEL_PULSE_CNT_TYPE;
// typedef bitfield32_t APA_CAR_POS_WHEEL_PULSE_CNT_CAL_TYPE;
typedef int16_t APA_SLOT_ID_TYPE;
typedef int8_t APA_SLOT_PT_SLOPE_TYPE;
typedef int16_t APA_SLOT_PT_SLOPE_CAL_TYPE;
typedef uint8_t BOOLEAN;

typedef struct COMMON_VEHICLE_DATA
{
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
    unsigned char emergency_light_status;

    int RrFogLmpsAct;
    int FrFogLmpsAct;

    int fun1_switch_state;
    int fun2_switch_state;

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

    //int   VCU_reserved_int_0;//保留位
    //int   VCU_reserved_int_1;//保留位
    uint64_t timestamp;

    // added by YHL
    unsigned short int    fl_tire_pressure; // YHL: unit=kPa [0:508]
    unsigned short int    fr_tire_pressure; // YHL: unit=kPa [0:508]
    unsigned short int    rl_tire_pressure; // YHL: unit=kPa [0:508]
    unsigned short int    rr_tire_pressure; // YHL: unit=kPa [0:508] 
    
    unsigned char      left_none_driven_wheel_rot_dir;  // YHL: 0x0=no moving, 0x1=moving forward, 0x2=moving backward, 0x3=error/invalid
    unsigned char      right_none_driven_wheel_rot_dir; // YHL: 0x0=no moving, 0x1=moving forward, 0x2=moving backward, 0x3=error/invalid
    unsigned char      bonnet_status;     // YHL: 0=close, 1=open, 2=switch disconnect
    unsigned char      load_space_status; // YHL: 0=close, 1=open, 2=reserve, 3=reserve

    int   VCU_reserved_int_2;//保留位
    int   VCU_reserved_int_3;//保留位
    int   VCU_reserved_int_4;//保留位
    int   VCU_reserved_int_5;//保留位
    int   VCU_reserved_int_6;//保留位
    //int   VCU_reserved_int_7;//保留位
    //int   VCU_reserved_int_8;//保留位
    //int   VCU_reserved_int_9;//保留位             

    float VCU_reserved_float_0;//保留位
    float VCU_reserved_float_1;//保留位
    float VCU_reserved_float_2;//保留位
    float VCU_reserved_float_3;//保留位
    float VCU_reserved_float_4;//保留位
    float VCU_reserved_float_5;//保留位
    float VCU_reserved_float_6;//保留位
    float VCU_reserved_float_7;//保留位
    float VCU_reserved_float_8;//保留位
    float VCU_reserved_float_9;//保留位
}COMMON_VEHICLE_DATA;
typedef struct
{
    uint64_t nTimeStamp;        // 系统时间
    uint64_t process_finsh_time;//处理完毕时间
    int32_t nFrameID;           //无视
    int32_t nCounter;           //无视
} Header;
typedef struct Point2f_
{
    Point2f_(float32_t x_in=0.0f,float32_t y_in=0.0f)
    {
        x = x_in;
        y = y_in;
    }
    float32_t x;
    float32_t y;
}Point2f;
typedef struct
{
    int32_t     int32_IF_1;      //0表示雷达车位，1，表示视觉车位,-1表示没有车位
    int32_t     int32_IF_2;		//sReservIF_data_Single_Fusion_Lot.int32_IF_2 表示OCR数字
    int32_t     int32_IF_Array[10];
    float32_t   float32_IF_1;
    float32_t   float32_IF_2;
    float32_t   float32_IF_Array[10];
    int64_t     int64_IF_1;
    double      double_IF_1;
    Point2f     Points[4];
} CommonReserveIF;
typedef struct
{
    int32_t nType;          //为0表示表示水平车位，为1表示纵向车位，为2表示斜车位
    int32_t nParkingGarageID;
    float fConfidence;      //给 0~1，有桶或锁的库位不给出
    float lanemark_width;   // 车位线宽
    Point2f psSlotPoints[4];//四个点的给出顺序：库位开口开始，逆时针方向
    float fSita;            //the angle of vehicle direction and parking garage direction,车体前进轴与库位线之间的夹角
    int32_t nlocation;      //0表示右侧车位，1表示左侧车位
	float distance_d1;
	float distance_d2;
	float distance_d3;
	float distance_d4;
	float distance_d5;
	Point2f objPoints[5];
	int32_t status;
	unsigned long long m_LotsBuildTimestamp;	///< build lots can timestamp for fusion usage
    CommonReserveIF sReservIF_data_Single_Vision_Lot;//.int32_IF_2;表示counter
} SingleVisionLot;

typedef struct
{
    int     error_status;       //0表示正常，无error
    char    error_info[256];    //发生错误时，错误信息

    char    debug_info_1[256];  //用于调试的错误信息
    char    debug_info_2[256];  //用于调试的错误信息
} ErrorInfo;

typedef struct
{
	Header stHeader;
	int32_t nParkingGarageNum;          //找到的库位总数
	int32_t nSuggestParkingGarageID;    //推荐的库位 ID
	SingleVisionLot gstParkingLotList[PldVisionMaxNum];//如果只给出一个库位，则默认 gstParkingGarage[0]

	CommonReserveIF sReservIF_data_PLD_Vision_Result;
	ErrorInfo PLD_Vision_error_info;
} PldVisionResult;
typedef struct
{
	float32_t x;
	float32_t y;
}strCoordinateFloatType;
typedef struct
{
	float32_t fDistance[3];
	uint8_t ubType[3];
	uint8_t ubReserve;
}strAPASonarInfo;

typedef struct
{
	strCoordinateFloatType coordinate;
	float32_t fDistance[3];
	uint8_t ubType;
	uint8_t ubReserve[3];
}strUPASonarInfo;



typedef struct
{
	strCoordinateFloatType coordinate;
	float32_t fAngle;
}strCarPosition;

typedef struct{
	int32_t StepNumOfTrajectory;
}APATrajCalCarFollowPathDatahead;
typedef enum
{
	GEAR_P = 0u,
	GEAR_N,
	GEAR_D,
	GEAR_R
}enumGearPos;
typedef struct {
	strCarPosition CarPos;
	float32_t CarCurvature;
	enumGearPos shift_pos;
} APATrajCalCarFollowPathData;
#define APA_TRAJCAL_PARKING_MAX_OUTPUT_STEP_IN_ONE_PATH (50U)

typedef struct {
	APATrajCalCarFollowPathDatahead OutPutDataHead;
	APATrajCalCarFollowPathData FollowPathData[APA_TRAJCAL_PARKING_MAX_OUTPUT_STEP_IN_ONE_PATH];
} APATrajCalCarParkingPath;
#define FUSION_PLD_SLOT_MAX_NUM (12U)

typedef struct
{
	int16_t sSlotID;
	APA_ENUM_TYPE SlotType;//parking slot type got it 
	APA_ENUM_TYPE ParkingInSideType;//in the left or right got it 
	APA_ENUM_TYPE SlotRoadSideType;//the type of the edge of the parking slot ,1 is the road tooth,-1 is the wall got it 
	APA_ENUM_TYPE SublelaneRoadSideType;//the type of the edge of the opposite position,0 is the road tooth ,1 is the wall got it 
	APA_DISTANCE_CAL_FLOAT_TYPE APASlotPSlotDepth;//the slot depth in parallel slot //��ȷ���ǲ�����pp��Կ� by JN
	APA_DISTANCE_CAL_FLOAT_TYPE SlotWidth;//by JN ��pp����һ��
	strCarPosition SlotEndPos;
	BOOLEAN bObj1Detected;								/* Obj1 detected flag */
	BOOLEAN bObj2Detected;								/* Obj2 detected flag */
	strCoordinateFloatType SlotObj1Pos;//the coordinate of obj1 in vehicle coordinate system
	strCoordinateFloatType SlotObj2Pos;//the coordinate of obj2 in vehicle coordinate system //not ensure if the result is in the world coordinat got it 
	APA_DISTANCE_CAL_FLOAT_TYPE fSlotSubLaneObjX;//the distance of Opposite position in World coordinate system //got it 
	APA_DISTANCE_CAL_FLOAT_TYPE fSlotSubLaneObjXTheta;//byJN,����ȷ����λ�����logic��
	BOOLEAN bSlotSubLaneObjXThetaUpdated;//Ѱ������б�ʶλ����Ҫ����fSlotSubLaneObjXThetaͬ��
}FusionPldSlotParamT;

typedef struct
{
	int16_t nLeftSLotNum;
	int16_t nRightSLotNum;
	int8_t uParkingOutSlotType;
	FusionPldSlotParamT arrLeftSlotList[FUSION_PLD_SLOT_MAX_NUM];
	FusionPldSlotParamT arrRightSlotList[FUSION_PLD_SLOT_MAX_NUM];
	BOOLEAN bSlotLocked;
	FusionPldSlotParamT SlotLocked;
}FusionPldOutputT;


typedef struct
{
	float32_t fAcceleration;	//4,Acceleration unit m/ss
	float32_t fSpeed;			//8, speed  unit  m/s
	float32_t fDistance;		//12,distance unit  m
}strVerticalControl;

typedef struct
{
	strVerticalControl verticalCtrl;
	enumGearPos shift;
	float32_t steeringWheelAngle;		/*unit: degree*/
}strCarControl;



typedef struct
{
	BOOLEAN bSlotLocked;
	int8_t uSonarPldState;
	int8_t uSonarPldErrCode;
	int8_t uParkingOutSlotType;
	int16_t nLeftSLotNum;
	int16_t nRightSLotNum;
	FusionPldSlotParamT arrLeftSlotList[SONAR_PLD_SLOT_MAX_NUM];/* Yihui: saves slot data, by default, it save data for tail in parking mode */
	FusionPldSlotParamT arrRightSlotList[SONAR_PLD_SLOT_MAX_NUM];/* Yihui: saves slot data, by default, it save data for tail in parking mode */
	FusionPldSlotParamT arrLeftSlotListPark[SONAR_PLD_SLOT_MAX_NUM][2];/* Added by Yihui, save data for Tail in[0] and Head in[1] */
	FusionPldSlotParamT arrRightSlotListPark[SONAR_PLD_SLOT_MAX_NUM][2];/* Added by Yihui, save data for Tail in[0] and Head in[1] */
	FusionPldSlotParamT SlotLocked;
}SonarPldOutputT;

typedef struct ApaLcmData_
{
    uint32_t data_type;
    uint32_t data_size;
    COMMON_VEHICLE_DATA canInfo;
    //vpld data
    PldVisionResult target_vpld_output;
    //pp data
    APATrajCalCarParkingPath target_pp_path;
    int8_t default_slot_id;
    uint8_t apa_counter;
    uint8_t selected_slot_id;
    //sm state
    uint8_t target_apa_sm_state;

    strAPASonarInfo APASonarInfo[4];				//APA sonar index:	RLS:0 RRS:1 FLS:2 FRS:3
    strUPASonarInfo UPASonarInfo[10];		//UPA sonar index:	FLC:0 FLM:1 FRM:2 FRC:3 RLC:4 RLM:5 RRM:6 RRC:7

    //dr data
    strCarPosition target_vehicle_pose;
    //fpld data
    FusionPldOutputT target_fpld_output;
    strCarControl carCtrl;
    strVerticalControl meb_result;

    SonarPldOutputT sonarPldOutput;
    uint8_t brakeReq;
    uint8_t apa_state_id;
    uint8_t reserve[1998];
}ApaLcmData;


#endif