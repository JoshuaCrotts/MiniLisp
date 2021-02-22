#include "../lib/include/mpc.h"

/* Parse method called by the main method. */
long eval(mpc_ast_t *t);
long eval_op(long value1, char *op, long value2);
void parse(char *input);
void parse_cleanup(void);
void parse_setup(void);

static mpc_parser_t *number_t;
static mpc_parser_t *operator_t;
static mpc_parser_t *expression_t;
static mpc_parser_t *minilisp_t;

/**
 * 
 */
void 
parse(char *input) {
  mpc_result_t r;
  if (mpc_parse("<stdin>", input, minilisp_t, &r)) {
    // If we are successful then print out the AST.
    printf("%ld\n", eval(r.output));
    mpc_ast_delete(r.output);
  } else {
    // Otherwise, print the error.
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
  }
}

/**
 *
 * 
 * @param mpc_ast_t
 * 
 * @return long
 */
long 
eval(mpc_ast_t *t) {
  // If this is a number, just return the number itself.
  if (strstr(t->tag, "number")) {
    return atoi(t->contents);
  }

  // The operator is always the second child.
  char *op = t->children[1]->contents;

  // Store the third child in x.
  long x = eval(t->children[2]);

  // Recursively evaluate the remaining children.
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    ++i;
  }

  return x;
}

/**
 * 
 */
long 
eval_op(long value1, char *op, long value2) {
  if(0 == strcmp("+",  op)) return value1 + value2;
  if(0 == strcmp("-",  op)) return value1 - value2;
  if(0 == strcmp("*",  op)) return value1 * value2;
  if(0 == strcmp("/",  op)) return value1 / value2;
  if(0 == strcmp("%",  op)) return value1 % value2;

  return 0;
}

/**
 * 
 */
void 
parse_setup(void) {
  number_t   = mpc_new("number");
  operator_t = mpc_new("operator");
  expression_t = mpc_new("expr");
  minilisp_t = mpc_new("minilisp"); 

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                   \
      number  : /-?[0-9]+/                            ; \
      operator: '+' | '-' | '*' | '/'                 ; \
      expr    : <number> | '(' <operator> <expr>+ ')' ; \
      minilisp: /^/ <operator> <expr>+ /$/            ; \
    ",
    number_t, operator_t, expression_t, minilisp_t);
}

/**
 * 
 */
void 
parse_cleanup(void) {
  // Undefine and remove the parsers.
  mpc_cleanup(4, number_t, operator_t, expression_t, minilisp_t);
}