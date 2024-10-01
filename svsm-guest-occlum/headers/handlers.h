#ifndef HANDLERS_H
#define HANDLERS_H

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <asm/ptrace.h>

int     handle_core     (void __user *arg);
int     handle_ecreate  (void __user *arg);
int     handle_eadd     (void __user *arg);
int     handle_eenter   (void __user *arg);
int     handle_eextend  (void __user *arg);
int     handle_test     (void __user *arg);
int     handle_eresume  (void __user *arg);
int     handle_eremove  (void __user *arg);
int     handle_eldb     (void __user *arg);
int     handle_tramp    (void __user *arg);
int     handle_eaug     (void __user *arg);
int     handle_eblock   (void __user *arg);
int     handle_einit    (void __user *arg);
int     handle_ewb      (void __user *arg);
int     handle_perf     (void __user *arg);
int     handle_emodpr   (void __user * arg);

#define SUCCESS 0
#endif