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
int packetPos = 0;
int syncBytesSeen = 0;

void storePacket() {
    packetPos = 0;
    syncBytesSeen = 0;
    
    if (packetBuf.header.cmd == 0x1A) {
        char* sections[6];
        int currentSection = 1;
        
        sections[0] = packetBuf.gpsPacket.payload.gps;
        
        for (char* pos = packetBuf.gpsPacket.payload.gps; pos <= packetBuf.gpsPacket.payload.gps + 36; pos++) {
            if (*pos == ',') {
                *pos = '\0';
                pos++;
                sections[currentSection] = pos;
                currentSection++;
            }
        }
        
        DB_addGpsPacket(
            packetBuf.gpsPacket.header.id,
            packetBuf.gpsPacket.payload.status,
            sections[0],
            sections[1],
            sections[2],
            sections[3],
            sections[4],
            sections[5]
        );
    }
}

int nextPacket() {
    int bytesRead = 0;
    
    if (packetPos == 0 && syncBytesSeen != 2) {
        bytesRead = COMM_GetData(packetBuf.raw, 1);
        
        if (bytesRead == 1) {
            if (packetBuf.raw[0] == 0xEE) {
                syncBytesSeen += 1;
            } else {
                syncBytesSeen = 0;
            }
        }
    } else {
        bytesRead = COMM_GetData(packetBuf.raw, packetLength - packetPos);
        packetPos += bytesRead;
    }
    
    if (packetPos == packetLength) {
        storePacket();
    }
    
    return bytesRead;
}

void LOGGER_storeAvailablePackets() {
    while(nextPacket());
}
