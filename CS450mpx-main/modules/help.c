#include "help.h"
#include "mpx_supt.h"
#include <string.h>

// All of the strings to print for `help`
char* help_strings[] = {help_general,
                        help_help,
                        help_shutdown,
                        help_version,
                        help_settime,
                        help_gettime,
                        help_setdate,
                        help_getdate,
                        help_suspend,
                        help_resume,
                        help_setpriority,
                        help_showPCB,
                        help_showallprocesses,
                        help_showreadyprocesses,
                        help_showblockedprocesses,
                        help_deletePCB,
                        help_loadr3,
                        help_alarm,
                        help_infiniteprocess,
                        help_showallocmem,
                        help_showfreemem,
                        help_allocmem,
                        help_freemem,
                        help_loadr6};

// prints all of the help statements in help_strings
void print_all_help(){
    int str_count = sizeof(help_strings)/sizeof(help_strings[0]);
    int i;
    for(i = 0; i<str_count; i++){
        int message_len = strlen(help_strings[i]);
        sys_req(WRITE, DEFAULT_DEVICE, help_strings[i], &message_len);
    }
}


void help(char* arguments){
    char* command_string = NULL; //command to print help for

    if(!arguments){ //no command. print all commands
        print_all_help();
        return;
    }
    else if(!strcmp(arguments, "general"))
        command_string = help_general;
    else if(!strcmp(arguments, "help"))
        command_string = help_help;
    else if(!strcmp(arguments, "shutdown"))
        command_string = help_shutdown;
    else if(!strcmp(arguments, "version"))
        command_string = help_version;
    else if(!strcmp(arguments, "settime"))
        command_string = help_settime;
    else if(!strcmp(arguments, "gettime"))
        command_string = help_gettime;
    else if(!strcmp(arguments, "setdate"))
        command_string = help_setdate;
    else if(!strcmp(arguments, "getdate"))
        command_string = help_getdate;
    else if(!strcmp(arguments, "suspend"))
        command_string = help_suspend;
    else if(!strcmp(arguments, "resume"))
        command_string = help_resume;
    else if(!strcmp(arguments, "setpriority"))
        command_string = help_setpriority;
    else if(!strcmp(arguments, "showPCB"))
        command_string = help_showPCB;
    else if(!strcmp(arguments, "showallprocesses"))
        command_string = help_showallprocesses;
    else if(!strcmp(arguments, "showreadyprocesses"))
        command_string = help_showreadyprocesses;
    else if(!strcmp(arguments, "showblockedprocesses"))
        command_string = help_showblockedprocesses;   
    else if(!strcmp(arguments, "deletePCB"))
        command_string = help_deletePCB;    
    else if(!strcmp(arguments, "loadr3"))
        command_string = help_loadr3;    
    else if(!strcmp(arguments, "alarm"))
        command_string = help_alarm;
    else if(!strcmp(arguments, "infinite"))
        command_string = help_infiniteprocess;
    else if(!strcmp(arguments, "showallocmem"))
        command_string = help_showallocmem;
    else if(!strcmp(arguments, "showfreemem"))
        command_string = help_showfreemem;
    else if(!strcmp(arguments, "allocmem"))
        command_string = help_allocmem;
    else if(!strcmp(arguments, "freemem"))
        command_string = help_freemem;
    else if(!strcmp(arguments, "loadr6"))
        command_string = help_loadr6;
    
    if(command_string){
        int command_string_len = strlen(command_string);
        sys_req(WRITE, DEFAULT_DEVICE, command_string, &command_string_len);
    }
    else{
        print_all_help();
    }
}
