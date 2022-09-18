#include "list.h"
#include "hash.h"
#include "debug.h"
#include "bitmap.h"
#include "limits.h"
#include "hex_dump.h"
#include <stdio.h>
// struct command{

// };
char* cmd_list[]={"create","dumpdata",
                /* bitmap */
                "bitmap_mark","bitmap_all","bitmap_contains","bitmap_count",
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

char cmd[1024];
int main() {

    // while(1){
    //     scanf("%s",cmd);

    // }
    NO_RETURN;
}