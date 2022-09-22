#include "init.h"
void delete_list(struct list* list){

}
void free_struct_elem(struct struct_elem* s_elem){
    list_remove(&s_elem->elem);
    free(s_elem->p);
    free(s_elem);
}





static void dumpdata(char** args,struct struct_elem* s_elem){

    switch (s_elem->s_type)
    {
    case LIST:
        list_dump(s_elem->p);
        break;
    case HASH:
        break;

    case BITMAP:

        bitmap_dump_bit(s_elem->p);
        break;
    default:
        break;
    }
}

static void bitmap_mark_wrapper(char** args,struct struct_elem* s_elem ){
    int idx=atoi(args[2]);
    bitmap_mark((struct bitmap*) (s_elem->p),idx);
}

static void bitmap_all_wrapper(char** args,struct struct_elem* s_elem){
    size_t start=atoi(args[2]);
    size_t cnt=atoi(args[3]);
    if(bitmap_all(s_elem->p,start,cnt)){
        printf("true\n");
    }else{
        printf("false\n");
    }
}

static void bitmap_any_wrapper(char** args,struct struct_elem* s_elem){
    size_t start=atoi(args[2]);
    size_t cnt=atoi(args[3]);
    if(bitmap_any(s_elem->p,start,cnt)){
        printf("true\n");
    }else{
        printf("false\n");
    }
}

static void bitmap_contains_wrapper(char** args,struct struct_elem* s_elem){
    size_t start=atoi(args[2]);
    size_t cnt=atoi(args[3]);
    bool value=false;
    if(!strcmp(args[4],"true")){
        value=true;
    }
    if(bitmap_contains(s_elem->p,start,cnt,value)){
        printf("true\n");
    }else{
        printf("false\n");
    }
}

static void bitmap_count_wrapper(char** args,struct struct_elem* s_elem){
    size_t start=atoi(args[2]);
    size_t cnt=atoi(args[3]);
    bool value=false;
    if(!strcmp(args[4],"true")){
        value=true;
    }    
    printf("%ld\n",bitmap_count(s_elem->p,start,cnt,value));
}

static void bitmap_dump_wrapper(char** args UNUSED,struct struct_elem* s_elem){
    bitmap_dump(s_elem->p);
}

static void bitmap_expand_wrapper(char** args,struct struct_elem* s_elem){
    int size=atoi(args[2]);
    s_elem->p=bitmap_expand(s_elem->p,size);
}


static void bitmap_set_all_wraper(char** args,struct struct_elem* s_elem){
    bool value=false;
    if(!strcmp(args[2],"true")){
        value=true;
    }
    bitmap_set_all(s_elem->p,value);
}

static void delete_wrapper(char** args UNUSED,struct struct_elem* s_elem){
    free_struct_elem(s_elem);
}

static void list_push_back_wrapper(char** args,struct struct_elem* s_elem){
    // printf("%s %s %s\n",args[0],args[1],args[2]);
    int data=atoi(args[2]);

    struct list_elem* new=malloc(sizeof *new);
    new->data=data;
    
    list_push_back((struct list*) s_elem->p,new);
}

static void list_push_front_wrapper(char** args,struct struct_elem* s_elem){
    int data=atoi(args[2]);
    struct list_elem* new=malloc(sizeof *new);
    new->data=data;
    
    list_push_front((struct list*) s_elem->p,new);
}

static void bitmap_flip_wrapper(char** args, struct struct_elem* s_elem){
    int idx=atoi(args[2]);
    bitmap_flip(s_elem->p,idx);
}

static void bitmap_none_wrapper(char** args,struct struct_elem* s_elem){
    size_t start=atoi(args[2]);
    size_t cnt=atoi(args[3]);
    if(bitmap_none(s_elem->p,start,cnt)){
        printf("true\n");
    }else{
        printf("false\n");
    }
}

static void bitmap_reset_wrapper(char** args,struct struct_elem* s_elem){
    int idx=atoi(args[2]);
    bitmap_reset(s_elem->p,idx);
}

static void bitmap_scan_and_flip_wrapper(char** args, struct struct_elem* s_elem){
    size_t start=atoi(args[2]);
    size_t cnt=atoi(args[3]);
    bool value=false;
    if(!strcmp(args[4],"true")){
        value=true;
    }
    
    printf("%lu\n",bitmap_scan_and_flip(s_elem->p,start,cnt,value));
}
static void bitmap_scan_wrapper(char** args,struct struct_elem* s_elem){
    size_t start=atoi(args[2]);
    size_t cnt=atoi(args[3]);
    bool value=false;
    if(!strcmp(args[4],"true")){
        value=true;
    }
    printf("%lu\n",bitmap_scan(s_elem->p,start,cnt,value));
}

static void bitmap_set_multiple_wrapper(char** args,struct struct_elem* s_elem){
    size_t start=atoi(args[2]);
    size_t cnt=atoi(args[3]);
    bool value=false;
    if(!strcmp(args[4],"true")){
        value=true;
    }
    bitmap_set_multiple(s_elem->p,start,cnt,value);
}


static void bitmap_set_wrapper(char** args ,struct struct_elem* s_elem){
    int idx=atoi(args[2]);
    bool value=false;
    if(!strcmp(args[3],"true")){
        value=true;
    }
    bitmap_set(s_elem->p,idx,value);
}

static void bitmap_size_wrapper(char** args UNUSED,struct struct_elem* s_elem){
    printf("%lu\n",bitmap_size(s_elem->p));
}

static void bitmap_test_wrapper(char** args,struct struct_elem* s_elem){
    int idx=atoi(args[2]);
    if(bitmap_test(s_elem->p,idx)){
        printf("true\n");
    }else{
        printf("false\n");
    }
}

static void hash_insert_wrapper(char** args, struct struct_elem* s_elem){
    // hash_insert(s_elem->p,)
}

static void list_pop_back_wrapper(char** args UNUSED,struct struct_elem* s_elem){
    struct list_elem* list_elem=list_pop_back(s_elem->p);
    free(list_elem);
}

static void list_pop_front_wrapper(char** args UNUSED,struct struct_elem* s_elem){
    struct list_elem* list_elem=list_pop_front(s_elem->p);
    free(list_elem);
}

static void list_front_wrapper(char** args UNUSED,struct struct_elem* s_elem){
    printf("%d\n",list_front(s_elem->p)->data);
}

static void list_back_wrapper(char** args,struct struct_elem* s_elem){
    printf("%d\n",list_back(s_elem->p)->data);
}

bool list_elem_cmp_descending(const struct list_elem* a, const struct list_elem* b, void *aux UNUSED){
    if(a->data<=b->data){
        return true;
    }
    return false;
}

bool list_elem_cmp_ascending(const struct list_elem* a, const struct list_elem* b, void *aux UNUSED){
    if(a->data>b->data){
        return false;
    }
    return true;
}

static void list_insert_ordered_wrapper(char** args, struct struct_elem* s_elem){
    struct list_elem* list_elem=malloc(sizeof *list_elem);
    list_elem->data=atoi(args[2]);
    list_insert_ordered(s_elem->p,list_elem,list_elem_cmp_ascending,NULL);
}

static void list_insert_wrapper(char** args, struct struct_elem* s_elem){
    int idx=atoi(args[2]);
    int i;
    struct list_elem* list_elem=malloc(sizeof *list_elem);
    struct list_elem* iter;
    list_elem->data=atoi(args[3]);
    for(i=0,iter=list_begin((struct list*)s_elem->p); i<idx ;i++,iter=list_next(iter));
    list_insert(iter,list_elem);
    
}

static void list_empty_wrapper(char** args UNUSED, struct struct_elem* s_elem){
    if(list_empty(s_elem->p)){
        printf("true\n");
    }else{
        printf("false\n");
    }
}
static void list_size_wrapper(char** args UNUSED, struct struct_elem* s_elem){
    printf("%lu\n",list_size(s_elem->p));
}

static void list_max_wrapper(char** args UNUSED, struct struct_elem* s_elem){
    printf("%d\n",list_max((struct list*) s_elem->p,list_elem_cmp_ascending,NULL)->data);
}

static void list_min_wrapper(char** args UNUSED, struct struct_elem* s_elem){
    void* aux=NULL;
    printf("%d\n",list_min( (struct list*) s_elem->p,list_elem_cmp_descending,aux)->data);
}

static void list_remove_wrapper(char** args, struct struct_elem* s_elem){
    int idx=atoi(args[2]);
    int i;
    struct list_elem* iter;
    for(i=0,iter=list_begin((struct list*)s_elem->p); i<idx ;i++,iter=list_next(iter));
    list_remove(iter);
    free(iter);
}

static void list_reverse_wrapper(char** args,struct struct_elem* s_elem){
    list_reverse(s_elem->p);
}

static void list_shuffle_wrapper(char** args,struct struct_elem* s_elem){

}

static void list_sort_wrapper(char** args,struct struct_elem* s_elem){
    list_sort(s_elem->p,list_elem_cmp_ascending,NULL);
}

static void list_splice_wrapper(char** args,struct struct_elem* s_elem){
    // list_splice()
}

static void list_swap_wrapper(char** args,struct struct_elem* s_elem){
    
}

struct util_wrapper wrapper_list[]={
                {"dumpdata",dumpdata},
                /* bitmap */
                {"bitmap_mark",bitmap_mark_wrapper},{"bitmap_all",bitmap_all_wrapper},{"bitmap_any",bitmap_any_wrapper},
                {"bitmap_contains",bitmap_contains_wrapper},{"bitmap_count",bitmap_count_wrapper},
                {"bitmap_dump",bitmap_dump_wrapper},{"bitmap_expand",bitmap_expand_wrapper},{"bitmap_mark",bitmap_mark_wrapper},
                {"bitmap_set_all",bitmap_set_all_wraper},{"bitmap_flip",bitmap_flip_wrapper},
                {"bitmap_none",bitmap_none_wrapper},{"bitmap_scan_and_flip",bitmap_scan_and_flip_wrapper},{"bitmap_scan",bitmap_scan_wrapper},
                {"bitmap_reset",bitmap_reset_wrapper},{"bitmap_set_multiple",bitmap_set_multiple_wrapper},
                {"bitmap_set",bitmap_set_wrapper},{"bitmap_size",bitmap_size_wrapper},{"bitmap_test",bitmap_test_wrapper},
                // /* hash */
                // "hash_insert","hash_apply","hash_delete","hash_size",
                // "hash_clear","hash_empty","hash_find","hash_replace"
                // /* list */
                {"list_push_back",list_push_back_wrapper},{"list_push_front",list_push_front_wrapper},
                {"list_pop_back",list_pop_back_wrapper},{"list_pop_front",list_pop_front_wrapper},
                {"list_front",list_front_wrapper},{"list_back",list_back_wrapper},
                {"list_insert_ordered",list_insert_ordered_wrapper},{"list_insert",list_insert_wrapper},
                {"list_empty",list_empty_wrapper},{"list_size",list_size_wrapper},{"list_max",list_max_wrapper},
                {"list_min",list_min_wrapper},{"list_remove",list_remove_wrapper},{"list_reverse",list_reverse_wrapper},
                {"list_shuffle",list_shuffle_wrapper},{"list_sort",list_sort_wrapper},{"list_splice",list_splice_wrapper},
                {"list_swap",list_swap_wrapper},
                // "list_unique"
                // ,"delete","quit"
                };






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






int main() {
    int i;
    char** args=malloc(sizeof(char*)*8);
    for(i=0;i<8;i++){
        args[i]=malloc(64);
    }
    list_init(&all_list);

    while(1){

        for(i=0;i<8;i++){
            memset(args[i],0,64);
        }
        start=0;
        argc=0;
        pos=0;


        fgets(cmd,1024,stdin);
        // parsing
        while(cmd[start]!='\n'){
            while(whitespace(cmd[pos])) {
                pos++;
            }
            start=pos;
            if(cmd[pos]==ENTER) continue;

            while(cmd[pos]!=ENTER&&cmd[pos]!=SPACE){ 
                pos++;
            }
            strncpy(args[argc++],&cmd[start],pos-start);
            start=pos;
        }
        // printf("aftger parse : %s %s %s\n",args[0],args[1],args[2]);



        /* create struct */
        if(!strcmp(args[0],"create")){

            struct struct_elem* s_elem=malloc(sizeof *s_elem);
            char* name=args[2];

            
            if(!strcmp(args[1],"bitmap")){//bitmap
                size_t size=atoi(args[3]);
                // struct bitmap* bitmap=bitmap_create_with_name(size,name);
                struct bitmap* bitmap=bitmap_create(size);
                s_elem->p=bitmap;
                s_elem->s_type=BITMAP;
            }else if(!strcmp(args[1],"hashtable")){
            //hash 
                struct hash* hash; // create
                s_elem->p=hash;
                s_elem->s_type=HASH;
            }else{
                struct list* list= malloc(sizeof *list);
                list_init(list);
                s_elem->p=list;
                s_elem->s_type=LIST;
            }
            ASSERT(s_elem->p);
            strcpy(s_elem->name,name);
            list_push_back(&all_list,&s_elem->elem);
            continue;
        }

        if(!strcmp(args[0],"quit")){
            exit(0);
        }

        char*name=args[1];
        struct struct_elem* s_elem;
        int i;

        find_from_all_list(name,&s_elem);
        for(i=0;i<40;i++){
            if(!strcmp(args[0],wrapper_list[i].name)){
                wrapper_list[i].wrapper(args,s_elem);
                break;
            }
        }



    }
    NOT_REACHED();
}