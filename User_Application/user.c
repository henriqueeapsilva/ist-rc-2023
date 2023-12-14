#define _GNU_SOURCE

#include "user.h"


char *asip = "127.0.0.1";
char *asport = "58045";
char *INITIAL = "\ncommands:\n login [UID] [password]\n open [name] [asset_name] [start_value] [timeactive]\n close [AID]\n myaction or ma\n mybids or mb \n list or l\n show_asset [AID] or sa [AID]\n bid [AID] [value] or b [AID] [value]\n show_record [AID] or sr [AID]\n logout\n unregister\n exit\n";



int main(int argc, char **argv){

    int opt;
    char buffer[MAX_BUFFER_SIZE];
    char command[20];

    while ((opt = getopt(argc, argv, "n:p:")) != -1) {
        switch (opt) {
            case 'n':
                asip = optarg;
                break;
            case 'p':
                asport = optarg;
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-n ASIP] [-p ASport]\n", argv[0]);
                exit(EXIT_SUCCESS);
        }
    }
    while (1){
        printf("%s\n > ", INITIAL);
    
        // decide which command the user wants
        fgets(buffer, sizeof(buffer), stdin);
        // access the command to execute
        sscanf(buffer, "%s", command);
        //executes the command
        executeCommand(command, buffer);
    }
}