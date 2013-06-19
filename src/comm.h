#ifndef COMM_H
#define COMM_H

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <map>
#include <string>
#include <ctb-0.16/ctb.h>

extern std::string COMM_PORT;
extern std::string COMM_BAUD;
extern std::string COMM_PROTO;
extern std::string COMM_AVAILABLE_PORTS[];

void COMM_GetAvailablePorts(std::vector<std::string>& portsStrVec);

#endif
