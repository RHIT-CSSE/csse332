// Copyright 2009 Steve Gribble (gribble [at] cs.washington.edu)
// May 22nd, 2009
//
// This file defines the os_fs_metadata_t structure, which you'll
// create and populate in the early part of this project, and which
// you'll use in later parts of the project.  As well, this file
// defines function prototypes for the functions you need to implement
// as you make your way through the assignment.

#ifndef EXT2READER_INC_EXT2ACCESS_H
#define EXT2READER_INC_EXT2ACCESS_H

#include "types.h"
#include "blockgroup_descriptor.h"
#include "directoryentry.h"
#include "inode.h"
#include "superblock.h"

// For each block group, this structure tracks the block numbers of
// the first and last block in that blockgroup.
struct os_blockgroup_offsets_t {
  // numbering note: block number 0 is defined as the first block on
  // the disk
  os_uint32_t first_block_in_blockgroup;  // blocknum of 1st block
  os_uint32_t last_block_in_blockgroup;   // blocknum of last block
};

// some useful metadata that you will calculate about the disk
struct os_fs_metadata_t {
  os_uint32_t disk_size;              // # of bytes in disk image
  os_uint32_t block_size;             // blocksize, as defined in superblock
  os_uint32_t num_blocks;             // # of blocks in this disk
  os_uint32_t blockgroup_size;        // # of blocks per blockgroup
  os_uint32_t inodes_per_group;       // # inodes per blockgroup
  os_uint32_t inode_blocks_per_group; // # of inode blocks per blockgroup
  os_uint32_t num_blockgroups;        // # of blockgroups in this disk
  os_uint32_t num_blocks_per_desc_table;  // # blocks in a descriptor table

  // pointer to the blockgroup descriptor table (one entry per
  // block group).  you'll malloc space for this.
  struct os_blockgroup_descriptor_t *bgdt;
};

// Function prototypes for the functions you'll implement.
//
void load_ext2_metadata(int fd, struct os_fs_metadata_t *metadata);

// this one is actually done for you
void calc_metadata(int fd, struct os_superblock_t *sb, struct os_fs_metadata_t *output);

os_bool_t fetch_inode(os_uint32_t inode_number, int fd,
                      struct os_fs_metadata_t *metadata,
                      struct os_inode_t *returned_inode);


void calculate_offsets(os_uint32_t blocknum,
                       os_uint32_t blocksize,
                       os_int32_t *direct_num,
                       os_int32_t *indirect_index,
                       os_int32_t *double_index,
                       os_int32_t *triple_index);

os_uint32_t file_blockread(struct os_inode_t file_inode, int fd,
                           struct os_fs_metadata_t *metadata,
                           os_uint32_t blocknum, unsigned char *buffer);

os_bool_t file_read(int fd, int file_inode_num,
                    struct os_fs_metadata_t *metadata,
                    struct os_inode_t *inode, unsigned char **buffer);

os_bool_t pop_dir_component(char *path,
                            char **next_component);

os_uint32_t scan_dir(unsigned char *directory,
                     os_uint32_t directory_length,
                     char *filename);

os_uint32_t path_to_inode_num(char* path, int fd, struct os_fs_metadata_t *metadata);

os_bool_t path_read(char *path, int fd,
                    struct os_fs_metadata_t *metadata,
                    unsigned char **buffer, os_uint32_t *len);

#endif  // EXT2READER_INC_EXT2ACCESS_H

