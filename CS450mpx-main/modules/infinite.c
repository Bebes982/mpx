#include "infinite.h"

void infinite () {
    while (1){
        char* message = "Infinite process is running\n";
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }

}

void setup_infinite () {
    PCB* pcb = setup_pcb("infinite", PCB_USER_PROCESS, 5);
    insert_pcb(pcb);

    context * c = (context *) pcb->stack_top;
        c->cs = 0x08;
        c->ebp = (uint32_t)pcb->stack_base;
        c->esp = (uint32_t)pcb->stack_top;
        c->eflags = 0x0202;

        c->eip = (uint32_t) infinite;

        //everything else
        c->ds = 0x10;
        c->es = 0x10;
        c->fs = 0x10;
        c->gs = 0x10;

        
}
