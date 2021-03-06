// Sender program that sends messages every second

#include <stdio.h>
#include <unistd.h>

#include <logger.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: sender_first message\n");
        return 1;
    }
    int rc = logging_open("program1");
    if (rc) {
        printf("Error while connecting to DB");
        return 1;
    }

    int i = 0;
    while (i < 30) {
        int rc = 0;
        log(argv[1], LOG_INFO);
        if (rc) {
            printf("Error while logging");
            return 1;
        }
        sleep(1);
        i++;
    }

    //logging_shutdown();
}