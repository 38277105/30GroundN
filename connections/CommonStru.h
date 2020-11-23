#ifndef __COMMON_STRU__
#define __COMMON_STRU__

//#include <common/mavlink.h>
#include <ardupilotmega/mavlink.h>
struct Time_Stamps
{
    Time_Stamps()
    {
        reset_timestamps();
    }

    uint64_t heartbeat;
    uint64_t sys_status;
    uint64_t battery_status;
    uint64_t radio_status;
    uint64_t local_position_ned;
    uint64_t global_position_int;
    uint64_t position_target_local_ned;
    uint64_t position_target_global_int;
    uint64_t highres_imu;
    uint64_t attitude;

    void
    reset_timestamps()
    {
        heartbeat = 0;
        sys_status = 0;
        battery_status = 0;
        radio_status = 0;
        local_position_ned = 0;
        global_position_int = 0;
        position_target_local_ned = 0;
        position_target_global_int = 0;
        highres_imu = 0;
        attitude = 0;
    }
};

struct MavlinkMessages {

    int sysid;
    int compid;

  /*
********************************************************************************
*/
    // System Status
    mavlink_sys_status_t sys_status;

    // Battery Status
    mavlink_battery_status_t battery_status;

    // Radio Status
    mavlink_radio_status_t radio_status;

    // Local Position
    mavlink_local_position_ned_t local_position_ned;

    // Global Position
    mavlink_global_position_int_t global_position_int;

    // Local Position Target
    mavlink_position_target_local_ned_t position_target_local_ned;

    // Global Position Target
    mavlink_position_target_global_int_t position_target_global_int;

    //GPS_RAW_int data
    mavlink_gps_raw_int_t  gps_raw_int_data;

    mavlink_zy_sensors_t  zy_sensor_status;

    // HiRes IMU
    mavlink_highres_imu_t highres_imu;

    // Attitude
    mavlink_attitude_t attitude;

    //VFR_HUD
    mavlink_vfr_hud_t vfr_hud;

    // Time Stamps
    Time_Stamps time_stamps;

    mavlink_rc_channels_scaled_t rc_channels_scaled;

    mavlink_local_position_ned_system_global_offset_t position_RTK;

    mavlink_servo_output_raw_t servo_output_raw;

    mavlink_servo_output_raw_t servo_output_raw1;

    void
    reset_timestamps()
    {
        time_stamps.reset_timestamps();
    }

};

#endif //__COMMON_STRU__
