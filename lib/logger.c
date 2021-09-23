#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "logger.h"

#define OUT_LOG_FILE "/var/log/task_loglib.db"

static char * g_program_name = NULL;
static sqlite3 *g_db;
static char *zErrMsg = 0;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for(i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

static int log_create_db() {
    int rc = sqlite3_exec(g_db,
         "create table if not exists Log(" 
         "message TEXT, "
         "warn_level INTEGER, "
         "datetime TEXT, "
         "program TEXT, "
         "file_name TEXT, "
         "line_num INTEGER"
         ")"
         ,
         callback,
         0,
         &zErrMsg);
    if( rc!=SQLITE_OK ) {
        fprintf(stderr, "%d\n", rc);
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        fprintf(stderr, "Failed to create the database table, aborting...\n");
        sqlite3_free(zErrMsg);
        sqlite3_close(g_db);
        return 1;
    }
    return 0;
}

int logging_init(const char* program_name) {
    g_program_name = (char*)malloc(256);
    strncpy(g_program_name, program_name, 256);

    int rc = sqlite3_open(OUT_LOG_FILE, &g_db);
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(g_db));
        sqlite3_close(g_db);
        return 1;
    }
    rc = log_create_db();
    if( rc ) {
        return 1;
    }
    return 0;

}

int log_send_message(const char* message, int warning_level, const char* file_name, int line_num) {
    
    char sql_command[1024] = "";
    char buffer[256] = "";
    time_t now = time(NULL);

    strncat(sql_command, "insert into Log(message, warn_level, datetime, program, file_name, line_num) values (\"", 1023);
    
    // add log message
    strncat(sql_command, message, sizeof(sql_command) - 1);

    // add warning level
    strncat(sql_command,  "\", " , sizeof(sql_command) - 1); // ", \"2020\", \"main\", \"23.c.4\")"
    snprintf(buffer, sizeof(buffer) - 1, "%d", warning_level);
    strncat(sql_command,  buffer, sizeof(sql_command) - 1);

    // add date time
    struct tm *t = localtime(&now);
    strftime(buffer, sizeof(buffer)-1, "%Y-%m-%d %H:%M", t);
    printf("Current Date: %s", buffer);
    strncat(sql_command,  ", \"" , sizeof(sql_command) - 1);
    strncat(sql_command,  buffer, sizeof(sql_command) - 1);

    // add program
    strncat(sql_command,  "\", \"" , sizeof(sql_command) - 1);
    strncat(sql_command,  g_program_name, sizeof(sql_command) - 1);

    // add a file name
    strncat(sql_command,  "\", \"" , sizeof(sql_command) - 1);
    strncat(sql_command,  file_name, sizeof(sql_command) - 1);

    // add a line number
    strncat(sql_command,  "\", " , sizeof(sql_command) - 1); // ", \"2020\", \"main\", \"23.c.4\")"
    snprintf(buffer, sizeof(buffer) - 1, "%d", line_num);
    strncat(sql_command,  buffer, sizeof(sql_command) - 1);

    strncat(sql_command,  ")" , sizeof(sql_command) - 1);

    //printf("%s\n", sql_command);

    int rc = sqlite3_exec(g_db,
        sql_command,
        callback,
        0,
        &zErrMsg);

    if( rc!=SQLITE_OK ) {
        fprintf(stderr, "%d\n", rc);
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        fprintf(stderr, "Failed to send a message request to logging server, aborting..\n");
        sqlite3_free(zErrMsg);
        sqlite3_close(g_db);
        return 1;
    }
    return 0;
}

int logging_shutdown() {
    int rc = sqlite3_close(g_db);
    if( rc!=SQLITE_OK ) {
        fprintf(stderr, "%d\n", rc);
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        fprintf(stderr, "Failed to close the database, aborting...\n");
        sqlite3_free(zErrMsg);
        return 1;
    }
    sqlite3_free(zErrMsg);
    return 0;
}
