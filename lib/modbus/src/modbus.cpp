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

bool modbus_send_pkg(uint8_t *msg, size_t length) {
    int L = length+2;
    uint16_t crc = modbus_crc16(msg, length);
    
    msg[length] = uint8_t(crc);
    msg[length+1] = crc >> 8;

    SERIAL.write(msg, L);
    
    uint8_t response[2*L];
    for (int i = 0; i < 2*L; i++) response[i] = 0x00;
    int resp_L=0;
    SERIAL.flush();
    for(int i = 0; i < 100; i++) {
        Serial3.print("1");
    }

    while (SERIAL.available())
    {
        response[resp_L++] = SERIAL.read();
    }
    if(resp_L > L*2) return false;

    int begin = 0;
    while(response[begin] != msg[0]){begin++;}
    if(begin > resp_L-L) return false;
    
    for(int i = 0; i < L; i++) {
        if(response[begin+i] != msg[i]) {return false;}
    }
    return true;
}

void modbus_send_msg(uint8_t *msg, size_t length) {
    int faults = 0;
    while (!modbus_send_pkg(msg, length)){
        faults++;
        if(faults >= 10){
            //Communication fault
            system_set_exec_alarm(EXEC_ALARM_ABORT_CYCLE);
            protocol_execute_realtime();
            break;
        }
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