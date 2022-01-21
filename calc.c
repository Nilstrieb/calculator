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

typedef struct BinOp BinOp;
typedef struct Expr Expr;

struct BinOp {
    Expr *lhs;
    Expr *rhs;
};

struct Expr {
    ExprKind tag;
    union {
        int64_t literal;
        BinOp op;
    } value;
};

bool is_numeric(const char character) {
    return character >= '0' && character <= '9';
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

    const size_t literal_len = end - start;

    if (literal_len == 0) {
        fprintf(stderr, "Invalid number literal\n");
        exit(1);
    }

    // because C standard library functions are utterly deranged and want null terminated strings, we have to hack a little
    // just set the next character to null to get a nice null terminated string
    const char old_next_char = input[literal_len];

    const int64_t value = strtoll(input + start, NULL, 10);

    // and then don't forget to set it back :D
    input[literal_len] = old_next_char;

    Expr expr;
    expr.tag = LITERAL;
    expr.value.literal = value;

    return expr;
}

Expr parse_factor(char *input, size_t *offset) {
    const Expr lhs_expr = parse_literal(input, offset);

    const char op = input[*offset];

    ExprKind kind;

    switch (op) {
        case '\0':
            return lhs_expr;
        case '*':
            kind = MUL;
            break;
        case '/':
            kind = DIV;
            break;
        default:
            return lhs_expr;
    }

    (*offset)++;

    const Expr rhs_expr = parse_factor(input, offset);

    Expr *lhs = malloc(sizeof(Expr));
    *lhs = lhs_expr;

    Expr *rhs = malloc(sizeof(Expr));
    *rhs = rhs_expr;

    Expr result;
    result.tag = kind;

    BinOp bin_op;
    bin_op.lhs = lhs;
    bin_op.rhs = rhs;

    result.value.op = bin_op;

    return result;
}

Expr parse_term(char *input, size_t *offset) {
    const Expr lhs_expr = parse_factor(input, offset);

    const char op = input[*offset];
    (*offset)++;

    ExprKind kind;

    switch (op) {
        case '\0':
            return lhs_expr;
        case '+':
            kind = ADD;
            break;
        case '-':
            kind = SUB;
            break;
        default:
            fprintf(stderr, "Invalid operator: %c\n", op);
            exit(1);
    }

    const Expr rhs_expr = parse_term(input, offset);

    Expr *lhs = malloc(sizeof(Expr));
    *lhs = lhs_expr;

    Expr *rhs = malloc(sizeof(Expr));
    *rhs = rhs_expr;

    Expr result;
    result.tag = kind;

    BinOp bin_op;
    bin_op.lhs = lhs;
    bin_op.rhs = rhs;

    result.value.op = bin_op;

    return result;
}

Expr parse_expr(char *input, size_t *offset) {
    return parse_term(input, offset);
}

int64_t eval_expr(const Expr expr) {
    switch (expr.tag) {
        case LITERAL:
            return expr.value.literal;
        case ADD: {
            const int64_t lhs = eval_expr(*expr.value.op.lhs);
            const int64_t rhs = eval_expr(*expr.value.op.rhs);
            return lhs + rhs;
        }
        case SUB: {
            const int64_t lhs = eval_expr(*expr.value.op.lhs);
            const int64_t rhs = eval_expr(*expr.value.op.rhs);
            return lhs - rhs;
        }
        case MUL: {
            const int64_t lhs = eval_expr(*expr.value.op.lhs);
            const int64_t rhs = eval_expr(*expr.value.op.rhs);
            return lhs * rhs;
        }
        case DIV: {
            const int64_t lhs = eval_expr(*expr.value.op.lhs);
            const int64_t rhs = eval_expr(*expr.value.op.rhs);
            return lhs / rhs;
        }
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("provide me an argument please\n");
        return 1;
    }

    char *input = argv[1];

    size_t offset = 0;

    const Expr expr = parse_expr(input, &offset);

    const int64_t result = eval_expr(expr);

    printf("%ld\n", result);

    return 0;
}
