#include "mpx_supt.h"
#include <string.h>
#include "versionAndShutdown.h"
#include "data-structures/PCB.h"


int version(){
    char* message = "Module 5. Compiled on ";
    int messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    char* date = __DATE__;
    messageSize = (int)sizeof(date);
    sys_req(WRITE, DEFAULT_DEVICE, date, &messageSize);
    return 0;
}

int shutdown(){

    char* message = "Are you sure you want to shutdown?(y/n)\n";
    int messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    char input[2] = { 0 };
    int inputSize = (int)sizeof(input);
    sys_req(READ, DEFAULT_DEVICE, input, &inputSize);  

    // YES, shutting down  
    if(input[0] == 'y'){
        shutting_down = 1;
        sti();
        while(dequeue_next_pcb(ready_queue));
        sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
        return 1;
    }
    return 0;

}