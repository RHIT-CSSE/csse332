// Copyright 2009 Steve Gribble (gribble [at] cs.washington.edu).
// May 22nd, 2009.
//
// This is the "main" file for project 3 light.  We've written
// all of the code for you in this file; you don't need to make
// any changes here.  The code in this file invokes functions that
// you'll implement in extaccess.c, as well as the test code
// we've provided in testcode.c.
//
// It's worth reading through this file to understand what's happening:
// it's pretty straightforward.

#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "inc/types.h"
#include "inc/blockgroup_descriptor.h"
#include "inc/directoryentry.h"
#include "inc/inode.h"
#include "inc/superblock.h"
#include "inc/ext2access.h"
#include "inc/testcode.h"


// Usage:  filereader diskfile_name path_to_read
int main(int argc, char **argv) {
  // open up the disk file
  if (argc != 3) {
    printf("usage:  ext2reader diskfile_name path_to_read\n");
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
  load_ext2_metadata(fd, &metadata);

  char* path_to_read = argv[2];
  os_uint32_t inode_num = path_to_inode_num(path_to_read, fd, &metadata);
  if(inode_num == 0) {
    printf("Error - couldn't find file inode");
    exit(1);
  }
  struct os_inode_t inode;
  unsigned char* data_buffer;
  
  printf("file at inode %d\n", inode_num);
  if(!file_read(fd, inode_num, &metadata, &inode, &data_buffer)) {
    printf("Error - couldn't read file!");
    exit(1);
  }

  close(fd);
  char* output_filename = strrchr(path_to_read, '/');
  if(output_filename == NULL) {
    output_filename = path_to_read;
  } else {
    output_filename++;
  }
  printf("outputing file named %s...\n", output_filename);
  fd = open(output_filename, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR);
  if(fd == -1) {
    perror("error opening file for output");
    exit(1);
  }
  if(write(fd, data_buffer, inode.i_size) == -1) {
    perror("Error writing output file\n");
    exit(1);
  }
  close(fd);
  free(data_buffer);
  free(metadata.bgdt);
  return 0;
}
