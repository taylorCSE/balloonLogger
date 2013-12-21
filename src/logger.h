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
    uint16_t lastId;
    uint8_t lastCmd;
    int bytesRead;
    int packetsRead;
    char gps[36];
};

/**
 * Stores logger state both for local tracking and external display
**/

extern LOGGER_State_t LOGGER_state;

int LOGGER_storeAvailablePackets();
char* LOGGER_getLastPacket();

#endif
