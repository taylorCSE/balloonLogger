/**
 * \file BalloonLoggerCLI.cpp
 * \brief Main Application Code
 *
 * This is the entry point for the application.
 */

#include <stdio.h>

#include "logger.h"
#include "comm.h"
#include "version.h"

int main() {
    printf("BalloonLoggerCLI %s\n", VERSION);
    
    COMM_open("com6");
    
    while(1) {
        int packetsRead = LOGGER_storeAvailablePackets();
        
        if (packetsRead > 0) {
            printf("%d packets read (%d bytes)\n", LOGGER_state.packetsRead, LOGGER_state.bytesRead);
        }
    }
}