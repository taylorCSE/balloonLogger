#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <map>
#include <string>

#include "database.h"
#include "comm.h"

struct LOGGER_State_t {
    uint8_t gpsStatus;
    uint16_t altitude;
    uint16_t rate;
    uint8_t digital;
};

extern LOGGER_State_t LOGGER_state;

int LOGGER_storeAvailablePackets();
char* LOGGER_getLastPacket();
int LOGGER_lastPacketType();
int LOGGER_lastPacketId();

#endif
