#define LOG_DEBUG 10
#define LOG_INFO 20
#define LOG_WARN 30
#define LOG_ERROR 40
#define LOG_FATAL 50

int logging_init(const char* program_name);
int log_send_message(const char* message, int warning_level, const char* file_name, int line_num);
int logging_shutdown();
int get_program_logs(const char* program_name);
int get_logs();

#define log_stuff(MESSAGE, LOG_LEVEL) log_send_message(MESSAGE, LOG_LEVEL, __FILE__, __LINE__)