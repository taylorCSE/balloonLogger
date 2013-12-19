#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>

#define DBUG_OFF

#include <vector>
#include <map>
#include <string>

#include <my_global.h>                
#include <mysql.h>
#include <time.h>

extern FILE * DB_log;

extern std::string DB_USER;
extern std::string DB_PASS;
extern std::string DB_HOST;
extern std::string DB_NAME;
extern std::string DB_FLIGHT_ID;

extern std::string DB_STATUS;

void DB_connect();
void DB_query(char * item ...);
char* DB_resultAsText();
bool DB_isQueryReady();
void DB_addGpsPacket(uint16_t deviceId, int status, char* lat, char* latRef,
    char* lon, char* lonRef, char* spd, char* hdg);
void DB_addDataPacket(uint16_t deviceId, int DI, int altitude, int rate, uint16_t analog[18]);

#endif
