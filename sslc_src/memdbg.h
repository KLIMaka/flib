//	------------------------------------------------------------------------------------------
//
//		Copyright ©1996, Interplay Productions, Inc.
//		Not to be distributed without express consent of Interplay Productions
//
//	------------------------------------------------------------------------------------------

#ifndef _MEMDBG_H_
#define _MEMDBG_H_

#ifndef FALLOUT_BUILD                   // turns of by default for Fallout
#define INCLUDE_MEMDBG
#endif

#include <stdlib.h>
#include <string.h>

extern void memoryRegisterAlloc(void *(*)(size_t), void *(*)(void *, size_t),
                                void (*)(void *));

#define malloc(x)     mymalloc(x, __FILE__, __LINE__)
#define realloc(x,y)  myrealloc(x, y, __FILE__, __LINE__)
#define calloc(x,y)   mycalloc(x, y, __FILE__, __LINE__)
#define free(x)       tmpfree((void *)(&(x)), __FILE__, __LINE__)
#define strdup(x)     mystrdup(x, __FILE__, __LINE__)


#ifdef INCLUDE_MEMDBG

/*
 * Debug level 0 -- exits on malloc error, prints file and line error occurs
 * Debug level 1 -- just tracks block allocation, does no heap checking; useful for seeing where memory is being used
 * Debug level 2 -- tracks all blocks, checks for heap stomping
 */
#define MEMORY_DEBUG 0

extern void memoryRegisterDebug(int (*)(char *));
#endif

#if defined _PSX_
#undef strdup
#endif

extern void check_memory(void);
extern void dump_memory(void);
int get_current_memory(void);
int get_total_small_blocks(void);
int get_current_small_blocks(void);
extern void watch_free(void *);
extern void check_all(void);

void *mymalloc(size_t, char *, int), *myrealloc(void *, size_t, char *, int),
     *mycalloc(size_t, size_t, char *, int);
void myfree(void *, char *, int);
void tmpfree(void *, char *, int);
char *mystrdup( const char *, char *, int);

#endif
