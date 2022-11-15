#ifndef VM_SWAP_H
#define VM_SWAP_H
#include <stddef.h>
#include "vm/page.h"

void swap_deallocate(struct kpage_t* page);

void swap_init(void);
void swap_in(struct kpage_t* page);
size_t swap_out(struct kpage_t* page);


#endif