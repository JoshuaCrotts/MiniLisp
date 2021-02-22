#include "../lib/include/mpc.h"

/* Parse method called by the main method. */
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
    mpc_ast_print(r.output);
    mpc_ast_delete(r.output);
  } else {
    // Otherwise, print the error.
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
  }
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