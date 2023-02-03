#ifndef _TABLES_H
#define _TABLES_H

#include "system.h"

typedef struct idt_entry_struct
{
  uint16_t base_low;  //offset bits 0..15
  uint16_t sselect;   //stack selector in gdt or ldt
  uint8_t  zero;      //this stays zero; unused
  uint8_t  flags;     //attributes
  uint16_t base_high; //offset bits 16..31
}
  __attribute__ ((packed)) idt_entry;

typedef struct idt_struct
{
  uint16_t limit;
  uint32_t base;
}
  __attribute__ ((packed)) idt_descriptor;

typedef struct gdt_descriptor_struct
{
  uint16_t limit;
  uint32_t base;
}
  __attribute__ ((packed)) gdt_descriptor;

typedef struct gdt_entry_struct
{
  uint16_t limit_low; //first 16 bits of limit
  uint16_t base_low;  //first 16 bits of base
  uint8_t  base_mid;  //bits 16-23 of base
  uint8_t  access;    //next 8 bits; access flags
  uint8_t  flags;     //page granularity, size
  uint8_t  base_high; //last 8 bits of the base
}
  __attribute__ ((packed)) gdt_entry;


void idt_set_gate(uint8_t idx, uint32_t base, uint16_t sel, uint8_t flags);
void gdt_init_entry(int idx, uint32_t base, uint32_t limit, uint8_t access, 
		    uint8_t flags);

void init_idt();
void init_gdt();

#endif
