#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <map>
#include <string>

#include "database.h"
#include "comm.h"

int LOGGER_storeAvailablePackets();
char* LOGGER_getLastPacket();
int LOGGER_lastPacketType();
int LOGGER_lastPacketId();
int LOGGER_getGPSStatus();

#endif
