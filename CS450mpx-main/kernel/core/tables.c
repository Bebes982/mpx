/*
  ----- tables.c -----

  Description..: Routines for setting up the global descriptor
      table and interrupt descriptor table.
*/

#include <string.h>
#include <core/tables.h>

// Global Descriptor Table
gdt_descriptor gdt_ptr;
gdt_entry gdt_entries[5];

// Interrupt Descriptor Table
idt_descriptor idt_ptr;
idt_entry idt_entries[256];

// Assembly routines to install tables
extern void write_gdt_ptr(uint32_t, size_t);
extern void write_idt_ptr(uint32_t);

/*
  Procedure..: idt_set_gate
  Description..: Installs a new gate entry into the IDT.
*/
void idt_set_gate(uint8_t idx, uint32_t base, uint16_t sel,
		  uint8_t flags)
{
  idt_entry *new_entry = &idt_entries[idx];
  new_entry->base_low  = (base &  0xFFFF);
  new_entry->base_high = (base >> 16) & 0xFFFF;
  new_entry->sselect   = sel;
  new_entry->zero = 0;
  new_entry->flags = flags;
}

/*
  Procedure..: init_idt
  Description..: Creates the interrupt descriptor table and 
      writes the pointer using the defined assembly function.
*/
void init_idt()
{
  idt_ptr.limit = 256*sizeof(idt_descriptor) - 1;
  idt_ptr.base  = (uint32_t)idt_entries;
  memset(idt_entries, 0, 256*sizeof(idt_descriptor));
  
  write_idt_ptr((uint32_t)&idt_ptr);
}

/*
  Procedure..: gdt_init_entry
  Description..: Installs a new table entry into the global
      descriptor table.
*/
void gdt_init_entry(int idx, uint32_t base, uint32_t limit, 
		    uint8_t access, uint8_t flags)
{
  gdt_entry *new_entry = &gdt_entries[idx];
  new_entry->base_low  = (base & 0xFFFF);
  new_entry->base_mid  = (base >> 16) & 0xFF;
  new_entry->base_high = (base >> 24) & 0xFF;
  new_entry->limit_low = (limit & 0xFFFF);
  new_entry->flags  = (limit >> 16) & 0xFF;
  new_entry->flags |= flags & 0xF0;
  new_entry->access = access;
}

/*
  Procedure..: init_gdt
  Description..: Creates the global descriptor table and installs
      it using the defined assembly routine.
*/
void init_gdt()
{
  gdt_ptr.limit = 5 * sizeof(gdt_entry) - 1;
  gdt_ptr.base  = (uint32_t) gdt_entries;

  uint32_t limit = 0xFFFFFFFF;
  gdt_init_entry(0, 0, 0, 0, 0);           //required null segment
  gdt_init_entry(1, 0, limit, 0x9A, 0xCF); //code segment
  gdt_init_entry(2, 0, limit, 0x92, 0xCF); //data segment
  gdt_init_entry(3, 0, limit, 0xFA, 0xCF); //user mode code segment
  gdt_init_entry(4, 0, limit, 0xF2, 0xCF); //user mode data segment

  write_gdt_ptr((uint32_t) &gdt_ptr, sizeof(gdt_ptr));
}
