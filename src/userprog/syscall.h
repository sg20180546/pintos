#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include <stdint.h>
#include <stdbool.h>
// typedef int pid_t;
void syscall_init (void);
bool is_open_file_executing(const char* file);
#endif /* userprog/syscall.h */
