#ifndef VPLD_DATA_TYPE_H_
#define VPLD_DATA_TYPE_H_
#include <stdint.h>

#define OBJ_CORNER_THRESGOLD (0.4f)
#define OBJ_MAX_NUM          (20)
#define OBJ_CORNER_MAX_NUM   (20)
#define SLOT_CORNER_MAX_NUM (OBJ_CORNER_MAX_NUM)
#define SLOT_CORNER_ROI_WIDTH (28)
#define SLOT_CORNER_ROI_HEIGHT (28)
#define SLOT_ROI_WIDTH (28)
#define SLOT_ROI_HEIGHT (28)
#define SLOT_MAX_NUM (20)
typedef struct CV_Point_
{
    int x;
    int y;
}CV_Point;
typedef struct CV_Point2f_
{
    float x;
    float y;
}CV_Point2f;
typedef struct ObjectInfo_
{
    float number;
    float label;
    float score;
    float x_min;
    float y_min;
    float x_max;
    float y_max;

}ObjectInfo;

typedef struct SlotCorner_
{
    CV_Point2f point_v;
    CV_Point pixel;

}SlotCorner;

typedef struct Slot_
{
    uint16_t id;
    uint8_t type;
    uint8_t location;
    uint8_t inner_obstacle;
    uint8_t rsv[3];
    float width;
    float depth;
    SlotCorner corner[4];

}Slot;




#ifndef PI
#define PI 3.14159265358979323846264338327950288
#endif

#define CONSTANT  3
#define LINEAR    2
#define QUADRATIC 1
#define CUBIC     0

typedef enum
{
    SHIFT_P = 0x0,
    SHIFT_R = 0x1,
    SHIFT_N = 0x2,
    SHIFT_D = 0x3,
    SHIFT_INVALID = 0x4    
}GearShift;

typedef enum
{
    STATIONARY      = 0x0,
    MOVING_FORWARD  = 0x1,
    MOVING_BACKWARD = 0x2
}MovingDirection;

typedef struct VehicleModelParam_
{
    int is_pulse_enabled;
    int max_pulse;
    float meter_per_pulse;
    float wheel_base;
    float wheel_2_tail;
    float vehicle_width;
    float vehicle_length;
    float FL_coeff[4];
    float FR_coeff[4];
    float RL_coeff[4];
    float RR_coeff[4];
}VehicleModelParam;

typedef struct VehiclePose_
{
    float x;//unit = m
    float y;//unit = m
    float theta;//unit = rad
}VehiclePose;

typedef struct VehicleDeadReckoningInput_
{
    int timestamp;//unit = ms
    int gear_shift;
    float steering_wheel_angle;//unit = deg, left > 0, right < 0
    float wheel_speed_rl;//unit = km/h
    float wheel_speed_rr;//unit = km/h
    int wheel_pulse_rl;//unit = cont
    int wheel_pulse_rr;//unit = cont
}VehicleDeadReckoningInput;

typedef struct VehicleDeadReckoning_
{
    VehiclePose global_pose;
    VehiclePose delta_pose;
    VehicleModelParam vehicle_model_param;
    VehicleDeadReckoningInput current_input;
    VehicleDeadReckoningInput last_input;
}VehicleDeadReckoning;

typedef struct VpldResult_
{
    uint64_t timestamp;
    uint64_t frame_id;
    float object_raw_data[7*OBJ_MAX_NUM];
    uint8_t obj_sup_flag[OBJ_MAX_NUM];
    Slot slot[SLOT_MAX_NUM];
    uint8_t slot_num;
    uint8_t vpld_state;
    uint8_t rsv[2];
    VehiclePose current_pose;
    Slot target_slot;


}VpldResult;

#endif