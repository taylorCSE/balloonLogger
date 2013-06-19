/**
 * \file logger.cpp
 * \brief Parses serial data and logs to database
 */

#include "logger.h"

using namespace std;


struct header {
    uint16_t id;
    uint8_t cmd;
};

struct data_payload {
    uint8_t digital;
    uint16_t altitude;
    uint16_t rate;
    uint16_t analog[18];
};

struct gps_payload {
    uint8_t status;
    uint16_t altitude;
    uint16_t rate;
    char gps[36];
};

struct gps_packet {
    struct header header;
    struct gps_payload payload;
};

struct data_packet {
    struct header header;
    struct data_payload payload;
};

union PacketBuf {
    char raw[64];
    struct header header;
    struct data_packet dataPacket;
    struct gps_packet gpsPacket;
};

union PacketBuf packetBuf;
const int packetLength = 46;
int packetPos;

void storePacket() {
    packetPos = 0;
}

int nextPacket() {
    int readData = 0;
    
    if (packetPos > 0) {
        packetPos += COMM_GetData(packetBuf.raw, packetLength - packetPos);
        readData = 1;
    }
    
    if (packetPos == packetLength) {
        storePacket();
    }
    
    return readData;
}

void LOGGER_storeAvailablePackets() {
    while(nextPacket());
}
