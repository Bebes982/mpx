#include "initializeHeap.h"
#include "../mpx_supt.h"
#include <string.h>

const char* hex_val = "0123456789ABCDEF";

void print_hex(uint32_t n){
    char str[11];
    str[10] = '\0';
    str[0] = '0';
    str[1] = 'x';
    int i;
    for(i = 2; i<10; i++){
        unsigned char half_byte = (n>>((9-i)*4)) & 0xF;
        str[i] = hex_val[half_byte];
    }
    int len = 9;
    sys_req(WRITE, DEFAULT_DEVICE, str, &len);
}

char digit_to_char2(int a){
  if( a<0 || a > 9)
    return 0;

  return a + '0';
}

//prints integer to console
void print_int2(int a){
  int numberplace = 1000000000;
  char any_printed = 0;

  char s[] = {0,0};
  
  if(a<0){
    s[0] = '-';
    int l = 2;
    sys_req(WRITE, DEFAULT_DEVICE, s, &l);
  }

  do{
    int digit = (a / numberplace)%10;
    s[0] = digit_to_char2(digit);
    int ll = 2;

    switch(digit){
      case 0:
        if(!any_printed)
          break;
      default:
        sys_req(WRITE, DEFAULT_DEVICE, s, &ll);
        any_printed = 1;
    }

    numberplace /= 10;

  }while(numberplace > 0);
  if(!any_printed){
    int ll = 2;
     sys_req(WRITE, DEFAULT_DEVICE, s, &ll);
}

}

/*
typedef struct MCB{
    void *start_address;  
    size_t size;
    struct MCB* next;
    struct MCB* prev;
} MCB;
*/

/*
MCB* HeadFreeList;
MCB* TailFreeList;
MCB* HeadAllocatedList;
MCB* TailAllocatedList;
*/

void show_MCB(MCB* _mcb){
    if(!_mcb){
        char* e = "NULL MCB\n";
        int e_len = strlen(e);
        sys_req(WRITE, DEFAULT_DEVICE, e, &e_len);
        return;
    }

    char* nl = "\n";


    char* mm = "MCB address: \t";
    int mm_len = strlen(mm);
    sys_req(WRITE, DEFAULT_DEVICE, mm, &mm_len);
    print_hex((uint32_t) _mcb);
    int nlen = strlen(nl);
    sys_req(WRITE, DEFAULT_DEVICE, nl, &nlen);

    char* e1 = "Start address: \t";
    int e1_len = strlen(e1);
    sys_req(WRITE, DEFAULT_DEVICE, e1, &e1_len);
    print_hex((uint32_t) _mcb->start_address);
    sys_req(WRITE, DEFAULT_DEVICE, nl, &nlen);

    char* e12 = "End address: \t";
    int e12_len = strlen(e12);
    sys_req(WRITE, DEFAULT_DEVICE, e12, &e12_len);
    print_hex((uint32_t) _mcb->start_address + _mcb->size -1);
    nlen = strlen(nl);
    sys_req(WRITE, DEFAULT_DEVICE, nl, &nlen);



    char* e2 = "Size: \t\t";
    int e2_len = strlen(e2);
    sys_req(WRITE, DEFAULT_DEVICE, e2, &e2_len);
    print_int2(_mcb->size);
    nlen = strlen(nl);
    sys_req(WRITE, DEFAULT_DEVICE, nl, &nlen);

    char* all = "Status: \tallocated";
    char* fre = "Status: \tfree";
    char* mem_stat = _mcb->allocated ? all : fre;
    int e3_len = strlen(mem_stat);
    sys_req(WRITE, DEFAULT_DEVICE, mem_stat, &e3_len);
    nlen = strlen(nl);
    sys_req(WRITE, DEFAULT_DEVICE, nl, &nlen);

    char* e4 = "Previous MCB: \t";
    int e4_len = strlen(e4);
    sys_req(WRITE, DEFAULT_DEVICE, e4, &e4_len);
    print_hex((uint32_t) _mcb->prev);
    nlen = strlen(nl);
    sys_req(WRITE, DEFAULT_DEVICE, nl, &nlen);

    char* e5 = "Next MCB: \t";
    int e5_len = strlen(e5);
    sys_req(WRITE, DEFAULT_DEVICE, e5, &e5_len);
    print_hex((uint32_t) _mcb->next);
    nlen = strlen(nl);
    sys_req(WRITE, DEFAULT_DEVICE, nl, &nlen);

}

void show_free_mem(){
    MCB* cur_MCB;

    for(cur_MCB = HeadList; cur_MCB; cur_MCB = cur_MCB->next){
        if(!cur_MCB->allocated){
            show_MCB(cur_MCB);
            char* nl = "\n";
            int nlen = strlen(nl);
            sys_req(WRITE, DEFAULT_DEVICE, nl, &nlen);
        }
    }
}

void show_allocated_mem(){
    MCB* cur_MCB;

    for(cur_MCB = HeadList; cur_MCB; cur_MCB = cur_MCB->next){
        if(cur_MCB->allocated){
            show_MCB(cur_MCB);
            char* nl = "\n";
            int nlen = strlen(nl);
            sys_req(WRITE, DEFAULT_DEVICE, nl, &nlen);
        }
    }
}

void show_all_mem(){
    MCB* cur_MCB;

    for(cur_MCB = HeadList; cur_MCB; cur_MCB = cur_MCB->next){
        show_MCB(cur_MCB);
        char* nl = "\n";
        int nlen = strlen(nl);
        sys_req(WRITE, DEFAULT_DEVICE, nl, &nlen);
    
    }

}