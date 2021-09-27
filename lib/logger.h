#ifndef LOGGER_H
#define LOGGER_H

#define LOG_DEBUG 1
#define LOG_INFO 2
#define LOG_WARN 3
#define LOG_ERROR 4
#define LOG_FATAL 5

#define LOGLIB_OK 0
#define LOGLIG_WARNING_LEVEL_FAIL 1
#define LOGLIB_DATABASE_CREATION_FAIL 2
#define LOGLIB_GET_FAIL 3
#define LOGLIB_MESSAGE_STORE_FAIL 4
#define LOGLIB_CLOSE_FAIL 5
#define LOGLIB_OPEN_FAIL 6

int logging_open(const char* program_name);
int log_message(const char* message, int warning_level, const char* file_name, int line_num);
int logging_close();
//int get_program_logs(const char* program_name);
int get_logs();
void fprint_error(FILE* stream, int error_code);

#define log(MESSAGE, LOG_LEVEL) log_message(MESSAGE, LOG_LEVEL, __FILE__, __LINE__)
#define print_error(error_code) fprint_error(stdout, error_code)

#endif