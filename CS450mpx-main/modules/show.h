#include "data-structures/PCB.h"

/*
    Displays to screen name, priority, class, state, dispatching state
    • Parameters:
        name of process to display
    • Returns:
        void
*/
void showPCB(PCB* pcb);

/*
    Displays to screen name, priority, class, state, dispatching state
    of all ready processes
    • Parameters:
        none
    • Returns:
        void
*/
void showReadyProcesses();

/*
    Displays to screen name, priority, class, state, dispatching state
    of all blocked processes
    • Parameters:
        none
    • Returns:
        void
*/
void showBlockedProcesses();

/*
    Displays to screen name, priority, class, state, dispatching state
    of all processes
    • Parameters:
        none
    • Returns:
        void
*/
void showAllProcesses();
