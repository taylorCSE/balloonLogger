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

ctb::SerialPort* serialPort = 0;

int wait = 0;

void COMM_GetAvailablePorts(std::vector<std::string>& portsStrVec) {
    ctb::GetAvailablePorts(portsStrVec);
}

void COMM_open(string port) {
    COMM_PORT = port;
    
    if( serialPort ) {
        serialPort->Close();

        delete serialPort;
        
        serialPort = 0;
    }
    
    serialPort = new ctb::SerialPort();

    serialPort->Open(COMM_PORT.c_str(), atoi(COMM_BAUD.c_str()), 
                    COMM_PROTO.c_str(), 
                    ctb::SerialPort::NoFlowControl);
}

/**
 * COMM_GetData
 *
 * Non-blocking method to retreive available data from configured comm port
 */
int COMM_GetData(char * buf, int len) {
    if( !serialPort || time(0) < wait) {
        return 0;
    }

    int bytesRead = serialPort->Read( buf, len);
    
    if (bytesRead < 0) {
        printf("Attempting to reconnect...\n");
        COMM_open(COMM_PORT);
        wait = time(0) + 10; // Introduce a delay to work around a reconnect bug
    }

    return bytesRead;
}
