#ifndef VM_PAGE_H
#define VM_PAGE_H
#include "threads/thread.h"
#include "lib/kernel/hash.h"
#include "threads/palloc.h"
#include "threads/vaddr.h"
enum {
    VM_BIN, VM_FILE, VM_ANON
};

struct vm_entry
{
    uint8_t type;
    void* vaddr;
    bool read_only;

    bool loaded_on_phys;
    struct file* file;

    struct list_elem mmap_elem;

    size_t offset;
    size_t read_bytes;
    size_t zero_bytes;

    size_t swap_slot;

    struct hash_elem h_elem;
};

struct page{
    void* kaddr; // physcial
    struct vm_entry* vme;
    struct thread* thread;
    struct list_elem lru;
};

struct mmap_file{
    int mapid;
    struct file* file;
    struct list_elem elem;
    struct list vme_list;
};

void vm_init(struct hash* vm);
void insert_vme(struct hash* vm, struct vm_entry* vme);
void delete_vme(struct hash* vm, struct vm_entry* vme);
struct vm_entry* find_vme(void* vaddr);
void vm_destroy(struct hash* vm);
#endif