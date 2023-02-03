#include "data-structures/PCB.h"
#include "mpx_supt.h"

/*
Procedure..: infinite
Description..: Gets called by setup_infinite(). Prints a message to the console while it is running.
*/
void infinite();

/*
Procedure..: setup_infinite
Description..: Creates a new process which should only be able to be deleted after the user has suspended it. 
*/
void setup_infinite();
