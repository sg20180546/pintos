#include "init.h"
static void delete_list(struct list* list){
    struct list_elem* iter;
    struct list_elem* removed;
    for(iter=list_begin(list);iter!=list_end(list);){
        removed=iter;
        iter=list_remove(iter);
        free(removed);
    }
}

static void hash_desctructor(struct hash_elem *e, void *aux UNUSED){
    list_remove(&e->list_elem);
    free(e);
}

static void delete_hash(struct hash* hash){
    hash_destroy(hash,hash_desctructor);
}

static void delete_bitmap(struct bitmap* bitmap){
    bitmap_destroy(bitmap);
}

void free_struct_elem(struct struct_elem* s_elem){
    list_remove(&s_elem->elem);
    
    if(s_elem->s_type!=BITMAP) {
        free(s_elem->p);
    }
    free(s_elem);
}

static struct struct_elem* find_from_all_list(char* name);



static void dumpdata(char** args,struct struct_elem* s_elem){

    switch (s_elem->s_type)
    {
    case LIST:
        list_dump(s_elem->p);
        break;
    case HASH:
        hash_dump(s_elem->p);
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
    switch (s_elem->s_type)
    {
    case BITMAP:
        delete_bitmap(s_elem->p);
        break;
    case HASH:
        delete_hash(s_elem->p);
        break;
    case LIST:
        delete_list(s_elem->p);
        break;
    default:
        break;
    }
    free_struct_elem(s_elem);
}

static void list_push_back_wrapper(char** args,struct struct_elem* s_elem){
    // printf("%s %s %s\n",args[0],args[1],args[2]);
    int data=atoi(args[2]);
    struct list_item* new=malloc(sizeof *new);
    new->data=data;
    
    list_push_back((struct list*) s_elem->p,&new->elem);
}

static void list_push_front_wrapper(char** args,struct struct_elem* s_elem){
    int data=atoi(args[2]);
    struct list_item* new=malloc(sizeof *new);
    new->data=data;
    
    list_push_front((struct list*) s_elem->p,&new->elem);
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

static unsigned hash_hash(const struct hash_elem *e, void *aux UNUSED){
    return hash_int(e->data);
}

static bool hash_less(const struct hash_elem *a, const struct hash_elem *b, void *aux){
    if(hash_int(a->data)>=hash_int(b->data)){
        return false;
    }
    return true;
}

static void hash_insert_wrapper(char** args, struct struct_elem* s_elem){
    int data=atoi(args[2]);
    struct hash_elem* h_elem=malloc(sizeof *h_elem);
    h_elem->data=data;
    hash_insert(s_elem->p,h_elem);
}

static void square(struct hash_elem *e, void *aux UNUSED){
    e->data=e->data*e->data;
}

static void triple(struct hash_elem *e, void *aux UNUSED){
    e->data=(e->data)*(e->data)*(e->data);
}

static void hash_apply_wrapper(char** args, struct struct_elem* s_elem){
    char* func=args[2];
    if(!strcmp(func,"square")){
        hash_apply(s_elem->p,square);
    }else if(!strcmp(func,"triple")) {
        hash_apply(s_elem->p,triple);
    }
}

static void hash_delete_wrapper(char** args, struct struct_elem* s_elem){
    int data=atoi(args[2]);
    struct hash_elem h_elem;
    struct hash_elem* res;
    h_elem.data=data;
    res=hash_find(s_elem->p,&h_elem);
    if(res){
        hash_delete(s_elem->p,res);
    }
}

static void hash_size_wrapper(char** args UNUSED, struct struct_elem* s_elem){
    size_t size=hash_size(s_elem->p);
    printf("%ld\n",size);
}

static void hash_empty_wrapper(char** args UNUSED, struct struct_elem* s_elem){
    if(hash_empty(s_elem->p)){
        printf("true\n");
    }else{
        printf("false\n");
    }
}

static void hash_find_wrapper(char** args UNUSED, struct struct_elem* s_elem){
    int data=atoi(args[2]);
    struct hash_elem h_elem;
    struct hash_elem* res;
    h_elem.data=data;
    res=hash_find(s_elem->p,&h_elem);
    if(res!=NULL){
        printf("%d\n",data);
    }
}

static void hash_clear_wrapper(char** args UNUSED, struct struct_elem* s_elem){
    hash_clear(s_elem->p,NULL);
}

static void hash_replace_wrapper(char** args UNUSED, struct struct_elem* s_elem){
    int newdata=atoi(args[2]);
    struct hash_elem* h_elem=malloc(sizeof *h_elem);
    h_elem->data=newdata;
    hash_replace(s_elem->p,h_elem);
}

static void list_pop_back_wrapper(char** args UNUSED,struct struct_elem* s_elem){
    struct list_elem* list_elem=list_pop_back(s_elem->p);
    struct list_item* l_item=list_entry(list_elem,struct list_item,elem);
    free(l_item);
}

static void list_pop_front_wrapper(char** args UNUSED,struct struct_elem* s_elem){
    struct list_elem* list_elem=list_pop_front(s_elem->p);
    struct list_item* l_item=list_entry(list_elem,struct list_item,elem);
    free(l_item);
}

static void list_front_wrapper(char** args UNUSED,struct struct_elem* s_elem){
    printf("%d\n",list_entry(list_front(s_elem->p),struct list_item,elem)->data );
}

static void list_back_wrapper(char** args,struct struct_elem* s_elem){
    printf("%d\n",list_entry(list_back(s_elem->p),struct list_item,elem)->data);
}

bool list_elem_cmp_descending(const struct list_elem* a, const struct list_elem* b, void *aux UNUSED){
    if(list_entry(a,struct list_item,elem)->data <=list_entry(b,struct list_item,elem)->data){
        return true;
    }
    return false;
}

bool list_elem_cmp_ascending(const struct list_elem* a, const struct list_elem* b, void *aux UNUSED){
    if(list_entry(a,struct list_item,elem)->data> list_entry(b,struct list_item,elem)->data){
        return false;
    }
    return true;
}

static void list_insert_ordered_wrapper(char** args, struct struct_elem* s_elem){
    struct list_item* list_item=malloc(sizeof *list_item);
    list_item->data=atoi(args[2]);

    list_insert_ordered(s_elem->p,&list_item->elem,list_elem_cmp_ascending,NULL);
}

static void list_insert_wrapper(char** args, struct struct_elem* s_elem){
    int idx=atoi(args[2]);
    int i;
    struct list_item* list_item=malloc(sizeof *list_item);
    struct list_elem* iter;
    list_item->data=atoi(args[3]);
    for(i=0,iter=list_begin((struct list*)s_elem->p); i<idx ;i++,iter=list_next(iter));
    list_insert(iter,&list_item->elem);
    
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
    printf("%d\n",list_entry(list_max((struct list*) s_elem->p,list_elem_cmp_ascending,NULL),struct list_item,elem)->data);
}

static void list_min_wrapper(char** args UNUSED, struct struct_elem* s_elem){
    void* aux=NULL;
    printf("%d\n",list_entry(list_min((struct list*) s_elem->p,list_elem_cmp_descending,aux),struct list_item,elem)->data);
}

static void list_remove_wrapper(char** args, struct struct_elem* s_elem){
    int idx=atoi(args[2]);
    int i;
    struct list_elem* iter;
    struct list_item* item;
    for(i=0,iter=list_begin((struct list*)s_elem->p); i<idx ;i++,iter=list_next(iter));
    list_remove(iter);
    item=list_entry(iter,struct list_item,elem);
    free(item);
}

static void list_reverse_wrapper(char** args UNUSED,struct struct_elem* s_elem){
    list_reverse(s_elem->p);
}

static void list_shuffle_wrapper(char** args UNUSED,struct struct_elem* s_elem){
    list_shuffle(s_elem->p);
}

static void list_sort_wrapper(char** args,struct struct_elem* s_elem){
    list_sort(s_elem->p,list_elem_cmp_ascending,NULL);
}

static void list_splice_wrapper(char** args,struct struct_elem* s_elem){
    int idx=atoi(args[2]);
    struct struct_elem* src;
    src=find_from_all_list(args[3]);
    int start=atoi(args[4]);
    int end=atoi(args[5]);

    struct list_elem* before;
    struct list_elem* first;
    struct list_elem* last;
    int i;
    for(i=0,before=list_begin((struct list*)s_elem->p);i<idx;i++,before=list_next(before));
    // list_splice
    for(i=0,first=list_begin((struct list*)src->p);i<start;i++,first=list_next(first));
    for(i=0,last=list_begin((struct list*)src->p);i<end;i++,last=list_next(last) );
    list_splice(before,first,last);
}

static void list_swap_wrapper(char** args,struct struct_elem* s_elem){
    int i;
    struct list_elem* l1;
    struct list_elem* l2;
    int idx1=atoi(args[2]);
    int idx2=atoi(args[3]);

    for(i=0,l1=list_begin((struct list*)s_elem->p);i<idx1;i++,l1=list_next(l1));
    for(i=0,l2=list_begin((struct list*)s_elem->p);i<idx2;i++,l2=list_next(l2));
    list_swap(l1,l2);
}

static bool list_less(const struct list_elem *a, const struct list_elem *b, void *aux){
    if(list_entry(a,struct list_item,elem)->data > list_entry(b,struct list_item,elem)->data){
        return true;
    }
    return false;
}
static void list_unique_wrapper(char** args,struct struct_elem* s_elem){
    struct struct_elem* duplicates=NULL;

    duplicates=find_from_all_list(args[2]);


    if(duplicates!=NULL){
        list_unique(s_elem->p,duplicates->p,list_less,NULL);
    }else{
        list_unique(s_elem->p,NULL,list_less,NULL);
    }
}

struct util_wrapper wrapper_list[]={
                {"dumpdata",dumpdata},{"delete",delete_wrapper},
                /* bitmap */
                {"bitmap_mark",bitmap_mark_wrapper},{"bitmap_all",bitmap_all_wrapper},{"bitmap_any",bitmap_any_wrapper},
                {"bitmap_contains",bitmap_contains_wrapper},{"bitmap_count",bitmap_count_wrapper},
                {"bitmap_dump",bitmap_dump_wrapper},{"bitmap_expand",bitmap_expand_wrapper},{"bitmap_mark",bitmap_mark_wrapper},
                {"bitmap_set_all",bitmap_set_all_wraper},{"bitmap_flip",bitmap_flip_wrapper},
                {"bitmap_none",bitmap_none_wrapper},{"bitmap_scan_and_flip",bitmap_scan_and_flip_wrapper},{"bitmap_scan",bitmap_scan_wrapper},
                {"bitmap_reset",bitmap_reset_wrapper},{"bitmap_set_multiple",bitmap_set_multiple_wrapper},
                {"bitmap_set",bitmap_set_wrapper},{"bitmap_size",bitmap_size_wrapper},{"bitmap_test",bitmap_test_wrapper},
                // /* hash */
                {"hash_insert",hash_insert_wrapper},{"hash_apply",hash_apply_wrapper},{"hash_delete",hash_delete_wrapper},
                {"hash_size",hash_size_wrapper},{"hash_clear",hash_clear_wrapper},{"hash_empty",hash_empty_wrapper},
                {"hash_find",hash_find_wrapper},{"hash_replace",hash_replace_wrapper},
                // /* list */
                {"list_push_back",list_push_back_wrapper},{"list_push_front",list_push_front_wrapper},
                {"list_pop_back",list_pop_back_wrapper},{"list_pop_front",list_pop_front_wrapper},
                {"list_front",list_front_wrapper},{"list_back",list_back_wrapper},
                {"list_insert_ordered",list_insert_ordered_wrapper},{"list_insert",list_insert_wrapper},
                {"list_empty",list_empty_wrapper},{"list_size",list_size_wrapper},{"list_max",list_max_wrapper},
                {"list_min",list_min_wrapper},{"list_remove",list_remove_wrapper},{"list_reverse",list_reverse_wrapper},
                {"list_shuffle",list_shuffle_wrapper},{"list_sort",list_sort_wrapper},{"list_splice",list_splice_wrapper},
                {"list_swap",list_swap_wrapper},{"list_unique",list_unique_wrapper}
                };






static struct struct_elem* find_from_all_list(char* name){
    struct list_elem* iter;
    struct struct_elem* s_iter;
    struct struct_elem* ret=NULL;
    for(iter=list_begin(&all_list);iter!=list_end(&all_list);iter=list_next(iter)){
        s_iter=list_entry(iter,struct struct_elem,elem);
        if(!strcmp(name,s_iter->name)){
            ret=s_iter;
            break;
        }
    }
    return ret;
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
                hash=malloc(sizeof*hash);
                hash_init(hash,hash_hash,hash_less,NULL);
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
        s_elem=find_from_all_list(name);
        // if(!strcmp(args[0],"delete")){
        //     delete_wrapper()
        //     continue;            
        // }

        for(i=0;i<47;i++){
            if(!strcmp(args[0],wrapper_list[i].name)){
                wrapper_list[i].wrapper(args,s_elem);
                break;
            }
        }



    }
    NOT_REACHED();
}