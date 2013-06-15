#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>

#define DBUG_OFF

#include <vector>
#include <map>
#include <string>

#include <my_global.h>                
#include <mysql.h>

extern FILE * DB_log;

extern std::string DB_USER;
extern std::string DB_PASS;
extern std::string DB_HOST;
extern std::string DB_NAME;

extern std::string DB_STATUS;

void DB_connect();
void DB_query(char * item ...);
char* DB_resultAsText();
bool DB_isQueryReady();

#endif