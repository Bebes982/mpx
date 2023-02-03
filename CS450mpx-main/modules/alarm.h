/*
  Procedure..: alarm
  Description..: Prints a message to the screen when the time indicated is passed
*/
void alarm();

/*
  Procedure..: alarm
  Description..: Sets up alarm context and pcb
  Params..: char* time - time for alarm to be set formatted hh:mm:ss
            char* mess - message to display
*/
void setupAlarm(char* time, char* mess);