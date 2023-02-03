#include "procsr3.h"
#include "data-structures/PCB.h"

void initializeRegs(PCB* p, void proc_fn()){
        context * c = (context *) p->stack_top;
        c->cs = 0x08;
        c->ebp = (uint32_t)p->stack_base;
        c->esp = (uint32_t)p->stack_top;
        c->eflags = 0x0202;

        c->eip = (uint32_t) proc_fn;

        //everything else
        c->ds = 0x10;
        c->es = 0x10;
        c->fs = 0x10;
        c->gs = 0x10;
}
void initializeRegs2(PCB* p, int proc_fn()){
        context * c = (context *) p->stack_top;
        c->cs = 0x08;
        c->ebp = (uint32_t)p->stack_base;
        c->esp = (uint32_t)p->stack_top;
        c->eflags = 0x0202;

        c->eip = (uint32_t) proc_fn;

        //everything else
        c->ds = 0x10;
        c->es = 0x10;
        c->fs = 0x10;
        c->gs = 0x10;
}

void loadr3(){
    PCB* pcb1 = setup_pcb("proc1", PCB_USER_PROCESS, 5);
    initializeRegs(pcb1, proc1);
    insert_pcb(pcb1);

    PCB* pcb2 = setup_pcb("proc2", PCB_USER_PROCESS, 5);
    initializeRegs(pcb2, proc2);
    insert_pcb(pcb2);

    PCB* pcb3 = setup_pcb("proc3", PCB_USER_PROCESS, 5);
    initializeRegs(pcb3, proc3);
    insert_pcb(pcb3);

    PCB* pcb4 = setup_pcb("proc4", PCB_USER_PROCESS, 5);
    initializeRegs(pcb4, proc4);
    insert_pcb(pcb4);

    PCB* pcb5 = setup_pcb("proc5", PCB_USER_PROCESS, 5);
    initializeRegs(pcb5, proc5);
    insert_pcb(pcb5);
}
