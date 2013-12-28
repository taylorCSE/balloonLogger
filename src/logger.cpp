/**
 * \file logger.cpp
 * \brief Parses serial data and logs to database
 */

#include "logger.h"

using namespace std;

const char SYNC_BYTE = 0xEE;
const char DATA_PACKET = 0x2A;
const char GPS_PACKET = 0x1A;
const int packetLength = 46;

struct header {
    uint16_t id;
    uint8_t cmd;
};

struct gps_packet {
    uint16_t id;
    uint8_t cmd;
    uint8_t status;
    uint16_t altitude;
    uint16_t rate;
    char gps[36];
};

struct data_packet {
    uint16_t id;
    uint8_t cmd;
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
} packetBuf;

LOGGER_State_t LOGGER_state;

/**
 * Reverses byte order in a word
**/
uint16_t flipBytes(uint16_t in) {
    uint16_t hibyte = (in & 0xff00) >> 8;
    uint16_t lobyte = (in & 0xff);
    
    return lobyte << 8 | hibyte;
}

/**
 * Convert NMEA GPS value to degrees.minutes
 * 
 * NMEA format: dddmm.mmmmm or ddmm.mmmmm
 *
 * Conversion: dd + (mm.mmmmm/60)
**/
float convertNMEA(char* nmea) {
    char *startMinutes = strchr(nmea, '.');
    int hours;
    float minutes;
    
    if (!startMinutes) {
        return 0;
    } else {
        startMinutes -= 2;
        
        sscanf(startMinutes, "%f", &minutes);
        
        *startMinutes = 0x00;
        
        sscanf(nmea, "%d", &hours);
        
        return hours + (minutes/60);
    }
}

void storeGpsPacket() {
    packetBuf.gpsPacket.altitude = flipBytes(packetBuf.gpsPacket.altitude);
    packetBuf.gpsPacket.rate = flipBytes(packetBuf.gpsPacket.rate);
    
    LOGGER_state.gpsStatus = packetBuf.gpsPacket.status;
    LOGGER_state.altitude = packetBuf.gpsPacket.altitude;
    LOGGER_state.rate = packetBuf.gpsPacket.rate;
    
    char* parts[6];
    float lat, lon;
    
    parts[0] = packetBuf.gpsPacket.gps;
    
    int part = 1;
    
    for (int i = 0; i < 36; i++) {
        if (packetBuf.gpsPacket.gps[i] == ',') {
            packetBuf.gpsPacket.gps[i] = 0x00;
            parts[part] = packetBuf.gpsPacket.gps + i + 1;
            part++;
        }
    }
    
    if (part != 6) {
        printf("Invalid GPS string");
    } else {
        // Properly terminate last entry
        for(int i = 0; i < 7; i++) {
            if (parts[5][i] < '.' || parts[5][i] > '9') {
                parts[5][i] = 0x00;
            }
        }
        
        lat = convertNMEA(parts[0]);
        lon = convertNMEA(parts[2]);
        
        sprintf(LOGGER_state.position, "%f %s %f %s", lat, parts[1], lon, parts[3]);
        sprintf(LOGGER_state.spd, "%s", parts[4]);
        sprintf(LOGGER_state.hdg, "%s", parts[5]);
        
        DB_addGpsPacket(
            packetBuf.gpsPacket.id,
            packetBuf.gpsPacket.status,
            packetBuf.gpsPacket.altitude,
            packetBuf.gpsPacket.rate,
            lat, parts[1], lon, parts[3], parts[4], parts[5]
        );
    }
}

void storeDataPacket() {
    packetBuf.dataPacket.altitude = flipBytes(packetBuf.dataPacket.altitude);
    packetBuf.dataPacket.rate = flipBytes(packetBuf.dataPacket.rate);
    
    for(int i = 0; i < 18; i++) {
        packetBuf.dataPacket.analog[i] = flipBytes(packetBuf.dataPacket.analog[i]);
    }

    LOGGER_state.digital = packetBuf.dataPacket.digital;
    LOGGER_state.altitude = packetBuf.dataPacket.altitude;
    LOGGER_state.rate = packetBuf.dataPacket.rate;
    
    DB_addDataPacket(
        packetBuf.dataPacket.id,
        packetBuf.dataPacket.digital,
        packetBuf.dataPacket.altitude,
        packetBuf.dataPacket.rate,
        packetBuf.dataPacket.analog
    );
}

/** Stores a complete packet in the appropriate database table(s)
 *
 * This also updates the global logger state with the most recent data
 * for displaying to the user.
**/

void storePacket() {
    LOGGER_state.packetsRead++;
    
    packetBuf.header.id = flipBytes(packetBuf.header.id);
    
    LOGGER_state.lastId = packetBuf.header.id;
    LOGGER_state.lastCmd = packetBuf.header.cmd;
    
    if (packetBuf.header.cmd == GPS_PACKET) {
        storeGpsPacket();
    }
    
    if (packetBuf.header.cmd == DATA_PACKET) {
        storeDataPacket();
    }
    
    for (int i = 0; i < 64; i++) {
        packetBuf.raw[i] = 0x00;
    }
}

/** Reads a chunk of data from the serial port
 * 
 * If the new chunk completes a packet, the packet is parsed and stored.
 *
 * This function is non-blocking. It grabs available serial data and waits
 * to be called again when more data is available.
 *
 * \return int number of bytes read
**/

int nextChunk() {
    static int syncBytesSeen = 0;
    static int packetPos = 0;
    
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
        packetPos = 0;
        syncBytesSeen = 0;
        storePacket();
    }
    
    LOGGER_state.bytesRead += bytesRead;
    
    if (bytesRead > 0) {
        LOGGER_state.lastSerialData = time(0);
    }
    
    return bytesRead;
}

/** Reads available data from the serial port and stores in the database as necessary */
int LOGGER_storeAvailablePackets() {
    int startingPackets = LOGGER_state.packetsRead;
    
    while(nextChunk());
    
    return LOGGER_state.packetsRead - startingPackets;
}