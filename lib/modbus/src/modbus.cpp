#include "modbus.h"

void modbus_init() {
    SERIAL.begin(9600, SERIAL_8N2);
    Serial3.begin(9600);
}

void modbus_heartbeat() {
    uint8_t msg[8] = {MODBUS_ID, 0x03, 0x80, 0x00, 0x00, 0x01};
    modbus_send_msg(msg, 6);
}

void modbus_spindle_run() {
    uint8_t msg[8] = {MODBUS_ID, 0x06, 0x20, 0x00, 0x00, 0x01};
    modbus_send_msg(msg, 6);
}

void modbus_spindle_stop() {
    uint8_t msg[8] = {MODBUS_ID, 0x06, 0x20, 0x00, 0x00, 0x06};
    modbus_send_msg(msg, 6);
}

void modbus_spindle_reverse() {
    uint8_t msg[8] = {MODBUS_ID, 0x06, 0x20, 0x00, 0x00, 0x02};
    modbus_send_msg(msg, 6);
}

void modbus_spindle_set_rpm(float rpm) {
    uint8_t value = round(rpm * 0.39) & 0xFF;
    uint8_t msg[8] = {MODBUS_ID, 0x06, 0x10, 0x00, value, 0x00};
    modbus_send_msg(msg, 6);
}

void modbus_send_msg(uint8_t *msg, size_t length) {
    int L = length+2;
    uint16_t crc = modbus_crc16(msg, length);
    
    msg[length] = uint8_t(crc);
    msg[length+1] = crc >> 8;

    uint8_t response[L];

    SERIAL.write(msg, L);
    int i=0;
    int del=0;
    while(del < 1000) {
        Serial3.write(del);
        del++;
    }
}




uint16_t modbus_crc16(const uint8_t *data, size_t length) {
    uint16_t crc = 0xFFFF;
    
    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i];
        
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;  // 0xA001 is reversed polynomial of 0x8005
            } else {
                crc = crc >> 1;
            }
        }
    }
    
    return crc;
}