#ifndef PGWALK_H
#define PGWALK_H


#include <linux/highmem.h>

#include <linux/sched.h>
#include <linux/mm_types.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/io.h>

#include <linux/mm.h>

void print_page_table_entry(unsigned long address, int flag);
void set_pte_x(unsigned long address);
void set_pte_pkru(unsigned long address, int key);
void app_pgwalk(unsigned long virt_addr, unsigned long phys_addr, int not_execute_flag);

#endif