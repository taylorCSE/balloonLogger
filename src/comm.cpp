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

/** Fills the passed string vector with comm ports available for use */

void COMM_GetAvailablePorts(std::vector<std::string>& portsStrVec) {
    ctb::GetAvailablePorts(portsStrVec);
}

/** 
 * Opens the specified comm port 
 *
 * This will close any currently open port and connect to the newly specified port.
 *
 * \param port string specifying the port to open
**/

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
 * Non-blocking method to retreive available data from configured comm port 
 *
 * \param buf space to store data
 * \param len maximum amount of data to read into buffer
 * \return Number of bytes read on this call
**/

int COMM_GetData(char * buf, int len) {
    static int wait = 0;

    if( !serialPort || time(0) < wait) {
        return 0;
    }

    int bytesRead = serialPort->Read( buf, len);
    
    if (bytesRead < 0) {
        COMM_open(COMM_PORT);
        wait = time(0) + 10; // Introduce a delay to work around a reconnect bug
    }

    return bytesRead;
}
