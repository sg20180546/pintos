#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
typedef void syscall_handler_func (uint32_t);

static void syscall_handler (struct intr_frame *);

static void syscall_halt(uint32_t* esp);

static void syscall_exit(uint32_t* esp);

static void syscall_exec(uint32_t* esp);

static void syscall_wait(uint32_t* esp);

static void syscall_create(uint32_t* esp);

static void syscall_remove(uint32_t* esp);

static void syscall_open(uint32_t* esp);

static void syscall_filesize(uint32_t* esp);

static void syscall_read(uint32_t* esp);

static void syscall_write(uint32_t* esp);

static void syscall_seek(uint32_t* esp);
 
static void syscall_tell(uint32_t* esp);

static void syscall_close(uint32_t* esp);

static void (*syscall_handlers[])(uint32_t*) = 
                                    {syscall_halt,syscall_exit,syscall_exec,
                                    syscall_wait,syscall_create,syscall_remove,
                                    syscall_open,syscall_filesize,syscall_read,
                                    syscall_write,syscall_seek,syscall_tell,
                                    syscall_close};

void
syscall_init (void) 
{
  // printf("system call init\n\n");
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  // printf ("system call!\n\n");
  // thread_exit ();
  void* esp=f->esp; // syscall number
  uint32_t syscall_nr=*((uint32_t*)esp);
  syscall_handlers[syscall_nr](esp);
}

static void syscall_halt(uint32_t* esp)
{
  int a=*esp;
  a++;
}

static void syscall_exit(uint32_t* esp)
{
  thread_current()->exit_status=*(++esp);
  thread_exit();
}

static void syscall_exec(uint32_t* esp)
{
  int a=*esp;
  a++;
}

static void syscall_wait(uint32_t* esp)
{
  int a=*esp;
  a++;
}

static void syscall_create(uint32_t* esp)
{
  int a=*esp;
  a++;
}

static void syscall_remove(uint32_t* esp)
{
  int a=*esp;
  a++;
}

static void syscall_open(uint32_t* esp)
{
  int a=*esp;
  a++;
}

static void syscall_filesize(uint32_t* esp)
{
  int a=*esp;
  a++;
}

static void syscall_read(uint32_t* esp)
{
  int a=*esp;
  a++;
}

static void syscall_write(uint32_t* esp)
{
  int fd=*(++esp);
  char* buffer=*(++esp);
  int size=*(++esp);
  int check;
  putbuf(buffer,size);

  asm volatile
  (
    "movl %1, %%eax\n\t"
    "movl %%eax, %0\n\t"
    :"=m"(check)
    :"m"(size)
    :"eax"
  );
  ASSERT(check==size);
}

static void syscall_seek(uint32_t* esp)
{
  int a=*esp;
  a++;
}
 
static void syscall_tell(uint32_t* esp)
{
  int a=*esp;
  a++;
}

static void syscall_close(uint32_t* esp)
{
  int a=*esp;
  a++;
}