// Sender program that sends messages every 100 ms

#include <logger.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: sender_second message\n");
        return 1;
    }
    int rc = logging_open("program2");
    if (rc) {
        printf("Error while connecting to DB\n");
        return 1;
    }

    int i = 0;

    while (i < 100) {
        rc = log(argv[1], LOG_FATAL);
        if (rc) {
            printf("Error while logging\n");
            return 1;
        }
        usleep(100000);
        i++;
    }

    logging_close();

    return 0;
}