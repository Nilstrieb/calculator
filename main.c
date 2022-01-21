#include <stdio.h>
#include <stdint.h>

typedef enum {
    LITERAL
    ADD
    SUB
    MUL
    DIV
    GROUP
} expr_kind;

typedef struct {
    expr_kind tag;
    union {

    } value;
} expr;

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("provide me an argument please\n");
        return 1;
    }

    char *input = argv[0];

    return 0;
}

int parse_expr(char *input, size_t *offset) {}

int parse_factor(char *input, size_t *offset) {}

int parse_term(char *input, size_t *offset) {}

int parse_literal(char *input, size_t *offset) {}
