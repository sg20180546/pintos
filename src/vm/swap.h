#ifndef VM_SWAP_H
#define VM_SWAP_H
#include <stddef.h>
#include "vm/page.h"
struct block* swap_device;

void swap_init(void);
void swap_in(struct kpage_t* page);
size_t swap_out(struct kpage_t* page);

#endif