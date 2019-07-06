#pragma once
/**
 * Wrapper for cmake build. This is required for emcc, I include it in the 
 * desktop build for testing purposes.
 */
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

#define S_IFMT  ((mode_t) 0170000)	/* type of file */
#define S_IFREG ((mode_t) 0100000)	/* regular */
#define S_IFBLK 0060000		/* block special */
#define S_IFDIR 0040000  	/* directory */
#define S_IFCHR 0020000		/* character special */
#define S_IFIFO 0010000		/* this is a FIFO */
#define S_ISUID 0004000		/* set user id on execution */
#define S_ISGID 0002000		/* set group id on execution */
				/* next is reserved for future use */
#define S_ISVTX   01000		/* save swapped text even after use */


#ifdef __EMSCRIPTEN__

static inline FILE *popen(const char *command, const char *type) {
    return NULL;
}

static inline size_t getline(char **lineptr, size_t *n, FILE *stream) {
    return 0;
}

static inline int setenv(const char *name, const char *value, int overwrite) {
    return 0;
}

static inline int unsetenv(const char *name) {
    return 0;
}

static inline int access(const char *pathname, int mode) {
    return 0;
}

static inline int chdir(const char *path) {
    return 0;
}

static inline char *getcwd(char *buf, size_t size) {
    return 0;
}

#endif