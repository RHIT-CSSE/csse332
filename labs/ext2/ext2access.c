// Copyright 2009 Steve Gribble (gribble [at] cs.washington.edu).
// May 22nd, 2009.
//
// This file is where you'll finish the implementation of the various
// routines that understand and retrieve data from ext2's on-disk data
// structure.  Start from the project 3 light web page, and follow
// the directions on it to figure out what to do next.

#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

#include "inc/types.h"
#include "inc/blockgroup_descriptor.h"
#include "inc/directoryentry.h"
#include "inc/ext2access.h"
#include "inc/inode.h"
#include "inc/superblock.h"

// Reads the superblock populates the metadata structure which we'll
// use in later functions
//
// note that metadata is an output parameter
void load_ext2_metadata(int fd, struct os_fs_metadata_t* metadata) {
  // The superblock of the first block group is always located at
  // offset 1024.  It can be as big as a block, but practically we
  // only care about the first sizeof(struct os_superblock_t) bytes.

  // 1. Use lseek and read to dump that data directly into the
  // superblock_data variable (note that we've carefully constructured
  // this os_superblock_t struct to correspond exactly to the
  // superblock's data layout on the disk)
  struct os_superblock_t superblock_data;

  // YOUR LOADING CODE HERE
  
  // 2. All the data we need is in the superblock and groupblock
  // descriptor table (right after the superblock, but we need to read
  // the superblock first to figure out its size)...but its not in a
  // form that's most convenient.  So for your covenience we've built
  // a function called "calc_metadata" that transforms the superblock
  // data and pulls in the relevent groupblock descriptor table stuff
  // too.  If you've loaded the superblock properly this should fill
  // the metadata structure with everything it needs.

  // We'ved provided this code and and calc_metadata for you but it's
  // a good idea to look it over.
  calc_metadata(fd, &superblock_data, metadata);
  
}

// Given a superblock, calculate certain parameters of interest about
// the filesystem, and cache them in a struct os_fs_metadata_t
// structure.
void calc_metadata(int fd, struct os_superblock_t *sb, struct os_fs_metadata_t* fsm) {

  // Use fstat to figure out the size of the disk image file, and
  // store that
  struct stat mystat;
  fstat(fd, &mystat);
  fsm->disk_size = mystat.st_size;
  
  // Calculate the block size using the s_log_block_size field in
  // the superblock structure.  Be sure to look at
  // the comments next to the "s_log_blocksize" field in
  // inc/superblock.h.
  fsm->block_size = 1024 << sb->s_log_block_size;
  

  fsm->num_blocks = sb->s_blocks_count;
      
  fsm->inodes_per_group = sb->s_inodes_per_group;
  

  fsm->blockgroup_size = sb->s_blocks_per_group;

  // Calculate how many inode blocks there are per block group, and
  // store it in your allocated metadata structure.
  //
  // To do this, we need to use:
  //    (a) the number of inodes per group as reported in the
  //        superblock (sb->s_inodes_per_group)
  //    (b) the block size.
  //    (c) the size of an inode, as reported in the superblock
  //        (sb->s_inode_size).
  float inode_space = sb->s_inodes_per_group*sb->s_inode_size;
  fsm->inode_blocks_per_group = ceil(inode_space/fsm->block_size);

  // 8. Calculate how any blockgroups there are in this disk.
  // Remember that the first block group is defined to start at block
  // sb->s_first_data_block on the disk
  //
  // If the disk doesn't contain a perfect multiple of the block group
  // size, we need to figure out if the last group, which will be
  // smaller than normal, is big eough to support all the necessary
  // data structures plus have some space left over for data blocks.
  // 
  // A block group must be big enough to store:
  //    - a block for the block bitmap
  //    - a block for the inode bitmap
  //    - a block for the superblock
  //    - N blocks for the group descriptor table. you must calculate
  //      N, given how many block groups there are, and the fact that
  //      each block group descriptor is 32 bytes long.  You'll want
  //      to round up of you need a fractional # of blocks. 
  //    - M blocks to store inodes, as calculated in step 5 above.
  //    - at least 50 data blocks
  //


  // Calculate the # blockgroups, rounding down for now - we'll
  // fix this up soon.
  fsm->num_blockgroups = (fsm->num_blocks - sb->s_first_data_block) /
      fsm->blockgroup_size;
  // calculate the number of blocks remaining, if we can't fit a
  // perfect # of blockgroups in the disk.
  os_uint32_t remainder = (fsm->num_blocks - sb->s_first_data_block) %
    fsm->blockgroup_size;
  // calculate "N", the # of blocks needed to store the descriptor table,
  // pessimistically assuming we need an extra descriptor if remainder
  // is non-zero.
  os_uint32_t num_descriptortable_blocks =
    ((fsm->num_blockgroups + (remainder == 0 ? 0 : 1)) *
     sizeof(struct os_blockgroup_descriptor_t)) / fsm->block_size;
  // unlikely that the descriptor table fits in exactly an integral
  // number of blocks, so calculate the remainder.
  os_uint32_t ndb_remainder =
    ((fsm->num_blockgroups + (remainder == 0 ? 0 : 1)) *
     sizeof(struct os_blockgroup_descriptor_t)) % fsm->block_size;
  // if there is a remainder, increase "N" to fit the partial block.
  if (ndb_remainder > 0) {
    num_descriptortable_blocks += 1;
  }
  // now we have enough information to calculate the amount of
  // overhead per block group...
  os_uint32_t overhead =
    3 + num_descriptortable_blocks + fsm->inode_blocks_per_group;
  // see if we can fit a final block group in the # of blocks
  // remaining...
  if (remainder >= overhead + 50) {
    // yes, the partial blockgroup is big enough, so include it!
    fsm->num_blockgroups++;
  }

  // Calculate how many blocks are in the group descriptor
  // table
  num_descriptortable_blocks =
    (fsm->num_blockgroups * sizeof(struct os_blockgroup_descriptor_t)) /
    fsm->block_size;
  ndb_remainder =
    (fsm->num_blockgroups * sizeof(struct os_blockgroup_descriptor_t)) %
    fsm->block_size;
  if (ndb_remainder > 0) {
    num_descriptortable_blocks++;
  }
  fsm->num_blocks_per_desc_table = num_descriptortable_blocks;

  // Now load the blockgroup descriptor table.  Just like the
  // superblock, this is duplicated in every blockgroup (i.e. each
  // blockgroup has a descriptor table that contains info on ALL the
  // blockgroups).  It doesn't matter which one we use so we use the
  // first one.
  
  // Malloc enough space for the descriptor table.  To calculate
  // the number of bytes used by the descriptor table, multiply
  // fsm->block_size by fsm->num_blocks_per_desc_table.

  // Note it is kinda sketchy to malloc something as part of an
  // internal substructure because the caller might not realise this
  // exists and therefore not free it.  We do, but it's risky.
  struct os_blockgroup_descriptor_t *bgd_table;
  int bgd_size = fsm->block_size * fsm->num_blocks_per_desc_table;
  bgd_table = malloc(bgd_size);
  assert(bgd_table != NULL);
  
  int starting_block = sb->s_first_data_block + 1;
  assert(lseek(fd, starting_block*fsm->block_size, SEEK_SET) == starting_block*fsm->block_size);

  assert(read(fd, (void*) bgd_table, bgd_size) >= 0);

  fsm->bgdt = bgd_table;
}

// Fetches the inode for the given inode number, and returns a copy
// through the "returned_inode" argument.  If successful, returns
// TRUE.  If unsuccessful (e.g., the inode number is too big given
// this volume), returns FALSE.  Uses the information in
// metadata and blockgroup_desc_table to do its job.
os_bool_t fetch_inode(os_uint32_t inode_number, int fd,
                      struct os_fs_metadata_t *metadata,
                      struct os_inode_t *returned_inode) {
  // Step 1.  Figure out which blockgroup the inode is in.
  // Remember that the you stashed the number of inodes per
  // blockgroup in "metadata".  Also note that the first inode
  // has number 1, **not** number 0.  (Thanks a lot, ext2!)
  os_uint32_t blockgroup_num = 0xDEADBEEF;  // you must fix this.

  // Step 2.  Figure out the offset of the inode within
  // the group (i.e., is this the 0th inode in the group?
  // the 10th?  the 100th?)
  os_uint32_t offset_within_blockgroup = 0xDEADBEEF;  // you must fix this.

  // Step 3.  Make sure this inode number is valid -- i.e., verify
  // that the blockgroup number that you calculated makes sense, given
  // how many blockgroups are on this filesystem.  If it is not valid,
  // return FALSE.

  // Step 4.  Calculate the block # that this inode lives in.
  // Remember that the layout of the blockgroup is stored within that
  // blockgroup's descriptor table entry.  In particular, the
  // descriptor entry contains the field "bg_inode_table", which tells
  // you the block number of the first block of the inode area on
  // disk.  Since each inode is sizeof(struct os_inode_t) bytes, you
  // can figure out the block number that the inode lives in with
  // division.

  // we've provided you the code that does this.
  os_uint32_t num_inodes_per_block =
    metadata->block_size / sizeof(struct os_inode_t);

  os_uint32_t inode_block_num =
    offset_within_blockgroup / num_inodes_per_block;
  inode_block_num +=
    metadata->bgdt[blockgroup_num].bg_inode_table;

  // Step 5.  Calculate the byte offset of this inode within the
  // block that it lives on.

  // Step 6. Use lseek() and read() to read the inode off of disk
  // and into "inode", given the inodes block number and
  // byte offset within that block that you calculated earlier.
  
  // All done!
  return TRUE;
}


// Given a block offset in a file, calculate indexes that
// you will use to navigate through the indirection blocks
// of the file (if needed) to actually find the block.
//
// Stores the results in the variables passed by reference.
// Stores "-1" if a specific index is not needed.
void calculate_offsets(os_uint32_t blocknum,
                       os_uint32_t blocksize,
                       os_int32_t *direct_num,
                       os_int32_t *indirect_index,
                       os_int32_t *double_index,
                       os_int32_t *triple_index) {
  // Step 1.  There are 12 direct block pointers in an inode.
  // So, if the blocknum is in the following range, you'll use
  // the blocknum as direct_num (i.e., an index into the i_block
  // array in the inode):
  //
  //    block range = [0, 11]   (inclusive)
  // 
  // So, if blocknum <= 11, store the blocknum in "direct_num",
  // and "-1" in all the other indexes.
  if (blocknum <= 11) {
    *direct_num = blocknum;
    *indirect_index = *double_index = *triple_index = -1;
    return;
  }
  os_uint32_t blocks_left = blocknum - 12;

  // Step 2.  Note that a block pointer is 4 bytes long (just the
  // block number on disk).  So, an indirection block can store
  // (blocksize / 4) block pointers.
  //
  // Thus, if the block requested is in the following range
  // (inclusive), you'll use the singly indirect block, pointed 
  // at by entry 12 in the inode's i_block array:
  //
  //   block range = [12, 11 + (blocksize/4)]  (inclusive)
  //
  // See if the requested block is in this range, and if so, calculate
  // it's index in the singly indirect block.  Store this index
  // (i.e., a number between 0 and blocksize/4, inclusive)
  // in "indirect_index", and -1 everywhere else.
  if (blocks_left < blocksize/4) {
    *direct_num = *double_index = *triple_index = -1;
    *indirect_index = blocks_left;
    return;
  }
  blocks_left -= (blocksize/4);

  // Step 3.  If the block is in the following range,
  // you'll use the doubly indirect block pointed to by entry
  // 13 in the inode's i_block array, and the singly indirect
  // block that the appropriate entry in the doubly indirect
  // block points to:
  //
  //  block_range = 
  //     [12 + (blocksize/4),
  //      11 + (blocksize/4) + (blocksize/4)^2]
  //  (inclusive)
  //
  // Calculate the index into the doubly indirection block,
  // and the index into the singly indirect block it points to,
  // and store them in the appropriate arguments.  Store -1 in
  // the other two.
  if (blocks_left < (blocksize/4)*(blocksize/4)) {
    // add some code here
  }
  blocks_left -= (blocksize/4) * (blocksize/4);

  // Step 4.  If the block is in the following range,
  // you'll use the triply indirect block in entry 14
  // of the i_block array.
  //
  //  block_range =
  //    [12 + (blocksize/4) + (blocksize/4)^2
  //     11 + (blocksize/4) + (blocksize/4)^2 + (blocksize/4)^3]
  //  (inclusive).
  //
  // Calculate and store the appropriate indexes, and store -1 in
  // the other argument.
  if (blocks_left < (blocksize/4)*(blocksize/4)*(blocksize/4)) {
    //add some code here
  }

  // should never get here!
  assert(0);
}

// Read block number "blocknum" from the file pointed to by "inode"
// into "buffer".  Returns the total amount of bytes read, or -1 if
// there was an error (e.g., offset is beyond the end of file).
// Note that the last block of the file might not contain a full
// block's worth of data, so the returned byte count might be less
// than a full block.
//
// Don't bother to check if the inode is actually allocated (i.e.,
// don't worry about checking the inode bitmaps) -- just assume this
// is a valid inode.
os_uint32_t file_blockread(struct os_inode_t file_inode, int fd,
                           struct os_fs_metadata_t *metadata,
                           os_uint32_t blocknum, unsigned char *buffer) {
  os_bool_t range_in_hole = FALSE;

  // Step 1.  Verify that "blocknum" is within the range of bytes
  // within the file.  Use file_inode.i_size and metadata->block_size
  // to do this calculation.  If blocknum is out of range, return -1.
  if (blocknum*metadata->block_size >= file_inode.i_size) {
    return -1;
  }
  
  // Step 2.  Use calculate_offsets to calculate the indexes into
  // the inode s_blocks array, the indirection blocks, the double
  // indirection block, and the triple indirection block.
  os_int32_t direct_index, single_index, double_index, triple_index;
  calculate_offsets(blocknum, metadata->block_size, &direct_index,
                    &single_index, &double_index, &triple_index);

  // Step 3.  If triple_index is set, fetch the triple indirection
  // block using lseek() and read().  Use "buffer" as scratch space,
  // so you don't need to malloc anything yourself.  Retrieve the
  // address of the double indirection block from it, using
  // "triple_index" to index into the triple indirection block.
  // If a triple indirection block should be used, but the
  // triple indirection address is 0 (i.e., if file_inode.i_block[14]
  // is zero), then the file has a "hole" in it, with values zero
  // but no actual disk block allocated.  Test for this and if
  // there is a hole, just return zeros.
  os_uint32_t ti_blocknum = file_inode.i_block[14];
  os_uint32_t do_blocknum = 0;
  int res;

  if ((triple_index != -1) && (ti_blocknum == 0)) {
    range_in_hole = TRUE;
  } else if (triple_index != -1) {
    assert(double_index != -1);
    assert(single_index != -1);
    assert(direct_index == -1);

    assert((res = lseek(fd, ti_blocknum*metadata->block_size,
                        SEEK_SET)) == ti_blocknum*metadata->block_size);
    assert((res = read(fd, buffer, metadata->block_size)) ==
           metadata->block_size);
    assert(triple_index < (metadata->block_size / 4));
    do_blocknum = *(((os_uint32_t *) buffer) + triple_index);
  }

  // Step 4.  If double_index is set, fetch the double indirection
  // block using lseek() and read().  Use "buffer" as scratch space,
  // so you don't need to malloc anything yourself.  Retrieve the
  // address of the single indirection block from it.  Note that the
  // address of the double indirection block is either in
  // file_inode.i_block[13], or is something you calculated in step
  // 3.
  os_uint32_t si_blocknum = 0;
  if ((!range_in_hole) && (double_index != -1)) {
    assert(single_index != -1);
    assert(direct_index == -1);
    if (triple_index == -1) {
      do_blocknum = file_inode.i_block[13];
    }

    if (do_blocknum == 0) {
      range_in_hole = TRUE;
    } else {
      assert((res = lseek(fd, do_blocknum*metadata->block_size,
                          SEEK_SET)) == do_blocknum*metadata->block_size);
      assert((res = read(fd, buffer, metadata->block_size)) ==
             metadata->block_size);
      assert(double_index < (metadata->block_size / 4));
      si_blocknum = *(((os_uint32_t *) buffer) + double_index);
    }
  }

  // Step 5.  If single_index is set, fetch the single indirection
  // block using lseek() and read().  Use "buffer" as scratch space,
  // so you don't need to malloc anything yourself.  Retrieve the
  // address of the data block from it.  Note that the address of the
  // single indirection block is either in file_inode.i_block[12], or
  // is something you calculated in step 4.
  os_uint32_t direct_blocknum = 0;
  if ((!range_in_hole) && (single_index != -1)) {
    assert(direct_index == -1);
    if (double_index == -1) {
      si_blocknum = file_inode.i_block[12];
    }

    if (si_blocknum == 0) {
      range_in_hole = TRUE;
    } else {
      assert((res = lseek(fd, si_blocknum*metadata->block_size,
                          SEEK_SET)) == si_blocknum*metadata->block_size);
      assert((res = read(fd, buffer, metadata->block_size)) ==
             metadata->block_size);
      assert(single_index < (metadata->block_size / 4));
      direct_blocknum = *(((os_uint32_t *) buffer) + single_index);
    }
  }

  // Step 6.  Fetch the direct block using lseek() and read(),
  // into "buffer".  Note that the address of the direct block is
  // either in one of the first 12 file_inode.i_block entries, or is
  // something you calculated in step 5.
  if ((!range_in_hole) && (direct_index != -1)) {
    assert(direct_index < 12);
    direct_blocknum = file_inode.i_block[direct_index];
  }
  if (range_in_hole) {
    memset(buffer, 0, metadata->block_size);
  } else {
    assert((res = lseek(fd, direct_blocknum*metadata->block_size,
                        SEEK_SET)) == direct_blocknum*metadata->block_size);
    assert((res = read(fd, buffer, metadata->block_size)) ==
           metadata->block_size);
  }

  // Step 7.  Calculate how much valid data is actually in the
  // block you read.  Subtract (blocknum*metadata->block_size) from
  // file_inode.i_size.  If the difference is >= block_size, you read
  // a full block, otherwise you read the difference.  Return the
  // amount you calculated.
  os_uint32_t diff =
    file_inode.i_size - blocknum * metadata->block_size;
  if (diff >= metadata->block_size)
    return metadata->block_size;
  return diff;
}


// Allocates space for the file associated with file inode number
// "file_inode_num", and reads the contents of the file into the
// allocated space.  Returns the allocated buffer through "buffer",
// and the inode associated with the file through "inode".  Returns
// TRUE on success, FALSE on failure.  If FALSE, no space is
// allocated.  If the file size is 0, returns TRUE, but no space is
// allocated.
os_bool_t file_read(int fd, int file_inode_num,
                    struct os_fs_metadata_t *metadata,
                    struct os_inode_t *inode, unsigned char **buffer) {

  // Step 1.  Convert the file inode number into an inode, using
  // fetch_inode().  [Hint: pass the "inode" argument into fetch_inode, so
  // that fetch_inode() reads the inode into the structure passed to
  // this function by the caller.]

  // Step 2.  We'll only read links, regular files, and directories.
  // The OS is the only thing that knows how to interpret the other
  // file types.  So, test for the acceptable types, and return FALSE
  // if the inode is one of the unacceptable ones.  The acceptable
  // types are defined as bit masks in ext2reader/inc/inode.h.  For
  // example, to test if an inode is for a regular file, verify that
  // inode->i_mode & EXT2_S_IFREG is non-zero.

  // Step 3.  If the file size is 0, return TRUE.

  // Step 4.  Allocate space for the file.  Be sure to verify that
  // malloc worked!  (If not, return FALSE.)  Store the pointer to the
  // malloc'ed space in *buffer to return it to the caller.
  // 
  // We'll round up the amount of memory we allocate to be a multiple
  // of the blocksize, so that file_blockread() can always read a full
  // block from disk even if the file is not an integral number of
  // blocks.

  // we provided the implementation of this step for you. :)
  os_uint32_t malloc_size = inode->i_size;
  if (malloc_size % metadata->block_size != 0) {
    malloc_size +=
      metadata->block_size - (malloc_size % metadata->block_size);
  }
  *buffer = malloc(malloc_size);
  if (*buffer == NULL) {
    printf("malloc failed\n");
    return FALSE;
  }

  // Step 5.  Loop through the blocks of the file, reading in
  // the file's data using file_blockread.  If anything goes wrong,
  // free the allocated buffer and return FALSE.

  // All done! Return true.
  return TRUE;
}

// Given the contents of a directory file (and its length), scan
// through the directory looking for a file whose name is
// "filename".  If such a file is found, return its inode number.
// If not, return 0.
os_uint32_t scan_dir(unsigned char *directory,
                     os_uint32_t directory_length,
                     char *filename) {
  os_uint32_t current_offset = 0;

  if (strlen((char *) filename) == 0)
    return 0;

  // Step 1.  Set up a while loop, looping through each directory
  // entry.  You know when to stop when the offset you're at in the
  // directory file is at or beyond the directory file's length.
  while (current_offset < directory_length) {
      // ok if we're doing this right our current_offset should
      // be pointing at the beginning of a directory entry
      //
      // if so, it should be safe to do this

      struct os_direntry_t *current_entry_p = (struct os_direntry_t *) (directory + current_offset);

      
    // Step 2.  If inode of the current record is 0, then the
    // entry is invalid (e.g., that file was deleted from the
    // directory).  If so, skip to the next iteration of the while
    // loop (using "continue;") after properly incrementing
    // current_offset (use rec_len).

    // Step 3.  Compare the name in the directory entry to
    // "filename".  Note that you cannot rely on the name in
    // the directory entry to be NULL terminated, so you can't
    // use strcmp().  If the name matches, you're done -- return its
    // inode number.

    // Step 4.  If you didn't find it, add the appropriate amount to
    // current_offset, and fall into the next iteration of the while
    // loop.
  }

  // Step N:  fell off the end of the directory file, and didn't find
  // the entry we wanted, so return 0 to indicate failure.
  return 0;
}


// Given an absolute path to a file (e.g. /foo/bar/baz.txt - note
// initial '/') returns the inode number of the file, or 0 if the
// file can't be found.
os_uint32_t path_to_inode_num(char* path, int fd, struct os_fs_metadata_t *metadata) {

  // you always want to start your search at the root inode
  // it will be the first directory you do a file_read on
  os_uint32_t root = EXT2_ROOT_INO;

  // In my solution I wanted to edit the given path to replace '/'
  // characters with /0 as I go (hint use strchr).  But I can't do
  // this with the actual parameter - instead I have to copy the
  // parameter string to a new string that I can edit locally
  char path_copy[strlen(path) + 1];
  strncpy(path_copy, path, strlen(path) + 1);
  char* path_remaining = path_copy;

  // file_read mallocs the data it finds, but we still need a place to
  // put the output pointer
  unsigned char* buffer;

  // the inode is the second output parameter of file_read.  Use it to
  // figure out the size of the file that was read
  struct os_inode_t inode;
  

  // for me it's handy to omit the initial "/" from the path, because
  // we are already starting at the root
  path_remaining++;

  //for each / seperated entry in the path
  //1. Use file_read to read the current directory data
  //2. Use scan_dir to search for the entry you're looking for
  //3. That will become the directory you'll use in the next iteration
  return 0;
  
}
