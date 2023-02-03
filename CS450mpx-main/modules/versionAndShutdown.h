#ifndef VERSION_SHUTDOWN
#define VERSION_SHUTDOWN

char shutting_down;

/*
  Procedure..: version
  Description..: prints out the module number
  Params..: none
*/
int version();

/*
  Procedure..: sys_req
  Description..: Asks for a confirmation. If yes, exit the command handler.
  Params..: none
*/
int shutdown();


#endif