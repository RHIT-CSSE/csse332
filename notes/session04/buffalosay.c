#include <stdio.h>

// credit to the classic unix program cowsay
// https://en.wikipedia.org/wiki/Cowsay
//
// and also to mic & dwb who I guess created this ascii art to begin
// with (https://asciiart.website/index.php?art=animals/bison)

int main(int argc, char** argv) {
  if(argc < 2) {
    printf("Not enough arguments!\n");
  } else {
    printf(
"             _.-````'-,_\n"
"   _,.,_ ,-'`           `'-.,_\n"
" /)     (\\                   '``-.\n"
"((      ) )                      `\\\n"
" \\)    (_/                        )\\\n"
"  |       /)           '    ,'    / \\\n"
"  `\\    ^'            '     (    /  ))\n"
"    |      _/\\ ,     /    ,,`\\   (  \"`\n"
"     \\Y,   |  \\  \\  | ````| / \\_ \\\n"
"    /  `)_/    \\  \\  )    ( >  ( >\n"
"   |            \\( \\(     |/   |/\n"
"   |           /_(/_(    /_(  /_(\n"
"   | \n"
"   < %s > \n\n", argv[1]);
  }
  return 0;
}
