#include "userprog/syscall.h"

#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "userprog/process.h"
#include "lib/string.h"
#include "devices/shutdown.h"

#define MAX_SYSCALL_NR 13
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
typedef void syscall_handler_func (struct intr_frame*);

// struct list open_file_list;

struct syscall_handler_t {
  syscall_handler_func* func;
  char name[128]; // for debuging
  char argc;
};

static struct file* find_file_by_fd(int fd,struct thread* cur) {
  struct list_elem* iter;
  struct file* f_iter;
  struct list* open_file_list=&cur->open_file_list;
  for(iter=list_begin(open_file_list);iter!=list_end(open_file_list);iter=list_next(iter)) {
    f_iter=list_entry(iter,struct file,elem);
    if(f_iter->fd==fd){
      return f_iter;
    }
  }
  return NULL;
}
bool is_open_file_executing(const char* file){

  struct list_elem* iter;
  struct thread* t_iter;

  for(iter=list_begin(&all_list);iter!=list_end(&all_list);iter=list_next(iter)) {
    t_iter=list_entry(iter,struct thread,allelem);

    if(!strcmp(file,t_iter->name)){

      return true;
    }
  }
  return false;
}
static inline void exit(int status){
  thread_current()->exit_status=status;
  thread_exit();
}

static void syscall_handler (struct intr_frame *);

static void syscall_halt(struct intr_frame* f);

static void syscall_exit(struct intr_frame* f);

static void syscall_exec(struct intr_frame* f);

static void syscall_wait(struct intr_frame* f);

static void syscall_create(struct intr_frame* f);

static void syscall_remove(struct intr_frame* f);

static void syscall_open(struct intr_frame* f);

static void syscall_filesize(struct intr_frame* f);

static void syscall_read(struct intr_frame* f);

static void syscall_write(struct intr_frame* f);

static void syscall_seek(struct intr_frame* f);
 
static void syscall_tell(struct intr_frame* f);

static void syscall_close(struct intr_frame* f);

struct syscall_handler_t syscall_handlers[]=
                      {{syscall_halt,"halt",0},{syscall_exit,"exit",1},{syscall_exec,"exec",1},
                        {syscall_wait,"wait",1},{syscall_create,"create",2},{syscall_remove,"remove",1},
                        {syscall_open,"open",1},{syscall_filesize,"filesize",1},{syscall_read,"read",1},
                        {syscall_write,"write",3},{syscall_seek,"seek",2},{syscall_tell,"tell",1},
                        {syscall_close,"close",1}};


static inline bool is_valid_vaddr(uint32_t * esp){

  int i;
  if(*esp>MAX_SYSCALL_NR){
    return false;
  }
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
  
  
  uint32_t* esp=f->esp;

  if(!is_valid_vaddr(esp)){
    thread_current()->exit_status=-1;
    thread_exit();
  }  

  uint32_t syscall_nr=*((uint32_t*)esp);
  struct syscall_handler_t* handler=&syscall_handlers[syscall_nr];
  // printf("name : %s\n",handler->name);
  handler->func(f);
}

static void syscall_halt(struct intr_frame* f)
{
  shutdown_power_off();
}

static void syscall_exit(struct intr_frame* f)
{
  uint32_t* esp= f->esp;
  thread_current()->exit_status=*(++esp);
  thread_exit();
}

static void syscall_exec(struct intr_frame* f)
{ 
  
  uint32_t* esp= f->esp;
  const char* file=*(++esp);

  f->eax=process_execute(file);
}

static void syscall_wait(struct intr_frame* f)
{
  uint32_t* esp= f->esp;
  int fd=*(++esp);
  f->eax=process_wait(fd);
}

static void syscall_create(struct intr_frame* f)
{

  uint32_t* esp= f->esp;
  const char* file=*(++esp);
  unsigned initial_size=*(++esp);


  f->eax=filesys_create(file,initial_size);
  // EXPECT_EQ(ret,false);
  // asm volatile
  // (
  //   "movl %1, %%eax\n\t"
  //   "movl %%eax, %0\n\t"
  //   :"=m"(check)
  //   :"m"(ret)
  //   :"eax"
  // );
  // EXPECT_EQ(ret,check);
}

static void syscall_remove(struct intr_frame* f)
{

  uint32_t* esp= f->esp;
  const char* file=*(++esp);
  
}

static void syscall_open(struct intr_frame* f)
{  
  uint32_t* esp= f->esp;
  const char* file=*(++esp);
  if(file==NULL){
    return;
  }
  struct file* file_struct=filesys_open(file);
  if(is_open_file_executing(file)){
    file_deny_write(file_struct);
  }

  if(file_struct==NULL){
    f->eax=-1;
  }else{
    f->eax=file_struct->fd;
  }
}

static void syscall_filesize(struct intr_frame* f)
{
  uint32_t* esp= f->esp;
  int fd=*(++esp);
  struct file* file_struct=find_file_by_fd(fd,thread_current());
  int ret=0;
  if(file_struct){
    ret=file_length(file_struct);
  }
  f->eax=ret;
} 

static void syscall_read(struct intr_frame* f)
{
  uint32_t* esp= f->esp;
  int fd=*(++esp);
  void* buffer=*(++esp);
  unsigned size=*(++esp);
  

  if(!is_user_vaddr(buffer) ){
    exit(-1);
  }

  if(fd==STDOUT_FILENO||fd==STDIN_FILENO||fd<0){
    exit(-1);
  }

  struct file* file_struct=find_file_by_fd(fd,thread_current());

  if(file_struct==NULL){
    exit(-1);
  }else{
    f->eax=file_read(file_struct,buffer,size);
  }
}

static void syscall_write(struct intr_frame* f)
{
  uint32_t* esp= f->esp;
  int fd=*(++esp);
  char* buffer=*(++esp);
  int size=*(++esp);
  int ret;
  if(!is_user_vaddr(buffer)||!is_user_vaddr(*buffer)||fd<0 ){
    ret=-1;
  }
  
  if(fd==STDOUT_FILENO){
    putbuf(buffer,size);
  }else{
    struct file* file =find_file_by_fd(fd,thread_current());
    if(file==NULL){
      ret=-1;
    }else{
      if(file->deny_write){
        ret=0;
      }else{
        ret=file_write(file,buffer,size);
      }
      // ret=file_write(file,buffer,size);
    }
  }
  // asm volatile
  // (
  //   "movl %1, %%eax\n\t"
  //   "movl %%eax, %0\n\t"
  //   :"=m"(check)
  //   :"m"(size)
  //   :"eax"
  // );
  // ASSERT(check==size);
  f->eax=ret;
}

static void syscall_seek(struct intr_frame* f)
{
  uint32_t* esp= f->esp;
  int fd=*(++esp);
  off_t pos=*(++esp);
  struct file* file=find_file_by_fd(fd,thread_current());
  file->pos=pos;
}
 
static void syscall_tell(struct intr_frame* f)
{
  uint32_t* esp= f->esp;
  esp++;
}

static void syscall_close(struct intr_frame* f)
{
  uint32_t* esp=f->esp;
  int fd=*(++esp);
  struct file* file_struct= find_file_by_fd(fd,thread_current());
  
  if(file_struct){
    file_allow_write(file_struct);
    file_close(file_struct);
  }
}