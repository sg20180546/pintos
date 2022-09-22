#ifndef INIT_H_
#define INIT_H_
#include "list.h"
#include "hash.h"
#include "debug.h"
#include "bitmap.h"
#include "limits.h"
#include "hex_dump.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SPACE ' '
#define TAB '\t'
#define ENTER '\n'

#define whitespace(x) ( ( (x) ==SPACE) || ( (x) == TAB)  )
enum struct_type {HASH, LIST, BITMAP};
struct list all_list;
struct struct_elem {
    enum struct_type s_type;
    char name[32];
    void* p;
    struct list_elem elem;
};
struct util_wrapper{
    char name[64];
    void (*wrapper)(char**, struct struct_elem *);
};

char cmd[1024];

int argc;
int start;
int pos;
#endif
