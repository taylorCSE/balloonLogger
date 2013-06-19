/**
 * \file comm.cpp
 * \brief Handles serial communication
 */

#include "comm.h"

using namespace std;

/// Defualt Comm values
string COMM_PORT = "com1";
string COMM_BAUD = "4800";
string COMM_PROTO = "8N1";


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

void COMM_GetAvailablePorts(std::vector<std::string>& portsStrVec) {
    ctb::GetAvailablePorts(portsStrVec);
}
