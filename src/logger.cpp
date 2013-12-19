/**
 * \file logger.cpp
 * \brief Parses serial data and logs to database
 */

#include "logger.h"

using namespace std;

const char SYNC_BYTE = 0xEE;
const char DATA_PACKET = 0x2A;
const char GPS_PACKET = 0x1A;

struct header {
    uint16_t id;
    uint8_t cmd;
};

struct gps_packet {
    struct header header;
    uint8_t status;
    uint16_t altitude;
    uint16_t rate;
    char gps[36];
};

struct data_packet {
    struct header header;
    uint8_t digital;
    uint16_t altitude;
    uint16_t rate;
    uint16_t analog[18];
};

/**
 * Stores raw packet data
 * 
 * This union provides several different views into the raw data 
 * depending on the type of packet.
**/

union PacketBuf {
    char raw[64];
    struct header header;
    struct data_packet dataPacket;
    struct gps_packet gpsPacket;
};

union PacketBuf lastPacket;
union PacketBuf packetBuf;
const int packetLength = 46;
int packetPos = 0;
int syncBytesSeen = 0;

LOGGER_State_t LOGGER_state;

void write(char* msg) {
    FILE *fp;

    fp = fopen("log.txt","a");
    fprintf(fp,"%s\n",msg);
    fclose(fp);     
}

void storeGpsPacket() {
    LOGGER_state.gpsStatus = packetBuf.gpsPacket.status;
    LOGGER_state.altitude = packetBuf.gpsPacket.altitude;
    LOGGER_state.rate = packetBuf.gpsPacket.rate;
    
    /*
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
    */
}

void storeDataPacket() {
    LOGGER_state.digital = packetBuf.dataPacket.digital;
    LOGGER_state.altitude = packetBuf.dataPacket.altitude;
    LOGGER_state.rate = packetBuf.dataPacket.rate;

    DB_addDataPacket(
        packetBuf.dataPacket.header.id,
        packetBuf.dataPacket.digital,
        packetBuf.dataPacket.altitude,
        packetBuf.dataPacket.rate,
        packetBuf.dataPacket.analog
    );
}

void storePacket() {
    packetPos = 0;
    syncBytesSeen = 0;
    lastPacket = packetBuf;
    LOGGER_state.packetsRead++;
    
    LOGGER_state.lastId = packetBuf.header.id;
    LOGGER_state.lastCmd = packetBuf.header.cmd;
    
    if (packetBuf.header.cmd == GPS_PACKET) {
        storeGpsPacket();
    }
    
    if (packetBuf.header.cmd == DATA_PACKET) {
        storeDataPacket();
    }
}

int nextPacket() {
    int bytesRead = 0;
    
    if (packetPos == 0 && syncBytesSeen != 2) {
        bytesRead = COMM_GetData(packetBuf.raw, 1);
        
        if(bytesRead < 0) return 0;
        
        if (bytesRead == 1) {
            if (packetBuf.raw[0] == SYNC_BYTE) {
                syncBytesSeen += 1;
            } else {
                syncBytesSeen = 0;
            }
        }
    } else {
        bytesRead = COMM_GetData(packetBuf.raw + packetPos, packetLength - packetPos);

        if(bytesRead < 0) return 0;

        packetPos += bytesRead;
    }
    
    if (packetPos == packetLength) {
        storePacket();
    }
    
    LOGGER_state.bytesRead += bytesRead;
    
    return bytesRead;
}

int LOGGER_storeAvailablePackets() {
    int startingPackets = LOGGER_state.packetsRead;
    
    while(nextPacket());
    
    return LOGGER_state.packetsRead - startingPackets;
}