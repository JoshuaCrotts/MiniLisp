#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <editline/readline.h>

extern void parse(char *);
extern void parse_setup(void);
extern void parse_cleanup(void);

#endif // PARSE_H