// Copyright 2009 Steve Gribble (gribble [at] cs.washington.edu).
// May 22nd, 2009.
//
// This file contains the code we use to grade your assignment.
// The code is strategically invoked in various places in your
// project, and tests things you produce against their correct
// values.  You can use these tests to help you debug your
// code, and also to predict your grade on this assignment. :)

#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "inc/types.h"
#include "inc/blockgroup_descriptor.h"
#include "inc/directoryentry.h"
#include "inc/ext2access.h"
#include "inc/inode.h"
#include "inc/superblock.h"

static os_uint32_t final_grade = 0;
static os_uint32_t final_grades_available = 0;

void test_uint32_equal(os_uint32_t a, os_uint32_t expected,
                       char *testname, os_uint32_t points);
void test_int32_equal(os_int32_t a, os_int32_t expected,
                       char *testname, os_uint32_t points);
void test_string_equal(char *a, char *expected,
                       char *testname, os_uint32_t points);
#define test_uint16_equal test_uint32_equal
#define test_uint8_equal  test_uint32_equal
#define test_int16_equal test_int32_equal
#define test_int8_equal  test_int32_equal

void check_superblock(struct os_superblock_t *sb) {
  printf("superblock tests...\n");
  // make sure the superblock has the right magic num
  test_uint32_equal(sb->s_magic, EXT2_SUPER_MAGIC,
                    "sb->s_magic", 1);
  // make sure the volume name is right
  test_string_equal((char *) sb->s_volume_name, "451_filesystem", 
                    "sb->s_volume_name", 1);
  // make sure the number of blocks in filesystem is
  // what we expect
  test_uint32_equal(sb->s_blocks_count, 52000,
                    "sb->s_blocks_count", 1);
  // make sure the first data block is what we expect
  test_uint32_equal(sb->s_first_data_block, 1,
                    "sb->s_first_data_block", 1);
  // make sure there are no features
  test_uint32_equal(sb->s_feature_compat, 0,
                    "sb->s_feature_compat", 1);
}

void check_metadata(struct os_fs_metadata_t *fsm) {
  // check that all the fields in the metadata structure
  // are what we expect.
  printf("\nmetadata tests...\n");

  // just a few tests to verify the metadata is there
  test_uint32_equal(fsm->disk_size, 53248000, "fsm->disk_size", 1);
  test_uint32_equal(fsm->block_size, 1024, "fsm->block_size", 1);
  test_uint32_equal(fsm->num_blocks, 52000, "fsm->num_blocks", 1);

  printf("\nblockgroup descriptor table tests...\n");

  os_uint32_t num_descriptors_in_table = fsm->num_blockgroups;

  // check the first and the last
  test_uint16_equal(fsm->bgdt[0].bg_free_blocks_count, 6288,
                    "bgdt[0].bg_free_blocks_count", 1);

  test_uint16_equal(fsm->bgdt[num_descriptors_in_table-1].bg_free_blocks_count, 1973,
                    "bgdt[num_descriptors_in_table-1].bg_free_blocks_count", 1);

  
}

void check_fetch_inode(int fd,
                       struct os_fs_metadata_t *metadata) {
  printf("\nfetch_inode() tests...\n");
  struct os_inode_t inode;
  os_bool_t res;
  // test the root directory inode
  res = fetch_inode(EXT2_ROOT_INO, fd, metadata, &inode);
  test_uint8_equal(res, TRUE, "fetch_inode(EXT2_ROOT_INO, ..)", 1);
  // 0x41ED == EXT2_S_IFDIR | EXT2_S_IRUSR | EXT2_S_IWUSR |
  //           EXT2_S_IXUSR | EXT2_S_IRGRP | EXT2_S_IXGRP |
  //           EXT2_S_IROTH | EXT2_S_IXOTH;
  test_uint16_equal(inode.i_mode, 0x41ED, "inode.i_mode", 1);
  test_uint16_equal(inode.i_uid, 0, "inode.i_uid", 1);
  test_uint32_equal(inode.i_size, 1024, "inode.i_size", 1);
  // 124309108 == Sat May 23 08:55:08 PDT 2009, according to
  // the unix command:
  //     date -j -f "%s" 1243094108
  test_uint32_equal(inode.i_ctime, 1243094108, "inode.i_ctime", 1);
  test_uint32_equal(inode.i_blocks, 2, "inode.i_blocks", 1);
  test_uint32_equal(inode.i_block[0], 238, "inode.i_block[0]", 1);
  test_uint32_equal(inode.i_faddr, 0, "inode.i_faddr", 1);

  // test inode 1867, which turns out to be the file
  // 'fancy_stuff/symlink_to_1-intro.pdf'.
  res = fetch_inode(1867, fd, metadata, &inode);
  test_uint8_equal(res, TRUE, "fetch_inode(1867, ..)", 1);
  // 0xA1FF == EXT2_S_IFLNK | EXT2_S_IRUSR | EXT2_S_IWUSR |
  //           EXT2_S_IXUSR | EXT2_S_IRGRP | EXT2_S_IWGRP |
  //           EXT2_S_IXGRP | EXT2_S_IROTH | EXT2_S_IWOTH |
  //           EXT2_S_IXOTH;
  test_uint16_equal(inode.i_mode, 0xA1FF, "inode.i_mode", 1);
  test_uint16_equal(inode.i_uid, 1000, "inode.i_uid", 1);
  test_uint32_equal(inode.i_size, 27, "inode.i_size", 1);
  // 1243094088 == Sat May 23 08:54:48 PDT 2009, according to
  // the unix command:
  //     date -j -f "%s" 1243094088
  test_uint32_equal(inode.i_ctime, 1243094088, "inode.i_ctime", 1);
  test_uint32_equal(inode.i_blocks, 0, "inode.i_blocks", 1);
  test_uint32_equal(inode.i_faddr, 0, "inode.i_faddr", 1);

  // test inode 13048 -- should return true, since is last inode
  res = fetch_inode(13048, fd, metadata, &inode);
  test_uint8_equal(res, TRUE, "fetch_inode(13048, ..)", 1);

  // test inode 13049 -- should return false, since beyond end of last
  // inode of last blockgroup
  res = fetch_inode(13049, fd, metadata, &inode);
  test_uint8_equal(res, FALSE, "fetch_inode(13049, ..)", 1);
}

void check_calculate_offsets() {
  printf("\ncalculate_offsets() tests...\n");
  os_int32_t direct, single, doubly, triply;

  printf(" checking offsets for block 2, blocksize 1024\n");
  calculate_offsets(2, 1024, &direct, &single, &doubly, &triply);
  test_int32_equal(direct, 2, "direct", 1);
  test_int32_equal(single, -1, "singly indirect index", 1);
  test_int32_equal(doubly, -1, "doubly indirect index", 1);
  test_int32_equal(triply, -1, "triply indirect index", 1);

  printf(" checking offsets for block 12, blocksize 1024\n");
  calculate_offsets(12, 1024, &direct, &single, &doubly, &triply);
  test_int32_equal(direct, -1, "direct", 1);
  test_int32_equal(single, 0, "singly indirect index", 1);
  test_int32_equal(doubly, -1, "doubly indirect index", 1);
  test_int32_equal(triply, -1, "triply indirect index", 1);

  printf(" checking offsets for block 100, blocksize 1024\n");
  calculate_offsets(100, 1024, &direct, &single, &doubly, &triply);
  test_int32_equal(direct, -1, "direct", 1);
  test_int32_equal(single, 88, "singly indirect index", 1);
  test_int32_equal(doubly, -1, "doubly indirect index", 1);
  test_int32_equal(triply, -1, "triply indirect index", 1);

  printf(" checking offsets for block 267, blocksize 1024\n");
  calculate_offsets(267, 1024, &direct, &single, &doubly, &triply);
  test_int32_equal(direct, -1, "direct", 1);
  test_int32_equal(single, 255, "singly indirect index", 1);
  test_int32_equal(doubly, -1, "doubly indirect index", 1);
  test_int32_equal(triply, -1, "triply indirect index", 1);

  printf(" checking offsets for block 268, blocksize 1024\n");
  calculate_offsets(268, 1024, &direct, &single, &doubly, &triply);
  test_int32_equal(direct, -1, "direct", 1);
  test_int32_equal(single, 0, "singly indirect index", 1);
  test_int32_equal(doubly, 0, "doubly indirect index", 1);
  test_int32_equal(triply, -1, "triply indirect index", 1);

  printf(" checking offsets for block 65803, blocksize 1024\n");
  calculate_offsets(65803, 1024, &direct, &single, &doubly, &triply);
  test_int32_equal(direct, -1, "direct", 1);
  test_int32_equal(single, 255, "singly indirect index", 1);
  test_int32_equal(doubly, 255, "doubly indirect index", 1);
  test_int32_equal(triply, -1, "triply indirect index", 1);

  // this prevents a bug that can cause problems late in your tests
  printf(" checking offsets for block 1643, blocksize 1024\n");
  calculate_offsets(1643, 1024, &direct, &single, &doubly, &triply);
  test_int32_equal(direct, -1, "direct", 0);
  test_int32_equal(single, 95, "singly indirect index", 1);
  test_int32_equal(doubly, 5, "doubly indirect index", 1);
  test_int32_equal(triply, -1, "triply indirect index", 0);

  
  printf(" checking offsets for block 65804, blocksize 1024\n");
  calculate_offsets(65804, 1024, &direct, &single, &doubly, &triply);
  test_int32_equal(direct, -1, "direct", 1);
  test_int32_equal(single, 0, "singly indirect index", 1);
  test_int32_equal(doubly, 0, "doubly indirect index", 1);
  test_int32_equal(triply, 0, "triply indirect index", 1);

  printf(" checking offsets for block 12121212, blocksize 1024\n");
  calculate_offsets(12121212, 1024, &direct, &single, &doubly, &triply);
  test_int32_equal(direct, -1, "direct", 1);
  test_int32_equal(single, 112, "singly indirect index", 1);
  test_int32_equal(doubly, 243, "doubly indirect index", 1);
  test_int32_equal(triply, 183, "triply indirect index", 1);

  printf(" checking offsets for block 16843019, blocksize 1024\n");
  calculate_offsets(16843019, 1024, &direct, &single, &doubly, &triply);
  test_int32_equal(direct, -1, "direct", 1);
  test_int32_equal(single, 255, "singly indirect index", 1);
  test_int32_equal(doubly, 255, "doubly indirect index", 1);
  test_int32_equal(triply, 255, "triply indirect index", 1);



}

void check_file_blockread(int fd, struct os_fs_metadata_t *metadata) {
  printf("\nfile_blockread() tests...\n");
  struct os_inode_t inode;
  unsigned char buffer[1024];
  os_uint32_t res;

  if (!(fetch_inode(15, fd, metadata, &inode))) {
    printf("  *** fetch_inode() failed in check_file_blockread, "
           "so cannot proceed with tests.\n");
    exit(-1);
  }
  res = file_blockread(inode, fd, metadata, 0, buffer);

  // only one test because this was implmented for you
  
  test_int32_equal(res, 61, "file_blockread(inode 15, block 0)", 1);
  /* test_uint8_equal(buffer[0], 'T', "buffer[0]", 1); */
  /* test_uint8_equal(buffer[4], 'n', "buffer[4]", 1); */
  /* test_uint8_equal(buffer[60], '\n', "buffer[60]", 1); */
  /* res = file_blockread(inode, fd, metadata, 1, buffer); */
  /* test_int32_equal(res, -1, "file_blockread(inode 15, block 1)", 1); */

  /* if (!(fetch_inode(14, fd, metadata, &inode))) { */
  /*   printf("  *** fetch_inode() failed, so cannot proceed with tests.\n"); */
  /*   exit(-1); */
  /* } */
  /* res = file_blockread(inode, fd, metadata, 0, buffer); */
  /* test_int32_equal(res, 1024, "file_blockread(inode 14, block 0)", 2); */
  /* test_uint32_equal(buffer[0], 'L', "buffer[0]", 1); */
  /* test_uint32_equal(buffer[1023], 's', "buffer[1023]", 1); */
  /* res = file_blockread(inode, fd, metadata, 1500, buffer); */
  /* test_int32_equal(res, 1024, "file_blockread(inode 14, block 1500)", 2); */
  /* test_uint32_equal(buffer[0], 'R', "buffer[0]", 1); */
  /* test_uint32_equal(buffer[1023], 'g', "buffer[1023]", 1); */
  /* res = file_blockread(inode, fd, metadata, 1643, buffer); */
  /* test_int32_equal(res, 564, "file_blockread(inode 14, block 1643)", 2); */
  /* test_uint32_equal(buffer[0], 'u', "buffer[0]", 1); */
  /* test_uint32_equal(buffer[563], '\n', "buffer[563]", 1); */
  /* res = file_blockread(inode, fd, metadata, 1644, buffer); */
  /* test_int32_equal(res, -1, "file_blockread(inode 14, block 1643)", 2); */
}

void check_file_read(int fd, struct os_fs_metadata_t *metadata) {
  printf("\nfile_read() tests...\n");  

  struct os_inode_t inode;
  unsigned char *buffer;
  os_bool_t res;
  res = file_read(fd, 15, metadata, &inode, &buffer);
  test_uint32_equal(res, TRUE, "file_read(..,15,..)", 1);
  test_uint32_equal((buffer != NULL), 1, "(buffer != NULL)", 1);
  test_uint32_equal(inode.i_size, 61, "inode->i_size", 1);
  test_uint8_equal(buffer[0], 'T', "buffer[0]", 1);
  test_uint8_equal(buffer[59], '.', "buffer[59]", 1);
  test_uint8_equal(buffer[60], '\n', "buffer[60]", 1);
  free(buffer);
  buffer = NULL;

  res = file_read(fd, 14, metadata, &inode, &buffer);
  test_uint32_equal(res, TRUE, "file_read(..,14,..)", 1);
  test_uint32_equal((buffer != NULL), 1, "(buffer != NULL)", 1);
  test_uint32_equal(inode.i_size, 1682996, "inode->i_size", 1);
  test_uint8_equal(buffer[0], 'L', "buffer[0]", 1);
  test_uint8_equal(buffer[100000], ' ', "buffer[100000]", 1);
  test_uint8_equal(buffer[1682995], '\n', "buffer[1682995]", 1);
  free(buffer);
}

void check_scan_dir(int fd, struct os_fs_metadata_t *metadata) {
  printf("\nscan_dir() tests...\n");

  struct os_inode_t inode;
  unsigned char *buffer;
  os_bool_t res;

  // read in the root directory (inode 2)
  res = file_read(fd, 2, metadata, &inode, &buffer);
  if (res == FALSE) {
    printf("  *** fetch_inode() failed in check_scan_dir, "
           "so cannot proceed with tests.\n");
    exit(-1);
  }
  
  os_uint32_t inodenum = scan_dir(buffer, inode.i_size, ".");
  test_uint32_equal(inodenum, 2, "inode of '.' in /", 1);
  inodenum = scan_dir(buffer, inode.i_size, "451_lectures");
  test_uint32_equal(inodenum, 3729, "inode of '451_lectures' in /", 1);
  inodenum = scan_dir(buffer, inode.i_size, "lorem-ipsum.txt");
  test_uint32_equal(inodenum, 14, "inode of 'lorem-ipsum.txt' in /", 1);
  inodenum = scan_dir(buffer, inode.i_size, "asdlfkjalsdf");
  test_uint32_equal(inodenum, 0, "inode of 'asdlfkjalsdf' in /", 1);
  free(buffer);

  // read in the "big_directory" directory (inode 1869)
  res = file_read(fd, 1869, metadata, &inode, &buffer);
  if (res == FALSE) {
    printf("  *** fetch_inode() failed in check_scan_dir, "
           "so cannot proceed with tests.\n");
    exit(-1);
  }
  
  inodenum = scan_dir(buffer, inode.i_size, "..");
  test_uint32_equal(inodenum, 2,
                    "inode of '..' in /big_directory/", 1);
  inodenum = scan_dir(buffer, inode.i_size, ".");
  test_uint32_equal(inodenum, 1869,
                    "inode of '.' in /big_directory/", 1);
  inodenum = scan_dir(buffer, inode.i_size, "eddd211");
  test_uint32_equal(inodenum, 2146,
                    "inode of 'eddd211' in /big_directory/", 1);
  inodenum = scan_dir(buffer, inode.i_size, "asdlfkjalsdf");
  test_uint32_equal(inodenum, 0,
                    "inode of 'asdlfkjalsdf' in /", 1);
  free(buffer);
}

void check_path_to_inode_num(int fd, struct os_fs_metadata_t *metadata) {
  printf("\npath_to_inode_num tests...\n");

  os_bool_t res;

  res = path_to_inode_num("/small-file.txt", fd, metadata);
  test_uint32_equal(res, 15, "small-file.txt inode num", 4);
  res = path_to_inode_num("/451_lectures/7-sync.pdf", fd, metadata);
  test_uint32_equal(res, 3742, "7-sync inode num", 4);
  res = path_to_inode_num("/giant_directory_tree/2/2.1/2.1.0/some_file.txt", fd, metadata);
  test_uint32_equal(res, 6474, "some_file inode num", 4);
}


void print_grade(void) {
  printf("\nFinal grade: %d / %d\n", final_grade,
         final_grades_available);
}


void test_uint32_equal(os_uint32_t a, os_uint32_t expected,
                       char *testname, os_uint32_t points) {
  final_grades_available += points;
  printf(" - '%s' == %lu  ", testname, (unsigned long int) expected);
  if (a == expected) {
    printf("OK [+%d pt%s]\n", points, points == 1 ? "" : "s");
    final_grade += points;
    return;
  }
  printf("failed! (was %d)\n", a);
}

void test_int32_equal(os_int32_t a, os_int32_t expected,
                      char *testname, os_uint32_t points) {
  final_grades_available += points;
  printf(" - '%s' == %ld  ", testname, (long int) expected);
  if (a == expected) {
    printf("OK [+%d pt%s]\n", points, points == 1 ? "" : "s");
    final_grade += points;
    return;
  }
  printf("failed! (was %d)\n", a);
}


void test_string_equal(char *a, char *expected,
                       char *testname, os_uint32_t points) {
  final_grades_available += points;
  printf(" - '%s' == \"%s\"  ", testname, expected);
  if (strcmp(a, expected) == 0) {
    printf("OK [+%d pnt%s]\n", points, points == 1 ? "" : "s");
    final_grade += points;
    return;
  }
  printf("failed! (was \"%s\")\n", a);
}

// Usage:  ext2reader diskfile_name
int main(int argc, char **argv) {
  // open up the disk file
  if (argc != 2) {
    printf("usage:  ext2reader diskfile_name\n");
    return -1;
  }
  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    printf("couldn't open file \"%s\"\n", argv[1]);
    return -1;
  }

  // calculate filesystem metadata.  you get to implement this
  // function in file ext2_access.c.
  struct os_fs_metadata_t metadata;
  memset(&metadata, 0, sizeof (struct os_fs_metadata_t));
  
  load_ext2_metadata(fd, &metadata);
  check_metadata(&metadata);  // our test code
  
  // check that your inode retrieval function works
  check_fetch_inode(fd, &metadata);

  // check that your indirection index calculation function works
  check_calculate_offsets();

  // check that file_blockread() works correctly
  check_file_blockread(fd, &metadata);

  // check that file_read() works correctly
  check_file_read(fd, &metadata);

  // check that scan_dir() works correctly
  check_scan_dir(fd, &metadata);

  // check that path_read() works correctly
  check_path_to_inode_num(fd, &metadata);

  close(fd);
  print_grade();

  free(metadata.bgdt);
  return 0;
}
