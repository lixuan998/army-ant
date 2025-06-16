#pragma once

#include <lib/printk.h>

#define LOG_LEVEL_ERR   0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_DEBUG 2

#ifndef CURRENT_LOG_LEVEL
#define CURRENT_LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#define LOG(level, level_str, tag, fmt, ...)                   \
    do {                                                       \
        if (level <= CURRENT_LOG_LEVEL) {                      \
            printk(level_str "[%s] " fmt "\r\n", tag, ##__VA_ARGS__);   \
        }                                                      \
    } while (0)

#define KLOG_ERR(tag, fmt, ...)     \
    LOG(LOG_LEVEL_ERR, "[ERR]", tag, fmt, ##__VA_ARGS__)
#define KLOG_INFO(tag, fmt, ...)    \
    LOG(LOG_LEVEL_INFO, "[INFO]", tag, fmt, ##__VA_ARGS__)
#define KLOG_DEBUG(tag, fmt, ...)   \
    LOG(LOG_LEVEL_DEBUG, "[DEBUG]", tag, fmt, ##__VA_ARGS__)

extern const char boot_tag[];
extern const char mm_tag[];
extern const char mmu_tag[];