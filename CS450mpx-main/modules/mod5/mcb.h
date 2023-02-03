#ifndef _MCB_H
#define _MCB_H
#include <system.h>

/*
    Start Address - The base address of the usable memory in the block
    Size - The size of the block in bytes
    Pointers to the next and previous block in the list
*/

typedef struct MCB{
    size_t size;
    char allocated;
    struct MCB* next;
    struct MCB* prev;
    void *start_address;
} MCB;



#endif