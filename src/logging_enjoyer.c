#include "logger.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: logging_enjoyer.o message");
    }
    logging_init(argv[0]);

    int i = 0;

    while (i < 30) {
        LOG_DEBUG;
        int rc = log_stuff(argv[1], LOG_INFO);
        if (rc) {
            printf("Error while logging");
            return 1;
        }
        sleep(1);
    }

    logging_shutdown();
}