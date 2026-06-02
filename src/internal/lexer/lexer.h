#ifndef _INTERNAL_LEXER_LEXER_H
#define _INTERNAL_LEXER_LEXER_H

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "../../src/tools/list.h"

struct lexer {
        const char *input;
        size_t input_len;
        size_t pos;
        size_t line, col;
};
struct lexer *new_lexer(const char *input);
struct tokl *tokenize(struct lexer *lex);
void free_lexer(struct lexer *lex);

#endif // _INTERNAL_LEXER_LEXER_H
