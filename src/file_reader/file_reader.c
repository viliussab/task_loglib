#include <stdio.h>
#include <string.h>
#include <logger.h>

int main(int argc, char *argv[]) {
    int rc;
    char* filename = NULL;

    if (argc != 3 && argc != 1) {
        printf("Launch the program without arguments or with --program <program_name>\n");
        return 1;
    }
    if (argc == 3) {
        if (strcmp(argv[1], "--program") == 0 || strcmp(argv[1], "--P")) {
            filename = argv[2];
        }
        else {
            printf("Launch the program without arguments or with --program <program_name>\n");
            return 1;
        }
    }

    logging_init("");
    if (filename != NULL) {
        printf("Searching logs of program name %s\n", filename);
        rc = get_program_logs(filename);
    }
    else {
        rc = get_logs();
    }
    
    if (rc) {
        printf("Error has occured\n");
        return 1;
    }
    
    logging_shutdown();
    return 0;
}