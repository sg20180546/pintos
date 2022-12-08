#include "cache.h"
#include "lib/debug.h"
#include "lib/string.h"

struct block *fs_device;



void bct_init(void){
    int i;
    for(i=0;i<CACHE_SIZE;i++){
        bc_init(&buffer_cache_table[i],NULL,UNUNSED);
    }
    bc_clock=&buffer_cache_table[0];
}

void bc_init(struct buffer_head* cache,struct inode* inode,block_sector_t sector){
    cache->inode=inode;
    cache->bit&=0x0;
    cache->sector=sector;
    memset(cache->data,0,BLOCK_SECTOR_SIZE);
    lock_init(&cache->lock);
}

void bc_flush_all_entries(void){
    int i;
    for(i=0;i<CACHE_SIZE;i++){
        bc_flush_entry(&buffer_cache_table[i]);
    }
}

void bc_flush_entry(struct buffer_head* p_flush_entry){
    EXPECT_NE(fs_device,NULL);
    lock_acquire(&p_flush_entry->lock);
    // p_flush_entry->inode;
    // int sector=p_flush_entry->inode.;t
    // struct inode* inode=p_flush_entry->inode;
    
    block_write(fs_device,p_flush_entry->inode->data.start,p_flush_entry->data);
    lock_release(&p_flush_entry->lock);
}

struct buffer_head* bc_lookup(block_sector_t sector){ 
    int i;
    for(i=0;i<CACHE_SIZE;i++){
        if(buffer_cache_table[i].sector==sector){
            return &buffer_cache_table[i];
        }
    }

    return NULL;
}


bool bc_read(block_sector_t sector_idx, void* buffer,off_t bytes_read, int chunk_size, int sector_ofs){
    struct buffer_head* bc=bc_lookup(sector_idx);
    if(bc==NULL){
        // evict
        bc=bc_select_victim();
        // read to buffer cach
        // copy to buffer
    }else{
        memcpy(buffer+bytes_read,bc->data,chunk_size);
        SET_REF(bc->bit);
    }
}

struct buffer_head* bc_select_victim(void){
    int idx=(bc_clock-buffer_cache_table)/sizeof(struct buffer_head);
    while (1)
    {
        for(;idx<CACHE_SIZE;idx++)
            if(IS_REF(&buffer_cache_table[i]/)){

            }
        }
    }
    
}