/**
 * \file Database.cpp
 * \brief Handles database communication
 *
 * This is absolutely not thread safe. If calls are being made from 
 * multiple threads, behavior will be unexpected.
 */

#include "database.h"

using namespace std;

/// Globals to maintain database connection
MYSQL *DB_conn = NULL;
MYSQL_RES *DB_result = NULL;
FILE * DB_log = NULL;

/// Defualt database values
string DB_USER = "root";
string DB_PASS = "";
string DB_HOST = "127.0.0.1";
string DB_NAME = "balloontrack";
string DB_FLIGHT_ID = "MyFlightId";

/// Database status
string DB_STATUS = "Not initialized";

/// Small buffer to use in various calls
char DB_buf[16384];

int DB_PACKET_ID = 0;

/**
    Connect to the database
*/

void DB_connect() {
    DB_log = fopen("database.log","w");

    DB_conn = mysql_init(NULL);
    
    if (DB_conn == NULL) {
        // Connection to mysql library has failed
        fprintf(DB_log, "Error %u: %s\n", mysql_errno(DB_conn), 
                                          mysql_error(DB_conn));
        DB_STATUS = "MySQL init error";
        return;
    }

    if (mysql_real_connect(DB_conn, DB_HOST.c_str(), DB_USER.c_str(), 
                                    DB_PASS.c_str(), DB_NAME.c_str(),
                                    0, NULL, 0) == NULL) {
        // Connection to server and/or database has failed
        fprintf(DB_log,"Error %u: %s\n", mysql_errno(DB_conn), 
                                         mysql_error(DB_conn));
        DB_STATUS = "Connection Failed";
        DB_conn = 0x00;
        return;
    }
    
    fprintf(DB_log,"Database connecton established.\n");
    DB_STATUS = "Connected to DB";
}

/**
    Query the database
    
    This isn't particularly secure if a user is able to enter elements 
    of the query. It simply takes the supplied items and puts them 
    together as a query.
    
    The following are allowes as part of the format string:
    
    %s - String (char*)
    %d - Integer
    %f - Float
    %c - Character
    
    Example call:
    
    DB_query("select * from users where name = '%s'",name);
    
    In the preceeding example, %s will be replaced by the value in name 
    and the query will be executed.
*/

void DB_query(char* item ...) {
    char query[1024];
    int i = 0;
    
    query[0] = 0x00;
    
    /// Connect to the DB if necessary
    
    if(!DB_conn) DB_connect();
    if(!DB_conn) return;
    
    /// Assemble the query

    va_list v;
    va_start(v, item);
    
    while( item[i] != 0x00 && i < 1000) {
        if(item[i] == '%' && item[i+1] == 'd') {
            sprintf(query,"%s%d",query,va_arg(v,int));
            i++;
        } else if(item[i] == '%' && item[i+1] == 'u') {
            sprintf(query,"%s%u",query,va_arg(v,unsigned int));
            i++;
        } else if(item[i] == '%' && item[i+1] == 'f') {
            sprintf(query,"%s%f",query,va_arg(v,double));
            i++;
        } else if(item[i] == '%' && item[i+1] == 's') {
            sprintf(query,"%s%s",query,va_arg(v,char*));
            i++;
        } else {
            sprintf(query,"%s%c",query,item[i]);
        }
        i++;        
    }
    
    va_end(v);
    fprintf(DB_log, "Executing Query: %s\n",query);

    /// Execute the query
    
    if(!mysql_query(DB_conn, query)) {
        DB_result = mysql_store_result(DB_conn);
        if (DB_result) {
            fprintf(DB_log, "Rows Returned: %d\n",(int)mysql_num_rows(DB_result));
        } else {
            fprintf(DB_log, "Query executed successfully\n");            
        }
    } else {
        fprintf(DB_log, "Error querying database.\n");
        DB_result = 0x00;
    }
}

/**
    Get the query result as text.
    
    This returns a character pointer to the buffer holding the result.
*/

char* DB_resultAsText() {
    if(!DB_isQueryReady()) return((char*)"");

    int i = 0;
    MYSQL_ROW row;
    int num_fields;

    DB_buf[0] = 0x00;
    
    num_fields = mysql_num_fields(DB_result);
    
    while ((row = mysql_fetch_row(DB_result)))
    {
      for(i = 0; i < num_fields; i++)
      {
          sprintf(DB_buf,"%s%s ",DB_buf, row[i] ? row[i] : "NULL");
      }
      sprintf(DB_buf,"%s\n",DB_buf);
    }
    
    mysql_free_result(DB_result);

    return DB_buf;
}

/**
    Test to see if there is a query ready.
*/

bool DB_isQueryReady() {
    if(!DB_conn) return false;
    if(!DB_result) return false;
    return true;
}

void DB_addGpsPacket(uint16_t deviceId, uint8_t status, uint8_t altitude, uint8_t rate, char* lat, char* latRef, char* lon, char* lonRef, char* spd, char* hdg) {
    DB_query((char*)"INSERT INTO gps "
                "(FlightId, DeviceId, PacketId, Timestamp, Status, " 
                "Altitude, Rate, Lat, LatRef, Lon, LonRef, Spd, Hdg)"
             "VALUES ('%s', %d, %d, FROM_UNIXTIME(%d), %d, %d, %d,"
                "'%s', '%s', '%s', '%s', '%s', '%s')",
             DB_FLIGHT_ID.c_str(), deviceId, DB_PACKET_ID, time(0), status, altitude, rate, 
                lat, latRef, lon, lonRef, spd, hdg);
    
    DB_PACKET_ID++;
}

void DB_addDataPacket(uint16_t deviceId, int DI, int altitude, int rate, uint16_t analog[18]) {
    fprintf(DB_log, "deviceId: %u\n",deviceId);

    DB_query((char*)"INSERT INTO aip "
                "(FlightId, DeviceId, PacketId, TimesStamp, DI, Altitude, Rate," 
                "A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18) "
             "VALUES ('%s', %u, %d, FROM_UNIXTIME(%d), %d, %d, %d, "
                "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
             DB_FLIGHT_ID.c_str(), deviceId, DB_PACKET_ID, time(0), DI, altitude, rate,
             analog[0],
             analog[1],
             analog[2],
             analog[3],
             analog[4],
             analog[5],
             analog[6],
             analog[7],
             analog[8],
             analog[9],
             analog[10],
             analog[11],
             analog[12],
             analog[13],
             analog[14],
             analog[15],
             analog[16],
             analog[17]
            );
            
    DB_PACKET_ID++;
}