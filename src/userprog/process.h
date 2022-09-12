#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t tid);
void process_exit (void);
void process_activate (void);
// int process_wait_tid(tid_t tid);
#endif /* userprog/process.h */
