#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "logger.h"

#define OUT_LOG_FILE "/var/log/c_program_logs.db"

#define SIZE_PROGRAM_NAME 128

#define PRINT_SQLITE_ERROR(ERR_CODE, MESSAGE) fprintf(stderr, "Error code: %d\n", ERR_CODE); \
        fprintf(stderr, "sqlite3 error: %s\n", sqlite3_errmsg(g_db)); \
        fprintf(stderr, "%s\n", MESSAGE);

static char g_program_name[SIZE_PROGRAM_NAME] = "";
static sqlite3 *g_db = NULL;
static char *z_err_msg = NULL;

int parse_warning_lvl(int warning_level, char *dest)
{
    switch (warning_level) {
        case LOG_DEBUG:
            strcpy(dest, "[DEBUG]");
            break;

        case LOG_INFO:
            strcpy(dest, "[INFO]");
            break;

        case LOG_WARN:
            strcpy(dest, "[WARNING]");
            break;

        case LOG_ERROR:
            strcpy(dest, "[ERROR]");
            break;

        case LOG_FATAL:
            strcpy(dest, "[FATAL]");
            break;

        default:
            return LOGLIG_WARNING_LEVEL_FAIL;
    }
    return LOGLIB_OK;
}

static int callback(void *not_used, int argc, char **argv, char **az_col_name)
{
    int i;
    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", az_col_name[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static int log_create_db()
{
    int sqlite_rc = sqlite3_exec(g_db,
         "create table if not exists Log(" 
         "message TEXT, "
         "warn_level TEXT, "
         "datetime TEXT, "
         "program TEXT, "
         "file_name TEXT, "
         "line_num INTEGER"
         ")"
         ,
         callback,
         0,
         &z_err_msg);
    if (sqlite_rc != SQLITE_OK) {
        PRINT_SQLITE_ERROR(sqlite_rc, "Failed to create the database table, aborting...");
        return LOGLIB_DATABASE_CREATION_FAIL;
    }
    return LOGLIB_OK;
}

int logging_open(const char* program_name)
{
    int lib_rc = LOGLIB_OK;
    if (program_name != NULL) {
        strncpy(g_program_name, program_name, 128);
    }

    int sqlite_rc = sqlite3_open(OUT_LOG_FILE, &g_db);
    if (sqlite_rc != SQLITE_OK) {
        PRINT_SQLITE_ERROR(sqlite_rc, "Failed while opening the database");
        sqlite3_close(g_db);
        return LOGLIB_OPEN_FAIL;
    }
    lib_rc = log_create_db();
    if (lib_rc != LOGLIB_OK) {
        return lib_rc;
    }
    return lib_rc;
}

int log_message(const char* message, int warning_level, const char* file_name, int line_num)
{
    int lib_rc = LOGLIB_OK;

    char sql_command[1024] = "";
    char current_time[128] = "";
    char warning_text[32] = "";

    lib_rc = parse_warning_lvl(warning_level, warning_text);
    if (lib_rc != LOGLIB_OK) {
        return lib_rc;
    }
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(current_time, sizeof(current_time) - 1, "%Y-%m-%d %H:%M", t);

    snprintf(sql_command, sizeof(sql_command) - 1, 
        "insert into Log(message, warn_level, datetime, program, file_name, line_num) "
        "values"
        "("
        "\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", %d"
        ")",
        message, warning_text, current_time, g_program_name, file_name, line_num);

    printf("%s\n", sql_command);

    int sqlite_rc = sqlite3_exec(g_db,
        sql_command,
        callback,
        0,
        &z_err_msg);

    if (sqlite_rc != SQLITE_OK) {
        PRINT_SQLITE_ERROR(sqlite_rc, "Failed to send a message to the sqlite server, aborting...");
        sqlite3_free(z_err_msg);
        sqlite3_close(g_db);
        return LOGLIB_MESSAGE_STORE_FAIL;
    }
    return lib_rc;
}

int logging_close()
{
    int sqlite_rc = sqlite3_close(g_db);
    if(sqlite_rc != SQLITE_OK) {
        PRINT_SQLITE_ERROR(sqlite_rc, "Failed to close the database...");
        sqlite3_free(z_err_msg);
        return 1;
    }
    sqlite3_free(z_err_msg);
    return LOGLIB_OK;
}


int get_logs(const char* program_name)
{
    char sql_command[256] = "select * from Log";
    if (program_name != NULL) {
        snprintf(sql_command, sizeof(sql_command) - 1, "select * from Log where program = \"%s\"", program_name);
    }

    int sqlite_rc = sqlite3_exec(g_db,
        sql_command,
        callback,
        0,
        &z_err_msg);

    if (sqlite_rc != SQLITE_OK) {
        PRINT_SQLITE_ERROR(sqlite_rc, "Failed to retrieve log information from logging server, aborting...");
        sqlite3_free(z_err_msg);
        sqlite3_close(g_db);
        return LOGLIB_GET_FAIL;
    }

    return LOGLIB_OK;
}

void fprint_error(FILE* stream, int error_code)
{
    switch (error_code) {
        case LOGLIB_OK:
            fprintf(stream, "Misuse of error printing: LOGLIB_OK signal found\n");
            break;

        case LOGLIG_WARNING_LEVEL_FAIL:
            fprintf(stream, "Error while parsing warning level\n");
            break;

        case LOGLIB_DATABASE_CREATION_FAIL:
            fprintf(stream, "Error while creating Logging table in the database\n");
            break;

        case LOGLIB_GET_FAIL:
            fprintf(stream, "Error while searching for logs\n");
            break;

        case LOGLIB_MESSAGE_STORE_FAIL:
            fprintf(stream, "Error while storing the message into logging database\n");
            break;

        case LOGLIB_CLOSE_FAIL:
            fprintf(stream, "Failed to close the database\n");
            break;

        case LOGLIB_OPEN_FAIL:
            fprintf(stream, "Failed to open the database\n");
            break;

        default:
            fprintf(stream, "No such error message found\n");
    }
}