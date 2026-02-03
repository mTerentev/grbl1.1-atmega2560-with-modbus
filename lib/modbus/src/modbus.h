#ifndef modbus
#define modbus

#include <Arduino.h>
#include "grbl.h"

#define SERIAL Serial2
#define MAX_FREQ 320
#define MAX_RPM (320*60)
#define MODBUS_ID 0x01

#ifdef __cplusplus
extern "C" {
#endif

void modbus_init();

void modbus_heartbeat();

void modbus_spindle_run();

void modbus_spindle_stop();

void modbus_spindle_reverse();

void modbus_spindle_set_rpm(float rpm);

uint16_t modbus_crc16(const uint8_t *data, size_t length);

void modbus_send_msg(uint8_t *msg, size_t length);

#ifdef __cplusplus
}
#endif

#endif