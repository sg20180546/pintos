#include "cache.h"
#include "lib/debug.h"
#include "lib/string.h"

struct block *fs_device;



void bct_init(void){
    for(int i=0;i<CACHE_SIZE;i++){
        bc_init(&buffer_cache_table[i],NULL,UNUNSED);
    }
}

void bc_init(struct buffer_head* cache,struct inode* inode,block_sector_t sector){
    cache->inode=inode;
    cache->bit&=0x0;
    cache->sector=sector;
    memset(cache->data,0,BLOCK_SECTOR_SIZE);
    lock_init(&cache->lock);
}

void bc_flush_all_entries(void){
    for(int i=0;i<CACHE_SIZE;i++){
        bc_flush_entry(&buffer_cache_table[i]);
    }
}

void bc_flush_entry(struct buffer_head* p_flush_entry){
    EXPECT_NE(fs_device,NULL);
    lock_acquire(&p_flush_entry->lock);
    // p_flush_entry->inode;
    // int sector=p_flush_entry->inode.;
    // block_write(fs_device,p_flush_entry->inode->data,p_flush_entry->data);
    lock_release(&p_flush_entry->lock);
}