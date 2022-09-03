#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

typedef int pid_t;
// void syscall_init (void);

// /*
// Terminates Pintos by calling shutdown_power_off() (declared in threads/init.h). 
// This should be seldom used, because you lose some information about possible deadlock situations, etc.
// */
// void halt(void);

// /*
// Terminates the current user program, returning status to the kernel. 
// If the process's parent waits for it (see below), this is the status that will be returned. 
// Conventionally, a status of 0 indicates success and nonzero values indicate errors.
// */
// void exit(int status);

// /*
// Runs the executable whose name is given in cmd_line, passing any given arguments, 
// and returns the new process's program id (pid). 
// Must return pid -1, which otherwise should not be a valid pid, if the program cannot load or run for any reason. 
// Thus, the parent process cannot return from the exec until it knows whether the child process successfully loaded its executable. 
// You must use appropriate synchronization to ensure this.
// */
// pid_t exec(const char* cmd_line);

// int wait(pid_t pid);

// bool create(const char* file, unsigned initial_size);

// bool remove(const char* file);

// int open(const char* file);

// int filesize(int fd);

// int read(int fd, void* buffer, unsigned size);

// void seek(int fd, unsigned position);

// unsigned tell(int fd);

// void close(int fd);
#endif /* userprog/syscall.h */
