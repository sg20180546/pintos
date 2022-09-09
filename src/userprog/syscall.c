#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

#define MAX_SYSCALL_NR 13
typedef void syscall_handler_func (uint32_t*);

struct syscall_handler_t {
  syscall_handler_func* func;
  char name[128];
  char argc;
};


static void syscall_handler (struct intr_frame *);

static void syscall_halt(uint32_t* esp);

void syscall_exit(uint32_t* esp);

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

// static void (*syscall_handlers[])(uint32_t*) = 
//                                     {syscall_halt,syscall_exit,syscall_exec,
//                                     syscall_wait,syscall_create,syscall_remove,
//                                     syscall_open,syscall_filesize,syscall_read,
//                                     syscall_write,syscall_seek,syscall_tell,
//                                     syscall_close};

struct syscall_handler_t syscall_handlers[]=
                      {{syscall_halt,"halt",0},{syscall_exit,"exit",1},{syscall_exec,"exec",1},
                        {syscall_wait,"wait",1},{syscall_create,"create",2},{syscall_remove,"remove",1},
                        {syscall_open,"open",1},{syscall_filesize,"filesize",1},{syscall_read,"read",1},
                        {syscall_write,"write",3},{syscall_seek,"seek",2},{syscall_tell,"tell",1},
                        {syscall_close,"close",1}};
// static inline bool
// is_user_vaddrs (const void *vaddr) 
// {
//   return vaddr < PHYS_BASE;
// }

static inline bool is_valid_vaddr(uint32_t* esp){

  int i;
  for(i=0;i<=syscall_handlers[*esp].argc;++i){
    if(!is_user_vaddr(esp+i)){
      return false;
    }
  }
  return true;
}


void
syscall_init (void) 
{
  // printf("system call init\n\n");
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  
  
  // thread_exit ();
  uint32_t* esp=f->esp; // syscall number
  if(*esp>MAX_SYSCALL_NR ){
    thread_exit();
  }
  if(!is_valid_vaddr(esp)){

    thread_current()->exit_status=-1;
    thread_exit();
  }
  // printf ("system call! %p %d\n\n",f->esp,*(uint32_t*)(f->esp));
  //handling esp error;
  

  uint32_t syscall_nr=*((uint32_t*)esp);
  struct syscall_handler_t* handler=&syscall_handlers[*esp];
  // printf("name : %s\n",handler->name);
  syscall_handlers[syscall_nr].func(esp);
}

static void syscall_halt(uint32_t* esp)
{
  int a=*esp;
  a++;
}

void syscall_exit(uint32_t* esp)
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
  // printf("syscall open\n");

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
  // printf("syscall remove\n");

  int a=*esp;
  a++;
}

static void syscall_write(uint32_t* esp)
{
  // printf("syscall write\n\n");

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