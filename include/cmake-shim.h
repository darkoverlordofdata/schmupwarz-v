#pragma once
/**
 * Wrapper for cmake build. This is required for emcc, I include it in the 
 * desktop build for testing purposes.
 */
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

/**
 * Not using pipes in emcc 
 */
// static inline FILE *popen(const char *command, const char *type) {
//     return NULL;
// }

// static inline size_t getline(char **lineptr, size_t *n, FILE *stream) {
//     return 0;
// }
