// Dynamic list of elements of primitive type

#ifndef _TOOLS_LIST_H
#define _TOOLS_LIST_H

#include <stdlib.h>
#include <string.h>

#define LIST_DEFAULT_CAPACITY 1
#define LIST_RESIZE_COEFF 2

#define DEFINE_LIST(T, PREFIX)                                                 \
struct PREFIX {                                                                \
        T *data;                                                               \
        size_t capacity;                                                       \
        size_t size;                                                           \
};                                                                             \
struct PREFIX *PREFIX##_new(void);                                             \
static void PREFIX##_resize(struct PREFIX *self, size_t new_capacity);         \
void PREFIX##_push(struct PREFIX *self, T val);                                \
T PREFIX##_get(struct PREFIX *self, size_t index);                             \
void PREFIX##_free(struct PREFIX *self);

#define LIST_IMPL(T, PREFIX)                                                   \
                                                                               \
struct PREFIX *PREFIX##_new(void) {                                            \
        struct PREFIX *self = (struct PREFIX*)calloc(1, sizeof(struct PREFIX));\
        if (!self) return NULL;                                                \
        self->data = (T *)calloc(1, sizeof(T) * LIST_DEFAULT_CAPACITY);        \
        if (!self->data) {                                                     \
                free(self);                                                    \
                return NULL;                                                   \
        }                                                                      \
        self->capacity = LIST_DEFAULT_CAPACITY;                                \
        return self;                                                           \
}                                                                              \
                                                                               \
static void PREFIX##_resize(struct PREFIX *self, size_t new_capacity) {        \
        if (!self) return;                                                     \
        T *tmp = (T *)realloc(self->data, sizeof(T) * new_capacity);           \
        if (!tmp) return;                                                      \
        self->data = tmp;                                                      \
        self->capacity = new_capacity;                                         \
}                                                                              \
                                                                               \
void PREFIX##_push(struct PREFIX *self, T val) {                               \
        if (!self) return;                                                     \
        if (self->size >= self->capacity) {                                    \
                PREFIX##_resize(self, self->capacity * LIST_RESIZE_COEFF);     \
                if (self->size >= self->capacity) return;                      \
        }                                                                      \
        self->data[self->size++] = val;                                        \
}                                                                              \
                                                                               \
T PREFIX##_get(struct PREFIX *self, size_t index) {                            \
        if (index >= self->size) return 0;                                     \
        return self->data[index];                                              \
}                                                                              \
                                                                               \
void PREFIX##_free(struct PREFIX *self) {                                      \
        if (!self) return;                                                     \
        free(self->data);                                                      \
        free(self);                                                            \
}

#endif // _TOOLS_LIST_H
