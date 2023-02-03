#pragma once

void COMWRITE();


/*
  Procedure..: COMREAD
  Description..: This process writes a prompt to the 
  serial device, and then reads user input
  which is then printed back to the device
  Params..: None
*/
void COMREAD();
/*
  Procedure..: IOCOM25
  Description..: This process attempts to write a message to the 
  serial device 25 times and then exits
  Params..: None
*/
void IOCOM25();
/*
  Procedure..: IOCOM
  Description..: This process attempts to write a message to the 
  serial device until suspended and terminated
  Params..: None
*/
void IOCOM();