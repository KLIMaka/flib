//	------------------------------------------------------------------------------------------
//
//		Copyright ©1996, Interplay Productions, Inc.
//		Not to be distributed without express consent of Interplay Productions
//
//		$Revision: 2 $
//		$Author: Panettiere $
//		$Date: 11/19/97 6:49p $
//		$Archive: /ka/comp/MEMDBG.C $
//
//	------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "memdbg.h"

#undef malloc
#undef realloc
#undef free
#undef calloc

//#define WATCOM_CHECK_HEAP
//#define KEEP_FREE_LIST

#ifdef WATCOM_CHECK_HEAP
#include <malloc.h>

static void watcomCheckHeap(void) {
   switch( _heapchk() ) {
      case _HEAPOK:
         printf( "OK - heap is good\n" );
         break;
      case _HEAPEMPTY:
         printf( "OK - heap is empty\n" );
         break;
      case _HEAPBADBEGIN:
         printf( "ERROR - heap is damaged\n" );
         exit(1);
      case _HEAPBADNODE:
         printf( "ERROR - bad node in heap\n" );
         exit(1);
   }
}
#else
#define watcomCheckHeap()
#endif


static int defaultOutput(char *a) { return (printf("%s", a)); }
static int (*outputFunc)(char *) = defaultOutput;
void memoryRegisterDebug(int (*func)(char *)) { outputFunc = func; }

#ifndef _PSX_
#if defined(_MSC_VER)
static int __cdecl debug_printf(const char *fmt, ...) {
#else
static int debug_printf(const char *fmt, ...) {
#endif
   static char buf[256];
   int i;
   va_list arglist;

   if (!outputFunc) return 0;
   va_start(arglist, fmt);
   i = vsprintf(buf, fmt, arglist);
   va_end(arglist);
   outputFunc(buf);
   return i;
}
#else								//psx does not support vsprintf... I am working on it
static int debug_printf(char *buf)
{
	if (!outputFunc) return 0;
	outputFunc(buf);
	return 1;
}
#endif

static void error(char *s, unsigned long i, char *file, int line) {
#ifndef _PSX_
   debug_printf("%s: Error allocating block of size %ld (%x), %s %d\n", s, i, i, file, line);
#else
	char buf[256];
	sprintf(buf,"%s: Error allocating block of size %ld, %s %d\n", s, i, file, line);
	debug_printf(buf);
#endif
   exit(1);
}


static void *defaultMalloc(size_t s) { return malloc(s); }
static void *defaultRealloc(void *p, size_t s) { return realloc(p, s); }
static void defaultFree(void *p) { free(p); }
static void *(*mallocPtr)(size_t) = defaultMalloc;
static void *(*reallocPtr)(void *, size_t) = defaultRealloc;
static void (*freePtr)(void *) = defaultFree;
void memoryRegisterAlloc(void *(*m)(size_t),
                         void *(*r)(void *, size_t),
                         void (*f)(void *)) {
   mallocPtr = m;
   reallocPtr = r;
   freePtr = f;
}


#ifdef INCLUDE_MEMDBG

static int total = 0;

static int checkTotal(int total) {
   if (total < 0) {
      debug_printf("Total went negative!!!\n");
      exit(1);
   }
   return 1;
}

#define SMALLBLOCK 0x80000000

#if MEMORY_DEBUG > 0
typedef struct tag_heapdata {
   unsigned long tag;
   unsigned long size;
   unsigned char *file;
   unsigned long line;
   struct tag_heapdata *next;
   struct tag_heapdata *prev;
} HeapData;
static HeapData *heapHead;
static HeapData *freeList;
static void *findMem(int size, char *f, int l);

static void *add_new_ptr(void *x, int s, char *f, int l);
#define add_ptr(x,y,a,b) add_new_ptr(x,y,a,b)
#define get_ptr(x)   ((void *)((unsigned char *)(x) - sizeof(HeapData)))
#define get_size(x)  (((HeapData *)get_ptr(x))->size)
#define TWEAK(s) ((s) + sizeof(HeapData) + sizeof(long))
#else

#ifdef __MACBUILD__
#define findMem(x,a,b) 0
#define memdbgFree(x) freePtr(x)
#define add_ptr(a,b,c,d) (total += (b), checkTotal(total), *((long *)a) = (b), (void *)(((long *)a) + 2))
#define get_ptr(f) ((void *)(((long *)f) - 2))
#define check_and_remove_ptr(a,b,c) (total -= *(long *)get_ptr(a), checkTotal(total), total)
#define get_size(a) (*(long *)get_ptr(a))
#define TWEAK(x) (x + sizeof(double))  // Ideally, I want all data to be 8-byte aligned.
#else  // !Mac
#define findMem(x,a,b) 0
#define memdbgFree(x) freePtr(x)
#define add_ptr(a,b,c,d) (total += (b), checkTotal(total), *((long *)a) = (b), (void *)(((long *)a) + 1))
#define get_ptr(f) ((void *)(((long *)f) - 1))
#define check_and_remove_ptr(a,b,c) (total -= *(long *)get_ptr(a), checkTotal(total), total)
#define get_size(a) (*(long *)get_ptr(a))
#define TWEAK(x)   (x + sizeof(long))
#endif  // !Mac

#endif

static void *largeMalloc(int size, char *f, int l) {
   void *new;

   new = findMem(size, f, l);

   if (new) return new;

   new = mallocPtr(TWEAK(size));

   if (!new)
      error("malloc", size, f, l);

   return add_ptr(new, size, f, l);
}

#if MEMORY_DEBUG > 0

#define SMALLBLOCKSIZE 0
#define SMALLBLOCKSIZEMASK 0x0000001f
#define get_small_block_size(p) ((*(long *)get_small_block_ptr(p)) & SMALLBLOCKSIZEMASK)
#define get_small_block_ptr(p) ((void *)((long *)(p) - 1))
#define isSmallBlock(p) ((*((long *)get_small_block_ptr(p))) & SMALLBLOCK)
#define currentSmallBlocks 0
#define totalSmallBlocks 0
#define getEnd(x) ((long *)((unsigned char *)(x) + ((HeapData *)(x))->size + sizeof(HeapData)))

static void *add_new_ptr(void *x, int s, char *f, int l) {
   HeapData *a = x;

   total += s;
   checkTotal(total);
   a->tag = 0xFEEDFACE;
   a->size = s;
   a->file = (unsigned char *)f;
   a->line = l;
   a->next = heapHead;
   a->prev = 0;
   if (heapHead)
      heapHead->prev = a;
   heapHead = a;

   *getEnd(x) = 0xBEEFCABE;

   return (void *)((unsigned char *)x + sizeof(HeapData));
}

static void *findMem(int size, char *f, int l) {
#ifdef KEEP_FREE_LIST
   HeapData *a = freeList;

   size = (size + 3) & ~3;
   while(a) {
      if (a->size >= size) {
         // remove this block from the free list
         a->file = f;
         a->line = l;
         if (a->next) a->next->prev = a->prev;
         if (a->prev) a->prev->next = a->next;
         else freeList = a->next;

         // only split blocks if it will result in another new block
         // that is 32 bytes long.  Each block needs sizeof(HeapData) + sizeof(long)
         // extra space.
         if (a->size > size + sizeof(HeapData) + sizeof(long) + 32) {
            HeapData *b;

            // split block.
            b = (HeapData *)((unsigned char *)a + sizeof(HeapData) + size + sizeof(long));

            b->tag = 0xFEEDFACE;
            b->size = a->size - size - sizeof(long) - sizeof(HeapData);
            b->file = 0;
            b->line = 0;
            b->prev = 0;
            b->next = freeList;
            if (freeList) freeList->prev = b;
            freeList = b;
            a->size = size;
            *getEnd(b) = 0xBEEFCABE;
         }
         return add_new_ptr(a, a->size, f, l);
      }
      a = a->next;
   }

#endif
   return 0;
}

void dump_memory(void) {
   HeapData *a = heapHead;
   FILE *f;

   f = fopen("memory.dmp", "w");
   while(a) {
      fprintf(f, "%s %d %d\n", a->file, a->line, a->size);
      a = a->next;
   }

   fclose(f);
}

static void checkPtr(HeapData *a, char *f, int l) {
   if (a->tag != 0xFEEDFACE) {
#ifndef _PSX_
      debug_printf("Header stomped, %s, %d, detected %s %d\n", a->file, a->line, f, l);
#else
	  char buf[256];
	  sprintf(buf,"Header stomped, %s, %d, detected %s %d\n", a->file, a->line, f, l);
	  debug_printf(buf);
#endif
      exit(1);
   }

   if (*getEnd(a) != 0xBEEFCABE) {
#ifndef _PSX_
      debug_printf("Trailer stomped, %s %d, detected %s %d\n", a->file, a->line, f, l);
#else
	  char buf[256];
	  sprintf(buf,"Trailer stomped, %s %d, detected %s %d\n", a->file, a->line, f, l);
#endif
      exit(1);
   }
}

static void check_and_remove_ptr(void *x, char *f, int l) {
   HeapData *a;

   if (!x) {
      debug_printf("Null pointer passed to check_ptr\n");
      return;
   }

   a = get_ptr(x);

#if MEMORY_DEBUG > 1
   checkPtr(a, f, l);
#endif

   if (a->next) a->next->prev = a->prev;
   if (a->prev) a->prev->next = a->next;
   else heapHead = a->next;

   total -= a->size;
   checkTotal(total);
}

static void memdbgFree(void *x) {
   HeapData *a;

   if (!x) {
      debug_printf("Null pointer passed to memdbgFree\n");
      exit(1);
   }

   a = (HeapData *)x;

#ifdef KEEP_FREE_LIST
   a->prev = 0;
   a->next = freeList;
   if (freeList) freeList->prev = a;
   freeList = a;
   a->tag = 0xFEEDFACE;
   *getEnd(a) = 0xBEEFCABE;
#else
   freePtr(a);
#endif
}

#define check_all() my_check_all(__FILE__, __LINE__)

#if MEMORY_DEBUG > 1
int memdbgEnabled = 1;
void my_check_all(char *f, int l) {
   if (memdbgEnabled) {
      HeapData *a = heapHead;

      while(a) {
         checkPtr(a, f, l);
         a = a->next;
      }

      a = freeList;
      while(a) {
         checkPtr(a, f, l);
         a = a->next;
      }

      watcomCheckHeap();
   }   
}
#else
void my_check_all(char *f, int l) { f = f; l = l; }
#endif


void check_memory() {
   HeapData *a = heapHead;
   while(a) {
      char buf[256];
      sprintf(buf,"%s %d %d\n", a->file, a->line, a->size);
      debug_printf(buf);
      a = a->next;
   }
}

#else

void my_check_all(char *f, int l) { f = f; l = l; watcomCheckHeap(); }
void check_memory(void) { }
void dump_memory(void) { }

#define SMALLBLOCKSIZE 0

#if SMALLBLOCKSIZE != 0
#define HEAPNODESIZE (sizeof(HeapNode))
#define SMALLBLOCKSIZEMASK ~SMALLBLOCK
#define HEAPSIZE (4 * 1024 / HEAPNODESIZE)           // size in HeapNodes
#define get_small_block_size(p) ((*(long *)get_small_block_ptr(p)) & SMALLBLOCKSIZEMASK)
#define get_small_block_ptr(p) ((void *)((unsigned char *)p - offset(struct tag_data, data)))
#define isSmallBlock(p) ((*((long *)get_small_block_ptr(p))) & SMALLBLOCK)

// this code makes heavy assumptions that sizeof(pointer) == sizeof(long)
// the small block stuff also limits the lines in one file, if memdbg is on,
// to 0x7fffffff (I use the sign bit of the line number as a flag to tell me if
// it is a small block or not).  I don't think this will be a problem.

#define offset(type, field) ((size_t)&(((type *)0)->field))

typedef union tag_heapnode {
   struct tag_freeNode {
      union tag_heapnode *next;
      union tag_heapnode *prev;
   } freeNode;
   // this struct is here because I can't find the offset of
   // data in the struct tag_data structure inside of that structure;
   // this code is assuming that identical structures will be padded
   // identically.
   struct bogus_struct {
      unsigned long size;
      unsigned char data[];
   } bogus;
   struct tag_data {
      unsigned long size;
      unsigned char data[SMALLBLOCKSIZE + 4 - offset(struct bogus_struct, data)];
   } data;
} HeapNode;

// this structure *MUST* be > SMALLBLOCKSIZE in size
typedef struct tag_smallheap{
   struct tag_smallheap *next;
   struct tag_smallheap *prev;
   HeapNode *heap;
   HeapNode *free;
   int numAllocated;
} SmallHeap;

static SmallHeap *curHeap;
static int currentSmallBlocks, totalSmallBlocks;

static void smallFree(void *p, char *f, int l) {
   SmallHeap *cur = curHeap;
   HeapNode *n;

   total -= get_small_block_size(p);
   checkTotal(total);

   p = get_small_block_ptr(p);

   while (cur && (p < cur->heap || p >= cur->heap + HEAPSIZE))
      cur = cur->next;

   if (!cur) {
      exit(1);
   }

   n = p;

   n->freeNode.next = cur->free;
   n->freeNode.prev = 0;
   currentSmallBlocks--;

   if (cur->free) cur->free->freeNode.prev = n;
   cur->free = n;
   cur->numAllocated--;
   if (cur->numAllocated == 0) {
      if (cur->prev) cur->prev->next = cur->next;
      else curHeap = cur->next;

      if (cur->next) cur->next->prev = cur->prev;
      myfree(cur->heap, f, l);
      myfree(cur, f, l);
   }
}

static void *smallMalloc(int size, char *f, int l) {
   SmallHeap *cur = curHeap, *prev = 0;
   HeapNode *n, *p;

   while(cur) {
      if (cur->free) break;
      prev = cur;
      cur = cur->next;
   }

   if (!cur) {
      cur = largeMalloc(sizeof(SmallHeap), f, l);

      cur->numAllocated = 0;
      cur->prev = 0;
      cur->next = curHeap;
      if (curHeap) curHeap->prev = cur;
      curHeap = cur;

      n = cur->heap = largeMalloc(HEAPSIZE * HEAPNODESIZE, f, l);
      cur->free = cur->heap;

#define nextHeap(n) ((HeapNode *)((char *)(n) + HEAPNODESIZE))
      n->freeNode.prev = 0;
      n->freeNode.next = nextHeap(n);
      p = n;
      n = nextHeap(n);

      while(n < cur->heap + HEAPSIZE - 1) {
         n->freeNode.next = n + 1;
         n->freeNode.prev = p;
         p = n;
         n = nextHeap(n);
      }

      n->freeNode.prev = p;
      n->freeNode.next = 0;
   }

   n = cur->free;
   cur->free = cur->free->freeNode.next;

   if (n->freeNode.prev)
      n->freeNode.prev->freeNode.next = n->freeNode.next;
   else
      cur->free = n->freeNode.next;

   if (n->freeNode.next)
      n->freeNode.next->freeNode.prev = n->freeNode.prev;

   n->data.size = size | SMALLBLOCK;
   cur->numAllocated++;

   total += size;
   checkTotal(total);

   totalSmallBlocks++;
   currentSmallBlocks++;

   return (void *)(n->data.data);
}
#else
#define SMALLBLOCKSIZEMASK 0x0000001f
#define get_small_block_size(p) ((*(long *)get_small_block_ptr(p)) & SMALLBLOCKSIZEMASK)
#define get_small_block_ptr(p) ((void *)((long *)(p) - 1))
#define isSmallBlock(p) ((*((long *)get_small_block_ptr(p))) & SMALLBLOCK)
#define currentSmallBlocks 0
#define totalSmallBlocks 0
#endif // SMALLBLOCKSIZE != 0

#endif

void *mymalloc(size_t size, char *f, int l) {
   void *r;
   my_check_all(f, l);

#if SMALLBLOCKSIZE != 0
   if (size < SMALLBLOCKSIZE) {
      r = smallMalloc(size, f, l);
   }
   else
#endif
      r = largeMalloc(size, f, l);
   return r;
}

#if !defined(_MSC_VER)
#pragma argsused
#endif
void *mycalloc(size_t s, size_t n, char *f, int l) {
   void *new;

   my_check_all(f, l);

   // compilers will probably get a warning here if
   // memory debugging is turned on, since I set SMALLBLOCKSIZE to zero.
#if SMALLBLOCKSIZE != 0
   if (s*n < SMALLBLOCKSIZE) {
      new = smallMalloc(s*n, f, l);
      memset(new, 0, s*n);
      return new;
   }
#endif

   new = mallocPtr(TWEAK(s*n));
   if (!new)
      error("calloc", s*n, f, l);
   new = add_ptr(new, s*n, f, l);
   memset(new, 0, s*n);
   return new;
}

void *myrealloc(void *r, size_t s, char *f, int l) {
   void *new;

   my_check_all(f, l);

#if SMALLBLOCKSIZE != 0
   if (isSmallBlock(r)) {
      if (s < SMALLBLOCKSIZE) {
         total += s - get_small_block_size(r);
         checkTotal(total);
         *(long *)get_small_block_ptr(r) = s | SMALLBLOCK;
         return r;
      }

      new = mymalloc(s, f, l);
      memcpy(new, r, get_small_block_size(r));
      myfree(r, f, l);
      return new;
   }
#endif

#if MEMORY_DEBUG > 0 && defined(KEEP_FREE_LIST)
   {
      HeapData *p = get_ptr(r), *q;
      if (p->size >= s) {
         p->file = f;
         p->line = l;
         return r;
      }

      q = mymalloc(s, f, l);
      if (!q) return 0;

      memcpy(q, r, p->size);
      myfree(r, f, l);
      return q;
   }
#else
   check_and_remove_ptr(r, f, l);
   new = reallocPtr(get_ptr(r), TWEAK(s));
   if (!new)
      error("realloc", s, f, l);

   return add_ptr(new, s, f, l);
#endif
}

void myfree(void *p, char *f, int l) {
   if (!p) {
#ifndef _PSX_
      debug_printf("free: free of a null ptr, %s %d\n", f, l);
#else
	  char buf[256];
	  sprintf(buf,"free: free of a null ptr, %s %d\n", f, l);
	  debug_printf(buf);
#endif
      exit(1);
   }

#if SMALLBLOCKSIZE != 0
   if (isSmallBlock(p))
      smallFree(p, f, l);
   else {
      check_and_remove_ptr(p, f, l);
      my_check_all(f, l);
      memdbgFree(get_ptr(p));
   }
#else
   check_and_remove_ptr(p, f, l);
   my_check_all(f, l);
   memdbgFree(get_ptr(p));
#endif
}

char *mystrdup( const char *s, char *f, int l)
{
   int            len;
   char           *new;

	len = strlen( s);
	new = mymalloc( len + 1, f, l);
   if (!new)
		error( "strdup", len + 1, f, l);
	strcpy( new, s);
	return new;
}

int get_current_memory(void) { return total; }
int get_total_small_blocks(void) { return totalSmallBlocks; }
int get_current_small_blocks(void) { return currentSmallBlocks; }
#else

void my_check_all(char *f, int l) { f = f; l = l; }
void check_memory(void) { }
void dump_memory(void) { }
int get_current_memory(void) { return 0; }
int get_total_small_blocks(void) { return 0; }
int get_current_small_blocks(void) { return 0; }


void *mymalloc(size_t size, char *f, int l) {
   void *n;
   n = mallocPtr(size);
   if (!n) error("malloc", size, f, l);

   return n;
}

void myfree(void *p, char *f, int l) {
   if (!p) {
#ifndef _PSX_
      debug_printf("free: free of a null ptr, %s %d\n", f, l);
#else
	  char buf[256];
	  sprintf(buf,"free: free of a null ptr, %s %d\n", f, l);
	  debug_printf(buf);
#endif
      exit(1);
   }
   freePtr(p);
}

void *myrealloc(void *p, size_t size, char *f, int l) {
   p = reallocPtr(p, size);
   if (!p) error("realloc", size, f, l);
   return p;
}

void *mycalloc(size_t size, size_t nelems, char *f, int l) {
   void *n = mallocPtr(size*nelems);
   if (!n) error("calloc", size*nelems, f, l);
   memset(n, 0, size*nelems);
   return n;
}

char *mystrdup(const char *s, char *f, int l) {
   int            len;
   char           *new;

	len = strlen( s);
//	new = mallocPtr( len + 1, f, l);
   new = mallocPtr( len + 1);
   if (!new)
		error( "strdup", len + 1, f, l);
	strcpy( new, s);
	return new;
}


#endif

void tmpfree(void *p, char *file, int line) {
   myfree(*(void **)p, file, line);
   *(void **)p = 0;
}
