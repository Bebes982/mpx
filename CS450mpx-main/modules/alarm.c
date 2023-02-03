#include "data-structures/PCB.h"
#include "mpx_supt.h"
#include <string.h>
#include <core/io.h>
#include "getSetDateTime.h"
#include <system.h>

#include "loadr3.h"

// hh:mm:ss
void alarm(int sec, int minute, int hour, char* mess){ //char* mess, int hour, int minute, int sec){
    
    // check if past time
    while(1){

                // get current time
        outb(0x70, 0x04); // get hour
        unsigned char hbcd = inb(0x71);
        outb(0x70, 0x02); // get min
        unsigned char mbcd = inb(0x71);
        outb(0x70, 0x00); // get sec
        unsigned char sbcd = inb(0x71);

        int currh = bcd_to_int(hbcd);
        int currm = bcd_to_int(mbcd);
        int currs = bcd_to_int(sbcd);

        char time_passed = FALSE;

        if(currh > hour){
            time_passed = TRUE;
        }else if(currh >= hour && currm > minute){
            time_passed = TRUE;
        }else if(currh >= hour && currm >= minute && currs > sec){
            time_passed = TRUE;
        }

        if(!time_passed){
            sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
        }
        else{
            int messageSize = strlen(mess);
            char* nl = "\n";
            int nls= strlen(nl);
            sys_req(WRITE, DEFAULT_DEVICE, mess, &messageSize);
            sys_req(WRITE, DEFAULT_DEVICE, nl, &nls);
            sys_free_mem(mess);
            sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
        }
    }
}

void setupAlarm(char* time, char* mess){
    // check size
    if(strlen(time) != 8){
        char* message = "Input wrong size. Try 'help'.\n";
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return;
    }

    if(!mess){
        char* message = "Input does not have alarm message.\n";
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return;
    }

    int copy_size = strlen(mess)+8;

    char* copy_mess = sys_alloc_mem(copy_size);
    memset(copy_mess,'\0', copy_size);
    strcpy(copy_mess, mess);

    // get time
    int hour = (time[0]-'0')*10 + time[1]-'0';
    int minute = (time[3]-'0')*10 + time[4]-'0';
    int sec = (time[6]-'0')*10 + time[7]-'0';

    // check vals
    if(hour < 0 || minute < 0 || sec < 0 || hour > 24 || minute > 59 || sec > 59){
        char* message = "invalid input\n";
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return;
    }

    // setup pcb
    PCB* _pcb = setup_pcb(time, PCB_USER_PROCESS, 5);
        
    initializeRegs(_pcb, alarm);
    unsigned char* args = &_pcb->mem2[4];
    *((int*)args) = sec;
    *(int*)(args + sizeof(int)) = minute;
    *(int*)(args + sizeof(int)*2)  = hour;
    *(char**)(args + sizeof(int)*3) = copy_mess;
    
    insert_pcb(_pcb);
}