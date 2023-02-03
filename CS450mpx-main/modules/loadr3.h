#include "data-structures/PCB.h"
/*
  Procedure..: loadr3
  Description..: creates a PCB for each function in procsr3 file
  Params..: null
*/
void loadr3();
/*
  Procedure..: initializeRegs
  Description..: Initializes the register values for the process
  Params..: a pointer to the PCB and the name of the function with a void return type the EIP needs to point to
*/
void initializeRegs(PCB* p, void proc_fn());

/*
  Procedure..: initializeRegs
  Description..: Initializes the register values for the process
  Params..: a pointer to the PCB and the name of the function with a int return type the EIP needs to point to
*/
void initializeRegs2(PCB* p, int proc_fn());