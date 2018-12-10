#include "./author.h"

void
display_author_details(Author person) {
  int i = 0;

  printf("Author's name: %s\n", person.name);
  printf("Number of published books: %d \n", person.book_count);

  for (i=0; i < person.book_count; i++) {
    printf("%i. %s\n", i+1, person.book_name[i]);
  }
}
