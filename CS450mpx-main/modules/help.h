
#define help_general                "Type `help` for list of available commands. `$` character indicates awaiting user input.\n"
#define help_help                   "help [command]\n\t Print this message or help for a particular command.\n"
#define help_shutdown               "shutdown \n\t Stop operation of system. Prompts the user to confirm with 'y' or 'n'.\n"
#define help_version                "version \n\t Print current version of project.\n"
#define help_settime                "settime <hh mm ss> \n\t Set UTC time. Does not support switching timezones. Does not check for invalid input.\n"
#define help_gettime                "gettime \n\t Print the current time in UTC.\n"
#define help_setdate                "setdate <yyyy-mm-dd> \n\t Set the current date. Does not check for invalid input.\n"
#define help_getdate                "getdate \n\t Print the current date\n"
#define help_suspend                "suspend <proc name>\n\t If a process with the name exists suspends it.\n"
#define help_resume                 "resume <proc name>\n\t If a process with the name exists resume its execution.\n"
#define help_setpriority            "setpriority <proc name> <priority level>\n\t If a process with the name exists and the priority level is between 0 and 9 inclusive, update its priority level.\n"
#define help_showPCB                "showPCB <proc name>\n\t If a process with the name exists prints its information to the console.\n"
#define help_showallprocesses       "showallprocesses \n\t Prints out information about all the processes to the console.\n"
#define help_showreadyprocesses     "showreadyprocesses \n\t Prints out information about all ready processes to the console.\n"
#define help_showblockedprocesses   "showblockedprocesses \n\t prints out information about all blocked processes to the console.\n"
#define help_deletePCB              "deletePCB <proc name>\n\t If a process with the name exists delete it. Only works on suspended processes.\n"
#define help_loadr3                 "loadr3 \n\t creates a PCB for each function in the procsr3.c file.\n"
#define help_alarm                  "alarm <hh:mm:ss> <message>\n\t If the current time is exactly or after the specified time, prints out the message to the console. Message may not include spaces.\n"
#define help_infiniteprocess        "infinite\n\t Creates a process that infinitely prints out a message, but yields CPU control after each print.\n"
#define help_showallocmem           "showallocmem\n\t Prints out all the MCBs which are allocated.\n"
#define help_showfreemem            "showfreemem\n\t Prints out all the MCBs which are free.\n"
#define help_allocmem               "allocmem <size of memory>\n\t Dynamcally allocates memory with the input size.\n"
#define help_freemem                "freemem <address in hex>\n\t Frees the memory at the given location.\n"
#define help_loadr6                 "loadr6 \n\t Runs each test method that is provided in the newTestProcs.c file.\n"

/*
  Procedure..: help
  Description..: prints the help string associated with one or all commands.
  Params..: char* containing optional arguement
*/
void help(char* arguments);
