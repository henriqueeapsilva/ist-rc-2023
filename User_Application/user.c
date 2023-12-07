#define _GNU_SOURCE

#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>


char *ASIP = "193.136.138.142";
char *ASport = "58011";
char *INITIAL = "commands:\n login [UID] [password]\n open [name] [asset_name] [start_value] [timeactive]\n close [AID]\n myaction or ma\n mybids or mb \n list or l\n show_asset [AID] or sa [AID]\n bid [AID] [value] or b [AID] [value]\n show_record [AID] or sr [AID]\n logout\n unregister\n exit\n";



int main(int argc, char **argv){

    int opt, response;
    char buffer[MAX_RESPONSE_SIZE];
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
        printf("%s\n > ", INITIAL);
    
        // decide which command the user wants
        fgets(buffer, sizeof(buffer), stdin);
        // access the command to execute
        sscanf(buffer, "%s", command);
        //executes the command
        response = executeCommand(command, buffer);

        if(response == 0)
            printf("\n---- successful---- \n\n");
        else printf("\n---- failure---- \n\n");
    }
}