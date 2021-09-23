#include "logger.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: logging_fan.o message");
    }
    logging_init(argv[0]);

    int i = 0;

    while (i < 100000) {
        LOG_DEBUG;
        int rc = log_stuff(argv[1], LOG_FATAL);
        if (rc) {
            printf("Error while logging");
            return 1;
        }
        usleep(100);
    }

    logging_shutdown();

    return 0;
}