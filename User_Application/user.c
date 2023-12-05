#define _GNU_SOURCE

#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>


char *ASIP = "tejo.tecnico.ulisboa.pt";
char *ASport = "58045";
char *INITIAL = "commands:\n login [UID] [password]\n open [name] [asset_name] [start_value] [timeactive]\n close [AID]\n myaction or ma\n mybids or mb \n list or l\n show_asset [AID] or sa [AID]\n bid [AID] [value] or b [AID] [value]\n show_record [AID] or sr [AID]\n logout\n unregister\n exit\n";



int main(int argc, char **argv){

    int opt;
    char buffer[100];
    char command[20];

    while ((opt = getopt(argc, argv, "n:p:")) != -1) {
        switch (opt) {
            case 'n':
                ASIP = optarg;
                break;
            case 'p':
                ASport = optarg;
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-n ASIP] [-p ASport]\n", argv[0]);
                exit(EXIT_SUCCESS);
        }
    }
    while (1){
        printf("%s", INITIAL);

    
        // decide which command the user wants
        fgets(buffer, sizeof(buffer), stdin);
        // access the command to execute
        sscanf(buffer, "%s", command);
        //executes the command
        executeCommand(command, buffer);
    }
}