#include "swap.h"
#include "devices/block.h"
#include <bitmap.h>
#include <debug.h>
static struct block* swap_device;
static struct bitmap* swap_free_map;

void swap_in(struct kpage_t* page){
    // page round down kaddr
    block_sector_t sector=bitmap_scan_and_flip(swap_free_map,0,1,false);
    block_write(swap_device,sector,page->kaddr); 
    page->vme->swap_sector=sector;
    // page->vme->loaded_on_phys=false;
    // list_remove(&page->elem);
    // list_remove(&page->lru_elem);
}
size_t swap_out(struct kpage_t* page){
    bitmap_set(swap_free_map,page->vme->swap_sector,false);
    block_read(swap_device,page->vme->swap_sector,page->kaddr);
    page->vme->swap_sector=-1;
}

void swap_deallocate(struct kpage_t* page){
    if(page->vme->swap_sector==-1){
        return;
    }
    bitmap_set(swap_free_map,page->vme->swap_sector,false);
}

void swap_init(void){
    swap_device=block_get_role(BLOCK_SWAP);
    if (swap_device == NULL)
        PANIC ("No file swap device found, can't initialize file swap.");
    swap_free_map=bitmap_create(block_size(swap_device));
    if (swap_free_map == NULL)
        PANIC ("bitmap creation failed--swap device is too large");

}
