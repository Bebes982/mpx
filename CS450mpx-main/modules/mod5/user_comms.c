#include <string.h>
#include "../mpx_supt.h"
#include "allocate_mem.h"
#include "free_mem.h"
#include"user_comms.h"
#include "showmem.h"


void user_alloc(char* str){
    uint32_t len = atoi(str);
    allocate_mem(len);
}


unsigned char hex_char_to_int(char c){
    if(c >= '0' && c<='9'){
        return c - '0';
    }
    if(c >= 'a' && c <= 'f'){
        return c - 'a' + 0xA;
    }
    if(c >= 'A' && c<= 'F'){
        return c - 'A' + 0xA;
    }
    return 0;
}

uint32_t hex_str_int(char* hex){
    int len = strlen(hex);
    uint32_t ret = 0;
    int i;
    for(i =0; i<len; i++){
        ret <<= 4;
        ret |= hex_char_to_int(hex[i]);
    }
    return ret;
}

void user_free(char* hex){
    
    uint32_t addr = hex_str_int(hex);
    
    

    if(free_mem( (void*) addr)){
        print_hex(addr);
        char* free_err = " is not allocated. Cannot free.\n";
        int x = strlen(free_err);
        sys_req(WRITE, DEFAULT_DEVICE, free_err, &x);
    }
} 