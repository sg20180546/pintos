#include "page.h"

static unsigned vm_hash_func(const struct hash_elem* h_elem,void* aux){
    struct vm_entry* vm_entry=hash_entry(h_elem,struct vm_entry,h_elem);

    // remove offset, use threads/pte.h
    // uint32_t VPN=vm_entry&
    // return hash_int(vm_entry->vaddr);
}

static bool vm_less_func(const struct hash_elem* a, const struct hash_elem* b, void* aux){
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


bool insert_vme(struct hash* vm, struct vm_entry* vme){
    hash_insert(vm,&vme->h_elem);
}
bool delete_vme(struct hash* vm, struct vm_entry* vme){
    hash_delete(vm,&vme->h_elem);
}

struct vm_entry* find_vme(void* vaddr){
    // hash_find()
}

void vm_destroy(struct hash* vm){
    
}