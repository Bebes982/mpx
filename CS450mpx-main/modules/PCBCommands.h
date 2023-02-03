/*
  Procedure..: suspendPCB
  Description..: if a process with the name exists suspend it
  Params..: name of the PCB
*/
void suspendPCB(char* name);

/*
  Procedure..: resumePCB
  Description..: if a process with the name exists resume its executions
  Params..: name of the PCB
*/
void resumePCB(char* name);

/*
  Procedure..: suspendPCB
  Description..: if a process with the name exists suspend it
  Params..: name of the PCB and the new priority
*/
void setPriority(char* name, int priority);

/*
  Procedure..: deletePCB
  Description..: Finds the process specified, removes it from its queue, and frees the associated memory
  Params..: char* name - name of process
*/
void deletePCB(char* name);