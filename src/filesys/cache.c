#include "cache.h"
#include "lib/debug.h"
#include "lib/string.h"
#include "threads/malloc.h"
struct block *fs_device;

bool bc_start;

void bct_init(void){
    
    int i;
    for(i=0;i<CACHE_SIZE;i++){
        buffer_cache_table[i]=malloc(sizeof(struct buffer_head));
        bc_init(buffer_cache_table[i],NULL,UNUNSED);
    }
    lock_init(&bc_lock);
    // bc_clock=buffer_cache_table;
    bc_clock_idx=0;
    bc_start=true;
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
    // lock_acquire(&bc_lock);
    for(i=0;i<CACHE_SIZE;i++){
        // lock_acquire(&buffer_cache_table[i]->lock);
        bc_flush_entry(buffer_cache_table[i]);
        // lock_release(&buffer_cache_table[i]->lock);
    }
    // lock_release(&bc_lock);
}

void bc_flush_entry(struct buffer_head* p_flush_entry){
    EXPECT_NE(fs_device,NULL);


    p_flush_entry->dirty=false;

    if(p_flush_entry->sector!=UNUNSED){
        block_write(fs_device,p_flush_entry->sector,p_flush_entry->data);
    }

}

struct buffer_head* bc_lookup(block_sector_t sector){ 
    int i;
    // lock_acquire(&bc_lock);
    struct buffer_head* ret=NULL;
    for(i=0;i<CACHE_SIZE;i++){
        EXPECT_NE(buffer_cache_table[i],NULL);
        // printf("%d\n",sector);
        if(buffer_cache_table[i]->sector==sector){
            ret=buffer_cache_table[i];
            break;
        }
    }
    // lock_release(&bc_lock);
    return ret;
}


bool bc_read(struct inode* inode,block_sector_t sector_idx, void* buffer,off_t bytes_read, int chunk_size, int sector_ofs){
    // find first
    lock_acquire(&bc_lock);
    struct buffer_head* bc=bc_lookup(sector_idx);
    
    //if there no cache
    if(bc==NULL){
        // evict
        bc=bc_select_victim();
        bc_init(bc,inode,sector_idx);
        block_read(fs_device,sector_idx,bc->data);


    }else{

    }

    // copy to buffer
    memcpy(buffer+bytes_read,bc->data+sector_ofs,chunk_size);
    bc->ref=true;
    lock_release(&bc_lock);
    return true;
}

struct buffer_head* bc_select_victim(void){
    //int idx=(bc_clock-buffer_cache_table)/sizeof(struct buffer_head*);
    struct buffer_head* cur;
    // printf("index : %d \n",bc_clock_idx);
    while (1)
    {
        for(;bc_clock_idx<CACHE_SIZE;bc_clock_idx++){
            cur=buffer_cache_table[bc_clock_idx];
            // if(lock_try_acquire(&cur->lock)){
                if(cur->sector==UNUNSED){
                    // lock_release(&cur->lock);
                    bc_clock_idx++;
                    goto ret;
                    //return cur;
                }
                else if(cur->ref==true){
                    cur->ref=false;
                    // lock_release(&cur->lock);
                }else{
                    if(cur->dirty==true){
                        bc_flush_entry(cur);
                    }
                    bc_clock_idx++;
                    // lock_release(&cur->lock);
                    goto ret;
                }
            // }
        }
        bc_clock_idx=0;
    }
    ret:
        return cur;
}


bool bc_write(struct inode* inode,block_sector_t sector_idx, void* buffer,off_t bytes_written,int chunk_size,int sector_ofs){
    // find first
    lock_acquire(&bc_lock);
    struct buffer_head* bc=bc_lookup(sector_idx);
    bool new_bc_flag=false;
    // uint8_t* bounce=NULL;
    // if not , read to buffer cache
    if(bc==NULL){
        bc=bc_select_victim();
        // block_read(fs_device,sector_idx,bc->data);
        bc_init(bc,inode,sector_idx);
        //block_read()
        new_bc_flag=true;
    }
    // lock_acquire(&bc->lock);
    EXPECT_NE(bc,NULL);
    // select victim

    // write that buffer cache
    if(sector_ofs==0&&chunk_size==BLOCK_SECTOR_SIZE){
        memcpy(bc->data,buffer+bytes_written,chunk_size);
    }else{
        if(new_bc_flag==true){
            EXPECT_NE(3221479280,sector_idx);
            block_read(fs_device,sector_idx,bc->data);
        }
        memcpy(bc->data+sector_ofs,buffer+bytes_written,chunk_size);
    }

    bc->dirty=true;
    bc->ref=true;
    // lock_release(&bc->lock);
    lock_release(&bc_lock);
}

void bc_term(void){
    bc_flush_all_entries();
    for(int i=0;i<CACHE_SIZE;i++){
        free(buffer_cache_table[i]);
    }
    bc_start=false;
}