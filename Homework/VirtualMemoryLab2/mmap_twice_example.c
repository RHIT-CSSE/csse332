#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define UNIVERSAL_REGION 0xf9f8c000


/*

  This is designed to give you an example of how multiple mmaps work,
  in a way that's a little bit similar to how things will be used in
  the actual lab

 */

// how many different regions to do want?
// note because we can only mmap at page boundaries, these will be of getpagesize() in size
#define MY_PAGES 2

int main(int argc, char** argv)
{

    // STEP 1: Let's map a big region of memory
    int fd = open("delete_me.dat", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    if(fd < 0) {
        perror("error loading linked file");
        exit(25);
    }
    // STEP 2: Ensure the file is big enough for all our data
    char data = '\0';
    lseek(fd, getpagesize()*MY_PAGES, SEEK_SET);
    write(fd, &data, 1); 
    lseek(fd, 0, SEEK_SET);

    // STEP 3, mmap at some location chosen by mmap
    char* frames = mmap(NULL,
            getpagesize()*MY_PAGES,
            PROT_READ | PROT_WRITE | PROT_EXEC,
            MAP_SHARED,
            fd,
            0);

    if(frames == NULL) {
        perror("first mmap failed");
        exit(1);
    }
    // STEP 4, fill these pages with some data
    // we won't do this in the lab except indirecty through forth
    // but I want to make it obvious these pages are different

    // The first page will be filled with As, the second with Bs, etc.
    for(int i = 0; i < getpagesize()*MY_PAGES; i++) {
        int current_page = i / getpagesize();
        char output_char = 'A' + current_page;
        frames[i] = output_char;
    }

    // STEP 5, ok lets map the first page into the special region

    // edit this num to try other pages
    // just don't make it bigger than MY_PAGES
    int page_to_map = 1;
    
    void* result = mmap((void*) UNIVERSAL_REGION,
                        getpagesize(),
                        PROT_READ | PROT_WRITE | PROT_EXEC,
                        MAP_SHARED,
                        fd,
                        page_to_map*getpagesize());

    if(result != (void*) UNIVERSAL_REGION) {
        perror("second mmap failed");
        exit(1);
    }
    
    //lets output a few characters from the page

    char* uni_region = (void*) UNIVERSAL_REGION;
    printf("data in universal region: %c%c%c%c\n", uni_region[0], uni_region[1], uni_region[2], uni_region[3]);

    char* page_in_frames_array = &frames[page_to_map*getpagesize()];

    // edits work too.  Let's edit the first character of the page in frames array
    page_in_frames_array[0] = '?';
    // Let's edit the universal region and makes sure it propigates to the frames array
    uni_region[1] = '!';
    
    printf("data in universal region (after edit): %c%c%c%c\n", uni_region[0], uni_region[1], uni_region[2], uni_region[3]);
    printf("data in frames array (after edit): %c%c%c%c\n", page_in_frames_array[0], page_in_frames_array[1], page_in_frames_array[2], page_in_frames_array[3]);

    //now lets clean up
    munmap((void*) UNIVERSAL_REGION, getpagesize());
    munmap(frames, getpagesize()*MY_PAGES);
    close(fd);

    return 0;
}

