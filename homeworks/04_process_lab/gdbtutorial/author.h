#include <stdio.h>
#include <string.h>

struct author {
  char name[30];
  int book_count;
  char book_name[5][70];  // 5 books, 70 character names
};

typedef struct author Author;

void display_author_details(Author guy);
