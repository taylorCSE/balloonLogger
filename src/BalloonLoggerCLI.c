/**
 * \file BalloonLoggerCLI.cpp
 * \brief Main Application Code
 *
 * This is the entry point for the application.
 */

#include "logger.h"
#include "comm.h"

int main() {
    COMM_open();
    
    while(1) {
        LOGGER_storeAvailablePackets();
    }
}