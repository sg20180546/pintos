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
                "bitmap_mark","bitmap_all","bitmap_contains","bitmap_count"
                ,"delete","quit"};

char cmd[1024];
int main() NO_RETURN{

    while(1){
        scanf("%s",cmd);

    }
}