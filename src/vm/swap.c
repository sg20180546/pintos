#include "swap.h"
#include "devices/block.h"
#include <bitmap.h>
#include <debug.h>
static struct block* swap_device;
static struct bitmap* swap_free_map;

void swap_in(struct kpage_t* page){
    int i;
    block_sector_t sector=bitmap_scan_and_flip(swap_free_map,0,SECTOR_PER_PAGE,false);
    // printf("Swap in to %d,%p %d\n",sector,page->vme->vaddr,SECTOR_PER_PAGE);
    for(i=0;i<SECTOR_PER_PAGE;i++){
        block_write(swap_device,(sector+i),page->kaddr + (i*BLOCK_SECTOR_SIZE)); 
    }
    page->vme->swap_sector=sector;
}
size_t swap_out(struct kpage_t* page){
    int i;
    ASSERT(page->vme->swap_sector!=-1);
    // printf("swap out read from %d, vaddr :%p\n",page->vme->swap_sector,page->vme->vaddr);
    memset(page->kaddr,0,PGSIZE);
    bitmap_set_multiple(swap_free_map,page->vme->swap_sector,SECTOR_PER_PAGE,false);
    // bitmap_set(swap_free_map,page->vme->swap_sector,false);
    for(i=0;i<SECTOR_PER_PAGE;i++){
        block_read(swap_device, page->vme->swap_sector+i ,page->kaddr + (i*BLOCK_SECTOR_SIZE));
    }
    page->vme->swap_sector=-1;
}

void swap_deallocate(struct kpage_t* page){
    if(page->vme->swap_sector==-1){
        return;
    }
     bitmap_set_multiple(swap_free_map,page->vme->swap_sector,SECTOR_PER_PAGE,false);
}

void swap_init(void){
    swap_device=block_get_role(BLOCK_SWAP);
    if (swap_device == NULL)
        PANIC ("No file swap device found, can't initialize file swap.");
    swap_free_map=bitmap_create(block_size(swap_device));
    if (swap_free_map == NULL)
        PANIC ("bitmap creation failed--swap device is too large");

}
