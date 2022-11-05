#include "page.h"

struct hash lru_list;

static unsigned vm_hash_func(const struct hash_elem* helem,void* aux UNUSED){
    struct vm_entry* vm_entry=hash_entry(helem,struct vm_entry,h_elem);

    return hash_int(pg_round_down(vm_entry->vaddr));
}

static bool vm_less_func(const struct hash_elem* a, const struct hash_elem* b, void* aux UNUSED){
    struct vm_entry* va=hash_entry(a,struct vm_entry,h_elem);
    struct vm_entry* vb=hash_entry(b,struct vm_entry,h_elem);
    if(vb->vaddr>va->vaddr){
        return true;
    }
    return false;
}


void vm_init(struct hash* vm){
    hash_init(vm,vm_hash_func,vm_less_func,NULL);
}


inline void insert_vme(struct hash* vm, struct vm_entry* vme){
    hash_insert(vm,&vme->h_elem);
}
inline void delete_vme(struct hash* vm, struct vm_entry* vme){
    hash_delete(vm,&vme->h_elem);
}

struct vm_entry* find_vme(void* vaddr){

    struct thread* cur=thread_current();
    struct vm_entry src;
    src.vaddr=pg_round_down(vaddr);
    return hash_entry(hash_find(&cur->vm,&src.h_elem),struct vm_entry,h_elem);
}

static void destroy_vme(struct hash_elem *e, void *aux UNUSED){

    struct vm_entry* vme=hash_entry(e,struct vm_entry,h_elem);
    free(vme);
}

void vm_destroy(struct hash* vm){
    hash_destroy(vm,destroy_vme);
}




// static unsigned kpage_t_hash_func(const struct hash_elem* helem,void* aux UNUSED){
//     struct kpage_t* kpage=hash_entry(helem,struct kpage_t,h_elem);
//     return hash_int(kpage->kaddr);
// }

// static bool kpage_t_less_func(const struct hash_elem* a, const struct hash_elem* b, void* aux UNUSED){
//     struct kpage_t* ka=hash_entry(a,struct kpage_t,h_elem);
//     struct kpage_t* kb=hash_entry(b,struct kpage_t,h_elem);
//     if(kb->kaddr>ka->kaddr){
//         return true;
//     }
//     return false;
// }

// void lru_init(struct hash* lru){
//     hash_init(lru,kpage_t_hash_func,kpage_t_less_func,NULL);
// }

// inline void insert_kpage(struct kpage* kpage){
//     hash_insert(&lru_list,&kpage->h_elem);
// }

// inline void delete_kpage(struct kpage_t*kpage){

// }