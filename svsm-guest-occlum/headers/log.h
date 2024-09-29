#ifndef LOG_H
#define LOG_H

#include <linux/printk.h>

#undef pr_fmt
#define pr_fmt(fmt) "VS_GUEST: %s: %d. " fmt, __func__, __LINE__

#define vs_info(fmt...)  pr_info(fmt)
#define vs_warn(fmt...)  pr_warn("VS_WARN. " fmt)
#define vs_err(fmt...)  pr_err("VS_INFO. " fmt)

#ifdef DEBUG
#define VS_DEBUG(fmt, args...) vs_info(fmt, ##args)
#else
#define VS_DEBUG(fmt, args...) do { } while (0)
#endif

#endif