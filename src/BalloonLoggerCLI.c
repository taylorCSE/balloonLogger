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
    
    COMM_open();
    
    while(1) {
        LOGGER_storeAvailablePackets();
    }
}