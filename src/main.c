#include "../include/parse.h"

int 
main(int argc, char *argv[]) {

  // Print version and exit information.
  printf("MiniLisp Version 0.0.1\n");
  printf("Press Ctrl+c to Exit\n");

  while(true) {
    // Outputs the prompt and then reads it back in.
    char *input = readline("MiniLisp> ");

    // Adds the line to our current history.
    add_history(input);

    parse(input);

    free(input);
  }

  return 0;
}