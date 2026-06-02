#ifndef _INTERNAL_LEXER_TOKEN_H
#define _INTERNAL_LEXER_TOKEN_H

#include <stddef.h>
#include <string.h>

#include "token_type.h"
#include "../../tools/list.h"

#define TL_DEFAULT_CAPACITY 1
#define TL_RESIZE_COEFF 2

struct token {
        enum token_type type;
        const char *val;
        size_t line, col;
};
struct token new_token(enum token_type type, const char *val, size_t line,
                size_t col);

struct tokl {
        struct token *data;
        size_t capacity;
        size_t size;
};
struct tokl *new_tokl(void);
void push_tokl(struct tokl *tl, struct token tok);
struct token get_tokl(struct tokl *tl, size_t index);
void free_tokl(struct tokl *tl);

#endif // _INTERNAL_LEXER_TOKEN_H
