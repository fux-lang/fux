#include "token.h"

struct token new_token(enum token_type type, const char *val, size_t line,
                size_t col) {
        struct token tok;
        tok.type = type;
        tok.val = strdup(val);
        tok.line = line;
        tok.col = col;
        return tok;
}

struct tokl *new_tokl(void) {
        struct tokl *tl = (struct tokl *)calloc(1, sizeof(struct tokl));
        if (!tl) return NULL;
        tl->capacity = TL_DEFAULT_CAPACITY;
        tl->data = (struct token *)calloc(tl->capacity, sizeof(struct token));
        if (!tl->data) {
                free(tl);
                return NULL;
        }
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
                if (tl->size >= tl->capacity) return;
        }
        tl->data[tl->size++] = tok;
}

struct token get_tokl(struct tokl *tl, size_t index) {
        if (!tl || index >= tl->size) {
                struct token err_tok;
                err_tok.type = ERR;
                err_tok.val = "003";
                err_tok.line = 0;
                err_tok.col = 0;
                return err_tok; 
        }
        return tl->data[index];
}

void free_tokl(struct tokl *tl) {
        if (!tl) return;
        if (tl->data) {
                for (size_t i = 0; i < tl->size; ++i) {
                        free((void *)tl->data[i].val);
                }
                free(tl->data);
        }
        free(tl);
}
