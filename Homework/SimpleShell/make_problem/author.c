#include <stdio.h>
#include <string.h>
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

char* get_longest_book_title(Author person) {
  static char result[70];
  int bestLength = 0;
  int currentLength = 0;
  int i;
  for (i=0; i < person.book_count; i++) {
    currentLength = strlen(person.book_name[i]);
    if (currentLength > bestLength) {
      bestLength = currentLength;
      strncpy(result, person.book_name[i], 70);
    }
  }
  return result;
}
