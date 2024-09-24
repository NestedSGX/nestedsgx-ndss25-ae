/**
 * UTIL_H:
 * to export alloc function from the linux kernel.
*/

#ifndef UTIL_H
#define UTIL_H

#include <linux/kallsyms.h>
#include <linux/kprobes.h>
#include <linux/smp.h>
#include "log.h"

extern unsigned long (*kallsyms_lookup_name_fun)(const char *name);

// Extrenal symbol will be analysed by this macro.
#define RESOLVE_EXTERNAL_SYMBOL(symbol)                               \
	do {                                                          \
		symbol##_sym = (void *)kallsyms_lookup_name_fun(#symbol); \
		if (!symbol##_sym) {                                  \
			vs_err("Can't get symbol %s\n", #symbol);     \
			return -EINVAL;                               \
		}                                                     \
	} while (0)

int find_kallsyms_lookup_name(void);

#endif