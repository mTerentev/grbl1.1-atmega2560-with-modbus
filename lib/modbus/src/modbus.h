#ifndef modbus
#define modbus

#define SERIAL 2
#define MODBUS_BAUDRATE 9600
#define MAX_RPM 24000.00

#include <stdint.h>
#include <stdlib.h>
#include "uart_protocol_driver.h"

void modbus_init();

void modbus_heartbeat();

void modbus_spindle_run();

void modbus_spindle_stop();

void modbus_spindle_reverse();

void modbus_spindle_set_rpm(float rpm);

uint16_t modbus_crc16(const uint8_t *data, size_t length);

void modbus_send_msg(uint8_t *msg, size_t length);

#endif