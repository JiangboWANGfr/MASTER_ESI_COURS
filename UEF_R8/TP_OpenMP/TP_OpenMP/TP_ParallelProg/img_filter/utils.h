#ifndef UTILS_H_GUARD
#define UTILS_H_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static inline void die(const char* msg, const char* filename, const int fileno) {
    fprintf(stderr,"%s:%d:\t", filename, fileno);
    fprintf(stderr,"%s\n", msg);
    exit(-1);
}

#define DIE(msg) die((msg), __FILE__, __LINE__)

static inline void fatal(const char* msg, const char* filename, const int fileno) {
    fprintf(stderr,"%s:%d:\t", filename, fileno);
    perror(msg);
    exit(errno);
}

#define FATAL(msg) fatal((msg), __FILE__, __LINE__)

static inline void* xmalloc(size_t size, const char* filename, int lineno) {
    void* p = malloc(size);
    if (!p) fatal("malloc", filename, lineno);
    return p;
}

#define XMALLOC(size) xmalloc((size), __FILE__, __LINE__)

#endif //  UTILS_H_GUARD
