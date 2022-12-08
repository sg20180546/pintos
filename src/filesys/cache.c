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
    cache->dirty=false;
    cache->ref=false;
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

    p_flush_entry->dirty=false;
    block_write(fs_device,p_flush_entry->sector,p_flush_entry->data);
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


bool bc_read(struct inode* inode,block_sector_t sector_idx, void* buffer,off_t bytes_read, int chunk_size, int sector_ofs){
    // find first
    struct buffer_head* bc=bc_lookup(sector_idx);
    
    //if there no cache
    if(bc==NULL){
        // evict
        bc=bc_select_victim();
        // construct buffer cache
        block_read(fs_device,sector_idx,bc->data);
        bc->inode=inode;
    }else{

    }

    // copy to buffer
    memcpy(buffer+bytes_read,bc->data+sector_ofs,chunk_size);
    bc->ref=true;
}

struct buffer_head* bc_select_victim(void){
    int idx=(bc_clock-buffer_cache_table)/sizeof(struct buffer_head);
    struct buffer_head* cur;
    while (1)
    {
        for(;idx<CACHE_SIZE;idx++){
            cur=&buffer_cache_table[idx];
            if(cur->sector==UNUNSED){
                return cur;
            }
            else if(cur->ref==true){
                cur->ref=false;
            }else{
                if(cur->dirty==true){
                    bc_flush_entry(&cur);
                }
                bc_clock=&buffer_cache_table[idx+1];
                return cur;
            }
        }
    }
    NOT_REACHED();
}


bool bc_write(block_sector_t sector_idx, void* buffer,off_t bytes_written,int chunk_size,int sector_ofs){
    // find first
    
    // if not , read to buffer cache

    // select victim

    // write that buffer cache

}