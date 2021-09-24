#include <logger.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: sender_second message\n");
        return 1;
    }
    int rc = logging_init(argv[0]);
    if (rc) {
        printf("Error while connecting to DB\n");
        return 1;
    }

    int i = 0;

    while (i < 15) {
        rc = log_stuff(argv[1], LOG_FATAL);
        if (rc) {
            printf("Error while logging\n");
            return 1;
        }
        sleep(1);
        i++;
    }

    logging_shutdown();

    return 0;
}