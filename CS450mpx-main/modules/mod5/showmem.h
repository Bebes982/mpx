#ifndef _SHOW_MCB
#define _SHOW_MCB

#include <system.h>
#include "mcb.h"

void print_hex(uint32_t n);

void show_free_mem();

void show_MCB(MCB* _mcb);

void show_allocated_mem();

void show_all_mem();

void show_MCB(MCB* _mcb);
void print_int2(int a);

#endif