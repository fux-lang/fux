#include "src/internal/lexer/token.h"

struct token new_token(enum token_type type, const char *val, size_t line,
                size_t col) {
        struct token tok = {type, val, line, col};
        return tok;
}

struct tokl *new_tokl(void) {
        struct tokl *tl = (struct tokl *)calloc(1, sizeof(struct tokl));
        if (!tl) return NULL;
        tl->capacity = TL_DEFAULT_CAPACITY;
        tl->data = (struct token *)calloc(tl->capacity, sizeof(struct token));
        if (!tl->data) return NULL;
        return tl;
}

static void resize_tokl(struct tokl *tl, size_t new_capacity) {
        if (!tl) return;
        struct token *tmp = (struct token *)realloc(tl->data,
                        sizeof(struct token) * new_capacity);
        if (!tmp) return;
        tl->data = tmp;
        tl->capacity = new_capacity;
}

void push_tokl(struct tokl *tl, struct token tok) {
        if (tl->size >= tl->capacity) {
                resize_tokl(tl, tl->capacity * TL_RESIZE_COEFF);
        }
        tl->data[tl->size++] = tok;
}

struct token get_tokl(struct tokl *tl, size_t index) {
        if (index >= tl->size) {
                // ! ERROR
        }
        return tl->data[index];
}

void free_tokl(struct tokl *tl) {
        if (!tl || !tl->data) return;
        free(tl->data);
        free(tl);
}
