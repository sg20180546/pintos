#ifndef FILESYS_CACHE_H_
#define FILESYS_CACHE_H_
#include "lib/kernel/list.h"
struct buffer_head{
    void * data;
    struct list_elem* elem;
};

#endif