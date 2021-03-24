// Copyright 2009 Steve Gribble (gribble [at] cs.washington.edu)
// May 22nd, 2009
//
// This is the header file for the test/grading code; it declares
// prototypes for all of the test functions that are invoked within
// ext2reader.c.  You don't need to modify anything here.

#ifndef EXT2READER_INC_TESTCODE_H
#define EXT2READER_INC_TESTCODE_H

#include "types.h"
#include "superblock.h"
#include "blockgroup_descriptor.h"
#include "ext2access.h"

void check_metadata(struct os_fs_metadata_t *fsm);

void check_fetch_inode(int fd, struct os_fs_metadata_t *metadata,
                       struct os_blockgroup_descriptor_t *blockgroup_desc_table);
void check_calculate_offsets(void);

void check_file_blockread(int fd, struct os_fs_metadata_t *metadata);

void check_file_read(int fd, struct os_fs_metadata_t *metadata);

void check_pop_dir_component(void);

void check_scan_dir(int fd, struct os_fs_metadata_t *metadata);

void check_path_read(int fd, struct os_fs_metadata_t *metadata);

void print_grade(void);

#endif // EXT2READER_INC_TESTCODE_H
