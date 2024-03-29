#pragma once
// MESSAGE ZY_SENSORS PACKING

#define MAVLINK_MSG_ID_ZY_SENSORS 52

typedef struct __mavlink_zy_sensors_t {
 float main_voltage; /*< main_voltage*/
 float main_current; /*< main_current*/
 float bak_voltage; /*< bak_voltage*/
 float motor_current1; /*< motor_current1*/
 float motor_current2; /*< motor_current2*/
 float motor_current3; /*< motor_current3*/
 float motor_current4; /*< motor_current4*/
 float motor_current5; /*< motor_current5*/
 float motor_current6; /*< motor_current6*/
 float motor_current7; /*< motor_current7*/
 float motor_current8; /*< motor_current8*/
 float motor_temperature1; /*< motor_temperature1*/
 float motor_temperature2; /*< motor_temperature2*/
 float motor_temperature3; /*< motor_temperature3*/
 float motor_temperature4; /*< motor_temperature4*/
 float motor_temperature5; /*< motor_temperature5*/
 float motor_temperature6; /*< motor_temperature6*/
 float motor_temperature7; /*< motor_temperature7*/
 float motor_temperature8; /*< motor_temperature8*/
} mavlink_zy_sensors_t;

#define MAVLINK_MSG_ID_ZY_SENSORS_LEN 76
#define MAVLINK_MSG_ID_ZY_SENSORS_MIN_LEN 76
#define MAVLINK_MSG_ID_52_LEN 76
#define MAVLINK_MSG_ID_52_MIN_LEN 76

#define MAVLINK_MSG_ID_ZY_SENSORS_CRC 246
#define MAVLINK_MSG_ID_52_CRC 246



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ZY_SENSORS { \
    52, \
    "ZY_SENSORS", \
    19, \
    {  { "main_voltage", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_zy_sensors_t, main_voltage) }, \
         { "main_current", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_zy_sensors_t, main_current) }, \
         { "bak_voltage", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_zy_sensors_t, bak_voltage) }, \
         { "motor_current1", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_zy_sensors_t, motor_current1) }, \
         { "motor_current2", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_zy_sensors_t, motor_current2) }, \
         { "motor_current3", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_zy_sensors_t, motor_current3) }, \
         { "motor_current4", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_zy_sensors_t, motor_current4) }, \
         { "motor_current5", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_zy_sensors_t, motor_current5) }, \
         { "motor_current6", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_zy_sensors_t, motor_current6) }, \
         { "motor_current7", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_zy_sensors_t, motor_current7) }, \
         { "motor_current8", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_zy_sensors_t, motor_current8) }, \
         { "motor_temperature1", NULL, MAVLINK_TYPE_FLOAT, 0, 44, offsetof(mavlink_zy_sensors_t, motor_temperature1) }, \
         { "motor_temperature2", NULL, MAVLINK_TYPE_FLOAT, 0, 48, offsetof(mavlink_zy_sensors_t, motor_temperature2) }, \
         { "motor_temperature3", NULL, MAVLINK_TYPE_FLOAT, 0, 52, offsetof(mavlink_zy_sensors_t, motor_temperature3) }, \
         { "motor_temperature4", NULL, MAVLINK_TYPE_FLOAT, 0, 56, offsetof(mavlink_zy_sensors_t, motor_temperature4) }, \
         { "motor_temperature5", NULL, MAVLINK_TYPE_FLOAT, 0, 60, offsetof(mavlink_zy_sensors_t, motor_temperature5) }, \
         { "motor_temperature6", NULL, MAVLINK_TYPE_FLOAT, 0, 64, offsetof(mavlink_zy_sensors_t, motor_temperature6) }, \
         { "motor_temperature7", NULL, MAVLINK_TYPE_FLOAT, 0, 68, offsetof(mavlink_zy_sensors_t, motor_temperature7) }, \
         { "motor_temperature8", NULL, MAVLINK_TYPE_FLOAT, 0, 72, offsetof(mavlink_zy_sensors_t, motor_temperature8) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ZY_SENSORS { \
    "ZY_SENSORS", \
    19, \
    {  { "main_voltage", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_zy_sensors_t, main_voltage) }, \
         { "main_current", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_zy_sensors_t, main_current) }, \
         { "bak_voltage", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_zy_sensors_t, bak_voltage) }, \
         { "motor_current1", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_zy_sensors_t, motor_current1) }, \
         { "motor_current2", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_zy_sensors_t, motor_current2) }, \
         { "motor_current3", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_zy_sensors_t, motor_current3) }, \
         { "motor_current4", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_zy_sensors_t, motor_current4) }, \
         { "motor_current5", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_zy_sensors_t, motor_current5) }, \
         { "motor_current6", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_zy_sensors_t, motor_current6) }, \
         { "motor_current7", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_zy_sensors_t, motor_current7) }, \
         { "motor_current8", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_zy_sensors_t, motor_current8) }, \
         { "motor_temperature1", NULL, MAVLINK_TYPE_FLOAT, 0, 44, offsetof(mavlink_zy_sensors_t, motor_temperature1) }, \
         { "motor_temperature2", NULL, MAVLINK_TYPE_FLOAT, 0, 48, offsetof(mavlink_zy_sensors_t, motor_temperature2) }, \
         { "motor_temperature3", NULL, MAVLINK_TYPE_FLOAT, 0, 52, offsetof(mavlink_zy_sensors_t, motor_temperature3) }, \
         { "motor_temperature4", NULL, MAVLINK_TYPE_FLOAT, 0, 56, offsetof(mavlink_zy_sensors_t, motor_temperature4) }, \
         { "motor_temperature5", NULL, MAVLINK_TYPE_FLOAT, 0, 60, offsetof(mavlink_zy_sensors_t, motor_temperature5) }, \
         { "motor_temperature6", NULL, MAVLINK_TYPE_FLOAT, 0, 64, offsetof(mavlink_zy_sensors_t, motor_temperature6) }, \
         { "motor_temperature7", NULL, MAVLINK_TYPE_FLOAT, 0, 68, offsetof(mavlink_zy_sensors_t, motor_temperature7) }, \
         { "motor_temperature8", NULL, MAVLINK_TYPE_FLOAT, 0, 72, offsetof(mavlink_zy_sensors_t, motor_temperature8) }, \
         } \
}
#endif

/**
 * @brief Pack a zy_sensors message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param main_voltage main_voltage
 * @param main_current main_current
 * @param bak_voltage bak_voltage
 * @param motor_current1 motor_current1
 * @param motor_current2 motor_current2
 * @param motor_current3 motor_current3
 * @param motor_current4 motor_current4
 * @param motor_current5 motor_current5
 * @param motor_current6 motor_current6
 * @param motor_current7 motor_current7
 * @param motor_current8 motor_current8
 * @param motor_temperature1 motor_temperature1
 * @param motor_temperature2 motor_temperature2
 * @param motor_temperature3 motor_temperature3
 * @param motor_temperature4 motor_temperature4
 * @param motor_temperature5 motor_temperature5
 * @param motor_temperature6 motor_temperature6
 * @param motor_temperature7 motor_temperature7
 * @param motor_temperature8 motor_temperature8
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_zy_sensors_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float main_voltage, float main_current, float bak_voltage, float motor_current1, float motor_current2, float motor_current3, float motor_current4, float motor_current5, float motor_current6, float motor_current7, float motor_current8, float motor_temperature1, float motor_temperature2, float motor_temperature3, float motor_temperature4, float motor_temperature5, float motor_temperature6, float motor_temperature7, float motor_temperature8)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ZY_SENSORS_LEN];
    _mav_put_float(buf, 0, main_voltage);
    _mav_put_float(buf, 4, main_current);
    _mav_put_float(buf, 8, bak_voltage);
    _mav_put_float(buf, 12, motor_current1);
    _mav_put_float(buf, 16, motor_current2);
    _mav_put_float(buf, 20, motor_current3);
    _mav_put_float(buf, 24, motor_current4);
    _mav_put_float(buf, 28, motor_current5);
    _mav_put_float(buf, 32, motor_current6);
    _mav_put_float(buf, 36, motor_current7);
    _mav_put_float(buf, 40, motor_current8);
    _mav_put_float(buf, 44, motor_temperature1);
    _mav_put_float(buf, 48, motor_temperature2);
    _mav_put_float(buf, 52, motor_temperature3);
    _mav_put_float(buf, 56, motor_temperature4);
    _mav_put_float(buf, 60, motor_temperature5);
    _mav_put_float(buf, 64, motor_temperature6);
    _mav_put_float(buf, 68, motor_temperature7);
    _mav_put_float(buf, 72, motor_temperature8);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ZY_SENSORS_LEN);
#else
    mavlink_zy_sensors_t packet;
    packet.main_voltage = main_voltage;
    packet.main_current = main_current;
    packet.bak_voltage = bak_voltage;
    packet.motor_current1 = motor_current1;
    packet.motor_current2 = motor_current2;
    packet.motor_current3 = motor_current3;
    packet.motor_current4 = motor_current4;
    packet.motor_current5 = motor_current5;
    packet.motor_current6 = motor_current6;
    packet.motor_current7 = motor_current7;
    packet.motor_current8 = motor_current8;
    packet.motor_temperature1 = motor_temperature1;
    packet.motor_temperature2 = motor_temperature2;
    packet.motor_temperature3 = motor_temperature3;
    packet.motor_temperature4 = motor_temperature4;
    packet.motor_temperature5 = motor_temperature5;
    packet.motor_temperature6 = motor_temperature6;
    packet.motor_temperature7 = motor_temperature7;
    packet.motor_temperature8 = motor_temperature8;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ZY_SENSORS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ZY_SENSORS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ZY_SENSORS_MIN_LEN, MAVLINK_MSG_ID_ZY_SENSORS_LEN, MAVLINK_MSG_ID_ZY_SENSORS_CRC);
}

/**
 * @brief Pack a zy_sensors message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param main_voltage main_voltage
 * @param main_current main_current
 * @param bak_voltage bak_voltage
 * @param motor_current1 motor_current1
 * @param motor_current2 motor_current2
 * @param motor_current3 motor_current3
 * @param motor_current4 motor_current4
 * @param motor_current5 motor_current5
 * @param motor_current6 motor_current6
 * @param motor_current7 motor_current7
 * @param motor_current8 motor_current8
 * @param motor_temperature1 motor_temperature1
 * @param motor_temperature2 motor_temperature2
 * @param motor_temperature3 motor_temperature3
 * @param motor_temperature4 motor_temperature4
 * @param motor_temperature5 motor_temperature5
 * @param motor_temperature6 motor_temperature6
 * @param motor_temperature7 motor_temperature7
 * @param motor_temperature8 motor_temperature8
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_zy_sensors_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float main_voltage,float main_current,float bak_voltage,float motor_current1,float motor_current2,float motor_current3,float motor_current4,float motor_current5,float motor_current6,float motor_current7,float motor_current8,float motor_temperature1,float motor_temperature2,float motor_temperature3,float motor_temperature4,float motor_temperature5,float motor_temperature6,float motor_temperature7,float motor_temperature8)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ZY_SENSORS_LEN];
    _mav_put_float(buf, 0, main_voltage);
    _mav_put_float(buf, 4, main_current);
    _mav_put_float(buf, 8, bak_voltage);
    _mav_put_float(buf, 12, motor_current1);
    _mav_put_float(buf, 16, motor_current2);
    _mav_put_float(buf, 20, motor_current3);
    _mav_put_float(buf, 24, motor_current4);
    _mav_put_float(buf, 28, motor_current5);
    _mav_put_float(buf, 32, motor_current6);
    _mav_put_float(buf, 36, motor_current7);
    _mav_put_float(buf, 40, motor_current8);
    _mav_put_float(buf, 44, motor_temperature1);
    _mav_put_float(buf, 48, motor_temperature2);
    _mav_put_float(buf, 52, motor_temperature3);
    _mav_put_float(buf, 56, motor_temperature4);
    _mav_put_float(buf, 60, motor_temperature5);
    _mav_put_float(buf, 64, motor_temperature6);
    _mav_put_float(buf, 68, motor_temperature7);
    _mav_put_float(buf, 72, motor_temperature8);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ZY_SENSORS_LEN);
#else
    mavlink_zy_sensors_t packet;
    packet.main_voltage = main_voltage;
    packet.main_current = main_current;
    packet.bak_voltage = bak_voltage;
    packet.motor_current1 = motor_current1;
    packet.motor_current2 = motor_current2;
    packet.motor_current3 = motor_current3;
    packet.motor_current4 = motor_current4;
    packet.motor_current5 = motor_current5;
    packet.motor_current6 = motor_current6;
    packet.motor_current7 = motor_current7;
    packet.motor_current8 = motor_current8;
    packet.motor_temperature1 = motor_temperature1;
    packet.motor_temperature2 = motor_temperature2;
    packet.motor_temperature3 = motor_temperature3;
    packet.motor_temperature4 = motor_temperature4;
    packet.motor_temperature5 = motor_temperature5;
    packet.motor_temperature6 = motor_temperature6;
    packet.motor_temperature7 = motor_temperature7;
    packet.motor_temperature8 = motor_temperature8;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ZY_SENSORS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ZY_SENSORS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ZY_SENSORS_MIN_LEN, MAVLINK_MSG_ID_ZY_SENSORS_LEN, MAVLINK_MSG_ID_ZY_SENSORS_CRC);
}

/**
 * @brief Encode a zy_sensors struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param zy_sensors C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_zy_sensors_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_zy_sensors_t* zy_sensors)
{
    return mavlink_msg_zy_sensors_pack(system_id, component_id, msg, zy_sensors->main_voltage, zy_sensors->main_current, zy_sensors->bak_voltage, zy_sensors->motor_current1, zy_sensors->motor_current2, zy_sensors->motor_current3, zy_sensors->motor_current4, zy_sensors->motor_current5, zy_sensors->motor_current6, zy_sensors->motor_current7, zy_sensors->motor_current8, zy_sensors->motor_temperature1, zy_sensors->motor_temperature2, zy_sensors->motor_temperature3, zy_sensors->motor_temperature4, zy_sensors->motor_temperature5, zy_sensors->motor_temperature6, zy_sensors->motor_temperature7, zy_sensors->motor_temperature8);
}

/**
 * @brief Encode a zy_sensors struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param zy_sensors C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_zy_sensors_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_zy_sensors_t* zy_sensors)
{
    return mavlink_msg_zy_sensors_pack_chan(system_id, component_id, chan, msg, zy_sensors->main_voltage, zy_sensors->main_current, zy_sensors->bak_voltage, zy_sensors->motor_current1, zy_sensors->motor_current2, zy_sensors->motor_current3, zy_sensors->motor_current4, zy_sensors->motor_current5, zy_sensors->motor_current6, zy_sensors->motor_current7, zy_sensors->motor_current8, zy_sensors->motor_temperature1, zy_sensors->motor_temperature2, zy_sensors->motor_temperature3, zy_sensors->motor_temperature4, zy_sensors->motor_temperature5, zy_sensors->motor_temperature6, zy_sensors->motor_temperature7, zy_sensors->motor_temperature8);
}

/**
 * @brief Send a zy_sensors message
 * @param chan MAVLink channel to send the message
 *
 * @param main_voltage main_voltage
 * @param main_current main_current
 * @param bak_voltage bak_voltage
 * @param motor_current1 motor_current1
 * @param motor_current2 motor_current2
 * @param motor_current3 motor_current3
 * @param motor_current4 motor_current4
 * @param motor_current5 motor_current5
 * @param motor_current6 motor_current6
 * @param motor_current7 motor_current7
 * @param motor_current8 motor_current8
 * @param motor_temperature1 motor_temperature1
 * @param motor_temperature2 motor_temperature2
 * @param motor_temperature3 motor_temperature3
 * @param motor_temperature4 motor_temperature4
 * @param motor_temperature5 motor_temperature5
 * @param motor_temperature6 motor_temperature6
 * @param motor_temperature7 motor_temperature7
 * @param motor_temperature8 motor_temperature8
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_zy_sensors_send(mavlink_channel_t chan, float main_voltage, float main_current, float bak_voltage, float motor_current1, float motor_current2, float motor_current3, float motor_current4, float motor_current5, float motor_current6, float motor_current7, float motor_current8, float motor_temperature1, float motor_temperature2, float motor_temperature3, float motor_temperature4, float motor_temperature5, float motor_temperature6, float motor_temperature7, float motor_temperature8)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ZY_SENSORS_LEN];
    _mav_put_float(buf, 0, main_voltage);
    _mav_put_float(buf, 4, main_current);
    _mav_put_float(buf, 8, bak_voltage);
    _mav_put_float(buf, 12, motor_current1);
    _mav_put_float(buf, 16, motor_current2);
    _mav_put_float(buf, 20, motor_current3);
    _mav_put_float(buf, 24, motor_current4);
    _mav_put_float(buf, 28, motor_current5);
    _mav_put_float(buf, 32, motor_current6);
    _mav_put_float(buf, 36, motor_current7);
    _mav_put_float(buf, 40, motor_current8);
    _mav_put_float(buf, 44, motor_temperature1);
    _mav_put_float(buf, 48, motor_temperature2);
    _mav_put_float(buf, 52, motor_temperature3);
    _mav_put_float(buf, 56, motor_temperature4);
    _mav_put_float(buf, 60, motor_temperature5);
    _mav_put_float(buf, 64, motor_temperature6);
    _mav_put_float(buf, 68, motor_temperature7);
    _mav_put_float(buf, 72, motor_temperature8);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ZY_SENSORS, buf, MAVLINK_MSG_ID_ZY_SENSORS_MIN_LEN, MAVLINK_MSG_ID_ZY_SENSORS_LEN, MAVLINK_MSG_ID_ZY_SENSORS_CRC);
#else
    mavlink_zy_sensors_t packet;
    packet.main_voltage = main_voltage;
    packet.main_current = main_current;
    packet.bak_voltage = bak_voltage;
    packet.motor_current1 = motor_current1;
    packet.motor_current2 = motor_current2;
    packet.motor_current3 = motor_current3;
    packet.motor_current4 = motor_current4;
    packet.motor_current5 = motor_current5;
    packet.motor_current6 = motor_current6;
    packet.motor_current7 = motor_current7;
    packet.motor_current8 = motor_current8;
    packet.motor_temperature1 = motor_temperature1;
    packet.motor_temperature2 = motor_temperature2;
    packet.motor_temperature3 = motor_temperature3;
    packet.motor_temperature4 = motor_temperature4;
    packet.motor_temperature5 = motor_temperature5;
    packet.motor_temperature6 = motor_temperature6;
    packet.motor_temperature7 = motor_temperature7;
    packet.motor_temperature8 = motor_temperature8;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ZY_SENSORS, (const char *)&packet, MAVLINK_MSG_ID_ZY_SENSORS_MIN_LEN, MAVLINK_MSG_ID_ZY_SENSORS_LEN, MAVLINK_MSG_ID_ZY_SENSORS_CRC);
#endif
}

/**
 * @brief Send a zy_sensors message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_zy_sensors_send_struct(mavlink_channel_t chan, const mavlink_zy_sensors_t* zy_sensors)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_zy_sensors_send(chan, zy_sensors->main_voltage, zy_sensors->main_current, zy_sensors->bak_voltage, zy_sensors->motor_current1, zy_sensors->motor_current2, zy_sensors->motor_current3, zy_sensors->motor_current4, zy_sensors->motor_current5, zy_sensors->motor_current6, zy_sensors->motor_current7, zy_sensors->motor_current8, zy_sensors->motor_temperature1, zy_sensors->motor_temperature2, zy_sensors->motor_temperature3, zy_sensors->motor_temperature4, zy_sensors->motor_temperature5, zy_sensors->motor_temperature6, zy_sensors->motor_temperature7, zy_sensors->motor_temperature8);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ZY_SENSORS, (const char *)zy_sensors, MAVLINK_MSG_ID_ZY_SENSORS_MIN_LEN, MAVLINK_MSG_ID_ZY_SENSORS_LEN, MAVLINK_MSG_ID_ZY_SENSORS_CRC);
#endif
}

#if MAVLINK_MSG_ID_ZY_SENSORS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_zy_sensors_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float main_voltage, float main_current, float bak_voltage, float motor_current1, float motor_current2, float motor_current3, float motor_current4, float motor_current5, float motor_current6, float motor_current7, float motor_current8, float motor_temperature1, float motor_temperature2, float motor_temperature3, float motor_temperature4, float motor_temperature5, float motor_temperature6, float motor_temperature7, float motor_temperature8)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, main_voltage);
    _mav_put_float(buf, 4, main_current);
    _mav_put_float(buf, 8, bak_voltage);
    _mav_put_float(buf, 12, motor_current1);
    _mav_put_float(buf, 16, motor_current2);
    _mav_put_float(buf, 20, motor_current3);
    _mav_put_float(buf, 24, motor_current4);
    _mav_put_float(buf, 28, motor_current5);
    _mav_put_float(buf, 32, motor_current6);
    _mav_put_float(buf, 36, motor_current7);
    _mav_put_float(buf, 40, motor_current8);
    _mav_put_float(buf, 44, motor_temperature1);
    _mav_put_float(buf, 48, motor_temperature2);
    _mav_put_float(buf, 52, motor_temperature3);
    _mav_put_float(buf, 56, motor_temperature4);
    _mav_put_float(buf, 60, motor_temperature5);
    _mav_put_float(buf, 64, motor_temperature6);
    _mav_put_float(buf, 68, motor_temperature7);
    _mav_put_float(buf, 72, motor_temperature8);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ZY_SENSORS, buf, MAVLINK_MSG_ID_ZY_SENSORS_MIN_LEN, MAVLINK_MSG_ID_ZY_SENSORS_LEN, MAVLINK_MSG_ID_ZY_SENSORS_CRC);
#else
    mavlink_zy_sensors_t *packet = (mavlink_zy_sensors_t *)msgbuf;
    packet->main_voltage = main_voltage;
    packet->main_current = main_current;
    packet->bak_voltage = bak_voltage;
    packet->motor_current1 = motor_current1;
    packet->motor_current2 = motor_current2;
    packet->motor_current3 = motor_current3;
    packet->motor_current4 = motor_current4;
    packet->motor_current5 = motor_current5;
    packet->motor_current6 = motor_current6;
    packet->motor_current7 = motor_current7;
    packet->motor_current8 = motor_current8;
    packet->motor_temperature1 = motor_temperature1;
    packet->motor_temperature2 = motor_temperature2;
    packet->motor_temperature3 = motor_temperature3;
    packet->motor_temperature4 = motor_temperature4;
    packet->motor_temperature5 = motor_temperature5;
    packet->motor_temperature6 = motor_temperature6;
    packet->motor_temperature7 = motor_temperature7;
    packet->motor_temperature8 = motor_temperature8;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ZY_SENSORS, (const char *)packet, MAVLINK_MSG_ID_ZY_SENSORS_MIN_LEN, MAVLINK_MSG_ID_ZY_SENSORS_LEN, MAVLINK_MSG_ID_ZY_SENSORS_CRC);
#endif
}
#endif

#endif

// MESSAGE ZY_SENSORS UNPACKING


/**
 * @brief Get field main_voltage from zy_sensors message
 *
 * @return main_voltage
 */
static inline float mavlink_msg_zy_sensors_get_main_voltage(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field main_current from zy_sensors message
 *
 * @return main_current
 */
static inline float mavlink_msg_zy_sensors_get_main_current(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field bak_voltage from zy_sensors message
 *
 * @return bak_voltage
 */
static inline float mavlink_msg_zy_sensors_get_bak_voltage(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field motor_current1 from zy_sensors message
 *
 * @return motor_current1
 */
static inline float mavlink_msg_zy_sensors_get_motor_current1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field motor_current2 from zy_sensors message
 *
 * @return motor_current2
 */
static inline float mavlink_msg_zy_sensors_get_motor_current2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field motor_current3 from zy_sensors message
 *
 * @return motor_current3
 */
static inline float mavlink_msg_zy_sensors_get_motor_current3(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field motor_current4 from zy_sensors message
 *
 * @return motor_current4
 */
static inline float mavlink_msg_zy_sensors_get_motor_current4(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field motor_current5 from zy_sensors message
 *
 * @return motor_current5
 */
static inline float mavlink_msg_zy_sensors_get_motor_current5(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field motor_current6 from zy_sensors message
 *
 * @return motor_current6
 */
static inline float mavlink_msg_zy_sensors_get_motor_current6(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Get field motor_current7 from zy_sensors message
 *
 * @return motor_current7
 */
static inline float mavlink_msg_zy_sensors_get_motor_current7(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  36);
}

/**
 * @brief Get field motor_current8 from zy_sensors message
 *
 * @return motor_current8
 */
static inline float mavlink_msg_zy_sensors_get_motor_current8(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  40);
}

/**
 * @brief Get field motor_temperature1 from zy_sensors message
 *
 * @return motor_temperature1
 */
static inline float mavlink_msg_zy_sensors_get_motor_temperature1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  44);
}

/**
 * @brief Get field motor_temperature2 from zy_sensors message
 *
 * @return motor_temperature2
 */
static inline float mavlink_msg_zy_sensors_get_motor_temperature2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  48);
}

/**
 * @brief Get field motor_temperature3 from zy_sensors message
 *
 * @return motor_temperature3
 */
static inline float mavlink_msg_zy_sensors_get_motor_temperature3(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  52);
}

/**
 * @brief Get field motor_temperature4 from zy_sensors message
 *
 * @return motor_temperature4
 */
static inline float mavlink_msg_zy_sensors_get_motor_temperature4(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  56);
}

/**
 * @brief Get field motor_temperature5 from zy_sensors message
 *
 * @return motor_temperature5
 */
static inline float mavlink_msg_zy_sensors_get_motor_temperature5(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  60);
}

/**
 * @brief Get field motor_temperature6 from zy_sensors message
 *
 * @return motor_temperature6
 */
static inline float mavlink_msg_zy_sensors_get_motor_temperature6(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  64);
}

/**
 * @brief Get field motor_temperature7 from zy_sensors message
 *
 * @return motor_temperature7
 */
static inline float mavlink_msg_zy_sensors_get_motor_temperature7(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  68);
}

/**
 * @brief Get field motor_temperature8 from zy_sensors message
 *
 * @return motor_temperature8
 */
static inline float mavlink_msg_zy_sensors_get_motor_temperature8(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  72);
}

/**
 * @brief Decode a zy_sensors message into a struct
 *
 * @param msg The message to decode
 * @param zy_sensors C-struct to decode the message contents into
 */
static inline void mavlink_msg_zy_sensors_decode(const mavlink_message_t* msg, mavlink_zy_sensors_t* zy_sensors)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    zy_sensors->main_voltage = mavlink_msg_zy_sensors_get_main_voltage(msg);
    zy_sensors->main_current = mavlink_msg_zy_sensors_get_main_current(msg);
    zy_sensors->bak_voltage = mavlink_msg_zy_sensors_get_bak_voltage(msg);
    zy_sensors->motor_current1 = mavlink_msg_zy_sensors_get_motor_current1(msg);
    zy_sensors->motor_current2 = mavlink_msg_zy_sensors_get_motor_current2(msg);
    zy_sensors->motor_current3 = mavlink_msg_zy_sensors_get_motor_current3(msg);
    zy_sensors->motor_current4 = mavlink_msg_zy_sensors_get_motor_current4(msg);
    zy_sensors->motor_current5 = mavlink_msg_zy_sensors_get_motor_current5(msg);
    zy_sensors->motor_current6 = mavlink_msg_zy_sensors_get_motor_current6(msg);
    zy_sensors->motor_current7 = mavlink_msg_zy_sensors_get_motor_current7(msg);
    zy_sensors->motor_current8 = mavlink_msg_zy_sensors_get_motor_current8(msg);
    zy_sensors->motor_temperature1 = mavlink_msg_zy_sensors_get_motor_temperature1(msg);
    zy_sensors->motor_temperature2 = mavlink_msg_zy_sensors_get_motor_temperature2(msg);
    zy_sensors->motor_temperature3 = mavlink_msg_zy_sensors_get_motor_temperature3(msg);
    zy_sensors->motor_temperature4 = mavlink_msg_zy_sensors_get_motor_temperature4(msg);
    zy_sensors->motor_temperature5 = mavlink_msg_zy_sensors_get_motor_temperature5(msg);
    zy_sensors->motor_temperature6 = mavlink_msg_zy_sensors_get_motor_temperature6(msg);
    zy_sensors->motor_temperature7 = mavlink_msg_zy_sensors_get_motor_temperature7(msg);
    zy_sensors->motor_temperature8 = mavlink_msg_zy_sensors_get_motor_temperature8(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ZY_SENSORS_LEN? msg->len : MAVLINK_MSG_ID_ZY_SENSORS_LEN;
        memset(zy_sensors, 0, MAVLINK_MSG_ID_ZY_SENSORS_LEN);
    memcpy(zy_sensors, _MAV_PAYLOAD(msg), len);
#endif
}
