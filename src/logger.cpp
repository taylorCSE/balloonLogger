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
    LOGGER_state.gpsStatus = packetBuf.gpsPacket.payload.status;
    LOGGER_state.altitude = packetBuf.gpsPacket.payload.altitude;
    LOGGER_state.rate = packetBuf.gpsPacket.payload.rate;
    
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
    LOGGER_state.digital = packetBuf.dataPacket.payload.digital;
    LOGGER_state.altitude = packetBuf.dataPacket.payload.altitude;
    LOGGER_state.rate = packetBuf.dataPacket.payload.rate;
    LOGGER_state.lastId = packetBuf.dataPacket.header.id
    LOGGER_state.lastCmd = packetBuf.dataPacket.header.cmd

    /*
    char tmp[37];
    
    write("analog values:");
    
    for (int i = 0; i < 18; i++) {
        sprintf(tmp, "%d", packetBuf.dataPacket.payload.analog[i]);
        write(tmp);
    }
    */

    DB_addDataPacket(
        packetBuf.dataPacket.header.id,
        packetBuf.dataPacket.payload.digital,
        packetBuf.dataPacket.payload.altitude,
        packetBuf.dataPacket.payload.rate,
        packetBuf.dataPacket.payload.analog
    );
}

void storePacket() {
    packetPos = 0;
    syncBytesSeen = 0;
    lastPacket = packetBuf;
    
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

void LOGGER_storeAvailablePackets() {
    while(nextPacket());
}

int LOGGER_lastPacketType() {
    return lastPacket.header.cmd;
}

int LOGGER_lastPacketId() {
    return lastPacket.header.cmd;
}