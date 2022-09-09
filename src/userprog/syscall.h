#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include <stdint.h>
// typedef int pid_t;
void syscall_init (void);
void syscall_exit(uint32_t* esp);
#endif /* userprog/syscall.h */
