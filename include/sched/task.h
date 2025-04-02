#pragma once

#include <common/dl_list.h>
#include <common/lock.h>

#include <mm/mmu.h>
#include <lib/stdint.h>
#include <lib/stddef.h>

#include <arch/sched/arch_context.h>

#define TASK_NAME_MAX_LEN 32
typedef uint32_t task_state_t;
typedef uint32_t hartid_t;
typedef uint32_t pid_t;
typedef uint32_t tid_t;
typedef uint32_t priority_t;

enum task_state {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_WAITING,
    TASK_TERMINATED,
};

struct cpu;

struct task {
    char           task_name[TASK_NAME_MAX_LEN];
    tid_t          tid;
    pid_t          pid;
    struct cpu     *cpu;
    task_state_t   state;
    priority_t     priority;
    void           *sp;
    uint32_t       stack_size;
    uint32_t       time_slice;
    uint32_t       wait_time;
    context        ctx;
    struct dl_list ready_list;
    struct dl_list pend_list;
    struct task    *parent;
    struct dl_list children;
};

struct cpu {
    struct task *current_task;
    uintptr_t *stack_ptr;
    struct spinlock lock;
};