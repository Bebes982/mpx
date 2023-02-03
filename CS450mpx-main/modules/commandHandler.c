#include "commandHandler.h"
#include "mpx_supt.h"
#include <string.h>

#include "versionAndShutdown.h"
#include "help.h"
#include "getSetDateTime.h"
#include "data-structures/PCB.h"
#include "show.h"
#include "PCBCommands.h"
#include "loadr3.h"

#include "alarm.h"
#include "infinite.h"
#include "mod5/user_comms.h"
#include "mod5/showmem.h"
#include "mod6/newTestProcs.h"

int comHand(){
    help("general"); //print welcome statement.

    int quit = 0;
    while(!quit){
        char buffer[100] = { ' ' };
        int bufferSize = (int)sizeof(buffer);
        sys_req(READ, DEFAULT_DEVICE, buffer, &bufferSize); //prompt user to enter command
        char* methodName = buffer;

        #define MAX_ARGS 32
        char* arguments[MAX_ARGS];
        memset(arguments,0, sizeof(char*)*MAX_ARGS);
        int arg_count = 0;

        int i;
        for(i = 0; i<(int)sizeof(buffer); i++){ //find first space
            if(buffer[i] == ' '){
                buffer[i] = '\0';
                arguments[arg_count] = buffer + i + 1; //everything after the space is the arguments.
                arg_count++;
            }
            else if(buffer[i] == '\0'){
                break;
            }
        }

        char* nl = "\n"; //used to print newline after some commands
        int nl_len = strlen(nl);

        if(strcmp(methodName, "shutdown") == 0){
            quit = shutdown();
        }
        else if(strcmp(methodName, "version") == 0){
            version();
            sys_req(WRITE,DEFAULT_DEVICE, nl, &nl_len);
        }
        else if(strcmp(methodName, "help") == 0){
            help(arguments[0]);
        }
        else if(strcmp(methodName, "setdate") == 0){
            setDate(arguments[0]);
        }
        else if(strcmp(methodName, "getdate") == 0){
            getDate(arguments[0]);
            sys_req(WRITE,DEFAULT_DEVICE, nl, &nl_len);
        }
        else if(strcmp(methodName, "settime") == 0){
            setTime(arguments[0]);
        }
        else if(strcmp(methodName, "gettime") == 0){
            getTime(arguments[0]);
            sys_req(WRITE,DEFAULT_DEVICE, nl, &nl_len);
        }
        else if(strcmp(methodName, "deletePCB") == 0){
            deletePCB(arguments[0]);
        }
        else if(strcmp(methodName, "showPCB") == 0){
            PCB* _pcb = find_pcb(arguments[0]);
            showPCB(_pcb);
        }
        else if(strcmp(methodName, "showallprocesses") == 0){
            showAllProcesses();
        }
        else if(strcmp(methodName, "showreadyprocesses") == 0){
            showReadyProcesses();
        }
        else if(strcmp(methodName, "showblockedprocesses") == 0){
            showBlockedProcesses();
        }
        else if(strcmp(methodName, "suspend") == 0){
            suspendPCB(arguments[0]);
        }
        else if(strcmp(methodName, "resume") == 0){
            resumePCB(arguments[0]);
        }
        else if(strcmp(methodName, "infinite") == 0){
            setup_infinite();
        }
        else if(strcmp(methodName, "alarm") == 0){
            setupAlarm(arguments[0], arguments[1]);
        }
        else if(strcmp(methodName, "setpriority") == 0){
            char* name = arguments[0];
            char* priority = arguments[1];
            int priorityInt = atoi(priority);
            setPriority(name, priorityInt);
        }
        else if(strcmp(methodName, "loadr3") == 0){
            loadr3();
        }
        else if(strcmp(methodName, "showallocmem") == 0){
            show_allocated_mem();
        }
        else if(strcmp(methodName, "showfreemem") == 0){
            show_free_mem();
        }
        else if(strcmp(methodName, "allocmem") == 0){            
            user_alloc(arguments[0]);
        }
        else if(strcmp(methodName, "freemem") == 0){
            user_free(arguments[0]);
        }
        else if (strcmp(methodName, "loadr6") == 0){
            COMWRITE();
            COMREAD();
            IOCOM25();
            IOCOM();
        }
        else{
            char* unknown_command = "Invalid command. Type `help` for help.\n";
            int unk_com_len = strlen(unknown_command);
            sys_req(WRITE, DEFAULT_DEVICE, unknown_command, &unk_com_len);
        }     
    }
    sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    return 0;
}