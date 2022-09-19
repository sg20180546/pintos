#include "list.h"
#include "hash.h"
#include "debug.h"
#include "bitmap.h"
#include "limits.h"
#include "hex_dump.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct struct_elem;

struct util_wrapper{
    char name[64];
    inline void (*wrapper)(char**,struct struct_elem *);
};

inline void bitmap_mark_wrapper(char**args,struct struct_elem* s_elem ){
    int idx=atoi(args[2]);
    bitmap_mark(idx,s_elem->p);
}




struct util_wrapper* wrapper_list[]={
                /* bitmap */
                {"bitmap_mark",bitmap_mark_wrapper},"bitmap_all","bitmap_contains","bitmap_count",
                "bitmap_dump","bitmap_expand","bitmap_mark","bitmap_set_all",
                "bitmap_flip","bitmap_none","bitmap_scan_and_flip","bitmap_set_multiple",
                "bitmap_set","bitmap_size","bitmap_test",
                /* hash */
                "hash_insert","hash_apply","hash_delete","hash_size",
                "hash_clear","hash_empty","hash_find","hash_replace"
                /* list */
                "list_push_back","list_push_front",
                "list_pop_back","list_pop_front",
                "list_front","list_back","list_insert_ordered","list_insert",
                "list_empty","list_size","list_max","list_min","list_remove",
                "list_reverse","list_shuffle","list_sort","list_splice","list_swap",
                "list_unique"
                ,"delete","quit"};
// enum cmd_n{
//     CREATE,DUMPDATA,
//                 /* bitmap */
//                 BITMAP_MARK,BITMAP_ALL,BITMAP_CONTAINS,BITMAP_COUNT,
//                 BITMAP_DUMP,BITMAP_EXPAND,BITMAP,"bitmap_set_all",
//                 "bitmap_flip","bitmap_none","bitmap_scan_and_flip","bitmap_set_multiple",
//                 "bitmap_set","bitmap_size","bitmap_test",
//                 /* hash */
//                 "hash_insert","hash_apply","hash_delete","hash_size",
//                 "hash_clear","hash_empty","hash_find","hash_replace"
//                 /* list */
//                 "list_push_back","list_push_front",
//                 "list_pop_back","list_pop_front",
//                 "list_front","list_back","list_insert_ordered","list_insert",
//                 "list_empty","list_size","list_max","list_min","list_remove",
//                 "list_reverse","list_shuffle","list_sort","list_splice","list_swap",
//                 "list_unique"
//                 ,"delete","quit"
// };



enum struct_type{HASH,LIST,BITMAP};
struct struct_elem {
    enum struct_type s_type;
    char name[32];
    void* p;
    struct list_elem elem;
};
void free_struct_elem(struct struct_elem* s_elem){
    list_remove(&s_elem->elem);
    free(s_elem->p);
    free(s_elem);
}




struct list all_list;

void find_from_all_list(char* name,struct struct_elem** s_elem){
    struct list_elem* iter;
    struct struct_elem* s_iter;
    for(iter=list_begin(&all_list);iter!=list_end(&all_list);iter=list_next(iter)){
        s_iter=list_entry(iter,struct struct_elem,elem);
        if(!strcmp(name,s_iter->name)){
            break;
        }
    }
    ASSERT(s_iter->p);
    *s_elem=s_iter;
}

char cmd[1024];


int argc;
int start;
int pos;
int main() {
    list_init(&all_list);
    while(1){
        char args[64][8];
        start=0;
        argc=0;
        pos=0;

        scanf("%s",cmd);

        // parsing
        while(cmd[start]!='\n'){
            while(cmd[pos]!=' '){
                pos++;
            }
            
            strlcpy(args[argc++],&cmd[start],pos-start+1);
            while(cmd[pos]==' '){
                pos++;
            }

            if(cmd[pos]=='\n'){
                break;
            }
            start=pos;
        }



        /* create struct */
        if(!strcmp(args[0],"create")){
            struct struct_elem* s_elem=malloc(sizeof *s_elem);
            char* name=args[2];
            switch (args[1][0])
            {   
            case 'b': //bitmap
                size_t size=atoi(args[3]);
                // struct bitmap* bitmap=bitmap_create_with_name(size,name);
                struct bitmap* bitmap=bitmap_create(size);
                s_elem->p=bitmap;
                s_elem->s_type=BITMAP;
                break;
            case 'h': //hash 
                struct hash* hash; // create
                s_elem->p=hash;
                s_elem->s_type=HASH;
                break;
            case 'l':
                struct list* list= malloc(sizeof *list);
                list_init(list);
                s_elem->p=list;
                s_elem->s_type=LIST;
                break;
            default:
                break;
            }
            list_push_back(&all_list,&s_elem->elem);
            continue;
        }

        /* dumpdata name */
        if(!strcmp(args[0],"dumpdata")){
            char* name=args[1];

            struct struct_elem* s_elem;

            find_from_all_list(name,&s_elem);

            ASSERT(s_elem!=list_end(&all_list));
            ASSERT(s_elem->p);
            switch (s_elem->s_type)
            {
            case BITMAP:
                /* code */
                bitmap_dump(s_elem->p);
                break;
            case HASH:
                
                break;
            case LIST:
                list_dump(s_elem->p);
                break;
            default:
                break;
            }
            continue;
        }

        if(!strcmp(args[0],"delete")){
            char* name=args[1];

            struct struct_elem* s_elem;
            find_from_all_list(name,&s_elem);

            ASSERT(s_elem!=list_end(&all_list));
            ASSERT(s_elem->p);
            free_struct_elem(s_elem);
            continue;;
        }
        if(!strcmp(args[0],"quit")){
            exit(0);
        }

        char*name=args[1];
        struct struct_elem* s_elem;
        find_from_all_list(name,&s_elem);
        switch (s_elem->s_type)
        {
        case BITMAP:








            break;
        case HASH:
            break;
        case LIST:
            break;
        default:
            break;
        }



    }
    NO_RETURN;
}