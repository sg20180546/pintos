#include "filesys/inode.h"

#include <debug.h>
#include <round.h>
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/free-map.h"
#include "threads/malloc.h"
#include "cache.h"

/* Identifies an inode. */
#define INODE_MAGIC 0x494e4f44

#define INDIRECT_NODE_ENTRIES (BLOCK_SECTOR_SIZE)/(sizeof (uint32_t))


#define DIRECT_NODE_EXTENT (BLOCK_SECTOR_SIZE)*(DIRECT_NODE_ENTIRES) // 63488
#define SINGLE_INDIRECT_EXTENT (BLOCK_SECTOR_SIZE)*(INDIRECT_NODE_ENTRIES) // 65536
#define DOUBLE_INDIRECT_EXTENT (BLOCK_SECTOR_SIZE)*(INDIRECT_NODE_ENTRIES)*(INDIRECT_NODE_ENTRIES) // 8388608

#define MAX_SECTOR_N (DIRECT_NODE_ENTIRES)+(INDIRECT_NODE_ENTRIES)+(INDIRECT_NODE_ENTRIES*INDIRECT_NODE_ENTRIES)

#define MAX_FILE_SIZE (DIRECT_NODE_EXTENT)+(SINGLE_INDIRECT_EXTENT)+(DOUBLE_INDIRECT_EXTENT)


/* On-disk inode.
   Must be exactly BLOCK_SECTOR_SIZE bytes long. */

/* Returns the number of sectors to allocate for an inode SIZE
   bytes long. */
bool bc_start;
static block_sector_t byte_to_sector (const struct inode *inode, off_t pos) ;
static inline size_t
bytes_to_sectors (off_t size)
{
  return DIV_ROUND_UP (size, BLOCK_SECTOR_SIZE);
}

/* In-memory inode. */

/* Returns the block device sector that contains byte offset POS
   within INODE.
   Returns -1 if INODE does not contain data for a byte at offset
   POS. */
static bool inode_allocate(struct inode* inode,size_t sectors){
  int pos=inode->data.length>>9; // pos : sector
  struct indirect_node_disk * indirect_node=calloc(1,sizeof* indirect_node);
  struct indirect_node_disk * double_indirect_node=calloc(1,sizeof*double_indirect_node);
  struct indirect_node_disk* single_indirect_node=calloc(1,sizeof*single_indirect_node);

  while(pos<sectors && pos<DIRECT_NODE_ENTIRES){
    free_map_allocate(1,&inode->data.direct[pos]);
    pos++;
  }

  if(pos<sectors&&pos<DIRECT_NODE_ENTIRES+INDIRECT_NODE_ENTRIES){
    free_map_allocate(1,&inode->data.single_indirect);
    memset(indirect_node,0,BLOCK_SECTOR_SIZE);
    while(pos<sectors&&pos<DIRECT_NODE_ENTIRES+INDIRECT_NODE_ENTRIES){
      free_map_allocate(1,&indirect_node->sector[pos-DIRECT_NODE_ENTIRES]);
      pos++;
    }
    block_write(fs_device,inode->data.single_indirect,indirect_node);
  }




  if(pos<sectors&&pos<MAX_SECTOR_N){
    free_map_allocate(1,&inode->data.double_indirect);
   memset(double_indirect_node,0,BLOCK_SECTOR_SIZE);


    while(pos<sectors&&pos<MAX_SECTOR_N){
      block_sector_t double_indirect_pos=(pos-DIRECT_NODE_ENTIRES-INDIRECT_NODE_ENTRIES)>>7;

      free_map_allocate(1,&double_indirect_node->sector[double_indirect_pos]);

      memset(single_indirect_node,0,BLOCK_SECTOR_SIZE);
      int i=0;
      while(pos<sectors&&i<INDIRECT_NODE_ENTRIES){
        // block_sector_t single_indirect_pos=(pos-DIRECT_NODE_ENTIRES-INDIRECT_NODE_ENTRIES)%INDIRECT_NODE_ENTRIES;
        free_map_allocate(1,&single_indirect_node->sector[i]);
        pos++;
        i++;
      }
      block_write(fs_device,  double_indirect_node->sector[double_indirect_pos], single_indirect_node);
    }

    block_write(fs_device, inode->data.double_indirect ,double_indirect_node);
  }
  free(indirect_node);
  free(double_indirect_node);
  free(single_indirect_node);
  return true;
}
void bf();


static block_sector_t
byte_to_sector (const struct inode *inode, off_t pos) // pos : byte
{
  ASSERT (inode != NULL);
  // if (pos < inode->data.length)
  //   return inode->data.start + pos / BLOCK_SECTOR_SIZE;
  // else
  //   return -1;

  block_sector_t* buf;
  block_sector_t ret;
  if(pos<DIRECT_NODE_EXTENT){
    return inode->data.direct[pos/BLOCK_SECTOR_SIZE];
  }else if(pos<DIRECT_NODE_EXTENT+SINGLE_INDIRECT_EXTENT){
    pos-=DIRECT_NODE_EXTENT;
    buf=malloc(BLOCK_SECTOR_SIZE);
    block_read(fs_device,inode->data.single_indirect,buf);
    ret=buf[pos/BLOCK_SECTOR_SIZE];
    free(buf);
    return ret;
  }else if(pos<DIRECT_NODE_EXTENT+SINGLE_INDIRECT_EXTENT+DOUBLE_INDIRECT_EXTENT){
    bf();
    pos-=(DIRECT_NODE_EXTENT+SINGLE_INDIRECT_EXTENT);
    /*
    513 : 0 1
    1025 : 0 2

    65536 + 34 : 1 0
    65536 + 513 : 1 1

    131072 + 34 : 2 0
    */
    block_sector_t double_indir=pos>>16;
    block_sector_t single_indir=pos&(SINGLE_INDIRECT_EXTENT-1)>>9;

    buf=malloc(BLOCK_SECTOR_SIZE);
    block_read(fs_device,inode->data.double_indirect,buf);
    ret=buf[double_indir];

    block_read(fs_device,ret,buf);
    ret=buf[single_indir];
    free(buf);
    return ret;
  }else{
    return -1;
  }
}

/* List of open inodes, so that opening a single inode twice
   returns the same `struct inode'. */
static struct list open_inodes;

/* Initializes the inode module. */
void
inode_init (void) 
{
  list_init (&open_inodes);
}

/* Initializes an inode with LENGTH bytes of data and
   writes the new inode to sector SECTOR on the file system
   device.
   Returns true if successful.
   Returns false if memory or disk allocation fails. */
bool
inode_create (block_sector_t sector, off_t length) //0 ,512
{
  struct inode_disk *disk_inode = NULL;
  struct inode inode;
  bool success = false;

  ASSERT (length >= 0);
  //printf("\ninode creaeting  : sector : %d length : %d\n",sector,length);
  /* If this assertion fails, the inode structure is not exactly
     one sector in size, and you should fix that. */
  // ASSERT (sizeof *disk_inode == BLOCK_SECTOR_SIZE);

  // disk_inode = calloc (1, sizeof *disk_inode);
  // if (disk_inode != NULL)
  //   {
      inode.data.length=0;
      size_t sectors = bytes_to_sectors (length); // # of sector
      inode.data.magic = INODE_MAGIC;

      success=inode_allocate(&inode,sectors);

      inode.data.length=length;
      block_write(fs_device,sector,&inode.data);
/////////////////////////////////////
    //   if (free_map_allocate (sectors, &disk_inode->start)) // sector start position
    //     {
    //       //printf("inode create at : %d start : %d// # of sectors ? %d ,\n",sector,disk_inode->start,sectors);
    //       // printf("hello length %d sectors %d, inoe start sector %d\n\n",length,sectors,disk_inode->start);
    //       block_write (fs_device, sector, disk_inode);
    //       if (sectors > 0) 
    //         {
    //           static char zeros[BLOCK_SECTOR_SIZE];
    //           size_t i;
              
    //           for (i = 0; i < sectors; i++) 
    //             block_write (fs_device, disk_inode->start + i, zeros);
    //         }
    //       success = true; 
    //     } 
    //   free (disk_inode);
    // }
  /////////////////////////////////////
  return success;
}

/* Reads an inode from SECTOR
   and returns a `struct inode' that contains it.
   Returns a null pointer if memory allocation fails. */
struct inode *
inode_open (block_sector_t sector)
{
  struct list_elem *e;
  struct inode *inode;

  /* Check whether this inode is already open. */
  for (e = list_begin (&open_inodes); e != list_end (&open_inodes);
       e = list_next (e)) 
    {
      inode = list_entry (e, struct inode, elem);
      if (inode->sector == sector) 
        {
          inode_reopen (inode);
          return inode; 
        }
    }

  /* Allocate memory. */
  inode = malloc (sizeof *inode);
  if (inode == NULL)
    return NULL;

  /* Initialize. */
  list_push_front (&open_inodes, &inode->elem);
  inode->sector = sector;
  inode->open_cnt = 1;
  inode->deny_write_cnt = 0;
  inode->removed = false;
          EXPECT_NE(3221479280,inode->sector);
  block_read (fs_device, inode->sector, &inode->data);
  return inode;
}

/* Reopens and returns INODE. */
struct inode *
inode_reopen (struct inode *inode)
{
  if (inode != NULL)
    inode->open_cnt++;
  return inode;
}

/* Returns INODE's inode number. */
block_sector_t
inode_get_inumber (const struct inode *inode)
{
  return inode->sector;
}

/* Closes INODE and writes it to disk.
   If this was the last reference to INODE, frees its memory.
   If INODE was also a removed inode, frees its blocks. */

static void inode_release(struct inode* inode){
  size_t file_sector_size=inode->data.length/BLOCK_SECTOR_SIZE + 1;
  struct inode_disk* idisk=&inode->data;
  struct indirect_node_disk * indirect_disk=malloc(sizeof*indirect_disk);
  struct indirect_node_disk* double_indirect_disk=malloc(sizeof* double_indirect_disk);

  int pos=0; // pos : sector
  while(pos<file_sector_size&&pos<DIRECT_NODE_ENTIRES ){
    free_map_release(idisk->direct[pos],1);
    pos++;
  }

  if(pos<file_sector_size&&pos<DIRECT_NODE_ENTIRES+INDIRECT_NODE_ENTRIES){
    // indirect_disk=malloc(sizeof*indirect_disk);
    block_read(fs_device,idisk->single_indirect,indirect_disk);
    while(pos<file_sector_size && pos<DIRECT_NODE_ENTIRES+INDIRECT_NODE_ENTRIES ){
      free_map_release(indirect_disk->sector[pos-DIRECT_NODE_ENTIRES],1);
      pos++;
    }
    // free(indirect_disk);
  }



  if(pos<file_sector_size&&pos<MAX_SECTOR_N){
    indirect_disk=malloc(sizeof*indirect_disk);
    block_read(fs_device,idisk->double_indirect,indirect_disk);
    while(pos<file_sector_size&&pos<MAX_SECTOR_N){

       int double_indirect_pos=(pos-DIRECT_NODE_ENTIRES-INDIRECT_NODE_ENTRIES)>>7;
       block_read(fs_device,indirect_disk->sector[double_indirect_pos],double_indirect_disk);

       int i=0;
       while(pos<file_sector_size&&i<INDIRECT_NODE_ENTRIES){
          free_map_release(double_indirect_disk->sector[i],1);
          i++;
          pos++;
       }

       free_map_release(indirect_disk->sector[double_indirect_pos],1);
    }
    free_map_release(idisk->double_indirect,1);
  }

  // while(pos<file_sector_size&&pos<MAX_SECTOR_N){
  //   struct indirect_node_disk* double_indirect_disk=malloc(sizeof* double_indirect_disk);
  //   int double_indirect_pos=(pos-DIRECT_NODE_ENTIRES-INDIRECT_NODE_ENTRIES)>>8;
  //   block_read(fs_device,indirect_disk[double_indirect_pos],double_indirect_disk);


  //   free(double_indirect_disk);
  //   // pos++;
  // }
  free(double_indirect_disk);
  free(indirect_disk);
}

void
inode_close (struct inode *inode) 
{
  /* Ignore null pointer. */
  if (inode == NULL)
    return;

  /* Release resources if this was the last opener. */
  if (--inode->open_cnt == 0)
    {
      /* Remove from inode list and release lock. */
      list_remove (&inode->elem);
 
      /* Deallocate blocks if removed. */
      if (inode->removed) 
        {
          free_map_release (inode->sector, 1);
          inode_release(inode);
          // free_map_release (inode->data.start,
          //                   bytes_to_sectors (inode->data.length)); 
        }

      free (inode); 
    }
}

/* Marks INODE to be deleted when it is closed by the last caller who
   has it open. */
void
inode_remove (struct inode *inode) 
{
  ASSERT (inode != NULL);
  inode->removed = true;
}

/* Reads SIZE bytes from INODE into BUFFER, starting at position OFFSET.
   Returns the number of bytes actually read, which may be less
   than SIZE if an error occurs or end of file is reached. */
off_t
inode_read_at (struct inode *inode, void *buffer_, off_t size, off_t offset) 
{
  uint8_t *buffer = buffer_;
  off_t bytes_read = 0;
  uint8_t *bounce = NULL;

  while (size > 0) 
    {
      /* Disk sector to read, starting byte offset within sector. */
      block_sector_t sector_idx = byte_to_sector (inode, offset);
      int sector_ofs = offset % BLOCK_SECTOR_SIZE;

      /* Bytes left in inode, bytes left in sector, lesser of the two. */
      // printf("inode lengthd ? :%d\n",inode_length(inode));
      off_t inode_left = inode_length (inode) - offset;
      int sector_left = BLOCK_SECTOR_SIZE - sector_ofs;
      int min_left = inode_left < sector_left ? inode_left : sector_left;

      /* Number of bytes to actually copy out of this sector. */
      int chunk_size = size < min_left ? size : min_left;
      if (chunk_size <= 0)
        break;
      // bc_start=false;
      if(bc_start){
        bc_read(inode,sector_idx,buffer,bytes_read,chunk_size,sector_ofs);
      }//////////////////////////////////////
      else{
      if (sector_ofs == 0 && chunk_size == BLOCK_SECTOR_SIZE)
        {
          /* Read full sector directly into caller's buffer. */
          // bc_read(sector_idx,buffer,bytes_read,chunk_size,sector_ofs);
                  EXPECT_NE(3221479280,sector_idx);
          block_read (fs_device, sector_idx, buffer + bytes_read);
          
        }
      else 
        {
          /* Read sector into bounce buffer, then partially copy
             into caller's buffer. */
          if (bounce == NULL) 
            {
              bounce = malloc (BLOCK_SECTOR_SIZE);
              if (bounce == NULL)
                break;
            }
                    EXPECT_NE(3221479280,sector_idx);
          block_read (fs_device, sector_idx, bounce);
          memcpy (buffer + bytes_read, bounce + sector_ofs, chunk_size);
        }
      ////////////////////////////////////////
      }

      /* Advance. */
      size -= chunk_size;
      offset += chunk_size;
      bytes_read += chunk_size;
    }
  free (bounce);

  return bytes_read;
}

/* Writes SIZE bytes from BUFFER into INODE, starting at OFFSET.
   Returns the number of bytes actually written, which may be
   less than SIZE if end of file is reached or an error occurs.
   (Normally a write at end of file would extend the inode, but
   growth is not yet implemented.) */
off_t
inode_write_at (struct inode *inode, const void *buffer_, off_t size,
                off_t offset) 
{
  const uint8_t *buffer = buffer_;
  off_t bytes_written = 0;
  uint8_t *bounce = NULL;

  if(offset + size > MAX_FILE_SIZE){
    return 0;
  }
  if(offset+size > inode->data.length){
    // printf("allocate new sector??\n\n");
    inode_allocate(&inode->data,bytes_to_sectors(offset+size));
  }
  // inode_allocate(&inode->data,offset+size);
  if (inode->deny_write_cnt)
    return 0;

  while (size > 0) 
    {

      block_sector_t sector_idx = byte_to_sector (inode, offset);
      int sector_ofs = offset % BLOCK_SECTOR_SIZE;

      /* Bytes left in inode, bytes left in sector, lesser of the two. */
      off_t inode_left = inode_length (inode) - offset;
      int sector_left = BLOCK_SECTOR_SIZE - sector_ofs;
      int min_left = inode_left < sector_left ? inode_left : sector_left;

      /* Number of bytes to actually write into this sector. */
      int chunk_size = size < min_left ? size : min_left;
      if (chunk_size <= 0)
        break;
      // bc_start=false;
      if(bc_start){
        bc_write(inode,sector_idx,buffer,bytes_written,chunk_size,sector_ofs);
      }else
      {/////////////////////////////////////////////////////
      if (sector_ofs == 0 && chunk_size == BLOCK_SECTOR_SIZE)
        {
          /* Write full sector directly to disk. */
                  EXPECT_NE(3221479280,sector_idx);
          block_write (fs_device, sector_idx, buffer + bytes_written);

          
        }
      else 
        {
          /* We need a bounce buffer. */
          if (bounce == NULL) 
            {
              bounce = malloc (BLOCK_SECTOR_SIZE);
              if (bounce == NULL)
                break;
            }

          /* If the sector contains data before or after the chunk
             we're writing, then we need to read in the sector
             first.  Otherwise we start with a sector of all zeros. */
          if (sector_ofs > 0 || chunk_size < sector_left) {
                    EXPECT_NE(3221479280,sector_idx);
            block_read (fs_device, sector_idx, bounce);
            }
          else
            memset (bounce, 0, BLOCK_SECTOR_SIZE);
          memcpy (bounce + sector_ofs, buffer + bytes_written, chunk_size);
          block_write (fs_device, sector_idx, bounce);
        }
      }
      /////////////////////////////////////////////////
      /* Advance. */
      size -= chunk_size;
      offset += chunk_size;
      bytes_written += chunk_size;
    }
  free (bounce);

  return bytes_written;
}

/* Disables writes to INODE.
   May be called at most once per inode opener. */
void
inode_deny_write (struct inode *inode) 
{
  inode->deny_write_cnt++;
  ASSERT (inode->deny_write_cnt <= inode->open_cnt);
}

/* Re-enables writes to INODE.
   Must be called once by each inode opener who has called
   inode_deny_write() on the inode, before closing the inode. */
void
inode_allow_write (struct inode *inode) 
{
  ASSERT (inode->deny_write_cnt > 0);
  ASSERT (inode->deny_write_cnt <= inode->open_cnt);
  inode->deny_write_cnt--;
}

/* Returns the length, in bytes, of INODE's data. */
off_t
inode_length (const struct inode *inode)
{
  return inode->data.length;
}
