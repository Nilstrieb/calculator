#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    LITERAL,
    ADD,
    SUB,
    MUL,
    DIV,
} ExprKind;

struct Expr;

typedef struct {
    struct Expr *lhs;
    struct Expr *rhs;
} BinOp;

typedef struct {
    ExprKind tag;
    union {
        int64_t literal;
        BinOp op;
    } value;
} Expr;

bool is_numeric(const char character) {
    return character >= '0' && character <= '9';
}

void parse_expr(char *input, size_t *offset) {
}

void parse_factor(char *input, size_t *offset) {
}

void parse_term(char *input, size_t *offset) {
}

Expr parse_literal(char *input, size_t *offset) {
    // the index of the first character of the literal
    const size_t start = *offset;

    // the index of the character past the last character of the literal
    size_t end = start;

    while (input[*offset] != '\0' && is_numeric(input[*offset])) {
        (*offset)++;
        end = *offset;
    }

    size_t literal_len = end - start;

    if (literal_len == 0) {
        fprintf(stderr, "Found end of file expecting number");
        exit(1);
    }

    // because C standard library functions are utterly deranged and want null terminated strings, we have to hack a little
    // just set the next character to null to get a nice null terminated string
    char old_next_char = input[literal_len];

    int64_t value = strtoll(input + start, NULL, 10);

    // and then don't forget to set it back :D
    input[literal_len] = old_next_char;

    Expr expr;
    expr.tag = LITERAL;
    expr.value.literal = value;

    return expr;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("provide me an argument please\n");
        return 1;
    }

    char *input = argv[1];

    size_t offset = 0;

    Expr expr = parse_literal(input, &offset);

    printf("%ld\n", expr.value.literal);

    return 0;
}
