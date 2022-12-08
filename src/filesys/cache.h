#ifndef FILESYS_CACHE_H_
#define FILESYS_CACHE_H_
#include "lib/kernel/list.h"
#include "threads/synch.h"
#include "devices/block.h"
#include "filesys/inode.h"

#define IS_DIRTY(b) 0x1&(b)
#define IS_REF(b) 0x2&(b)

#define SET_DIRTY(b) (b)&=0x1
#define SET_REF(b) (b)&=0x2

#define CACHE_SIZE 64
#define UNUNSED -1

struct buffer_head{
    uint8_t bit;
    struct inode* inode;
    block_sector_t sector;
    uint8_t data[BLOCK_SECTOR_SIZE];
    struct lock lock;
};

struct buffer_head* bc_clock;

struct buffer_head buffer_cache_table[CACHE_SIZE];

void bct_init(void);

void bc_init(struct buffer_head* cache,struct inode* inode,block_sector_t sector);

bool bc_read(block_sector_t sector_idx, void* buffer,off_t bytes_read, int chunk_size, int sector_ofs);

bool bc_write(block_sector_t sector_idx, void* buffer,off_t bytes_written,int chunk_size,int sector_ofs);

void bc_term(void);

struct buffer_head* bc_select_victim(void);

struct buffer_head* bc_lookup(block_sector_t sector);

void bc_flush_entry(struct buffer_head* p_flush_entry);

void bc_flush_all_entries(void);

#endif