#include <malloc.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#define LOGD(str, ...)



#define INCLUDE_VARR
#ifdef INCLUDE_VARR
/* Variable Array which act as base line of struct array, pointer array and
 * string array */
typedef struct varr {
  void *mem;
  size_t size;
  size_t loc;
} varr;
/* Create New VArr */
varr *varrnew(varr *o, size_t size) {
  o->mem = malloc(size);
  o->size = size;
  o->loc = 0;
  return o;
}

/* Print Varr as if it is memory */
void varrprintmem(varr *o, size_t size) {
  for (char *p = o->mem; p < (char *)o->mem + o->loc; p = p + size) {
    printf("VarrMem: %p\n", (*(char **)p));
  }
}

/* Execute external function on first mem */
void varrexecmem(varr *o, void *(*func)(void **)) {
  char *p = o->mem;
  func((void **)p);
}

/* Execute external function on all data assumuing equal size for mem it is
 * void* */
void varrexec(varr *o, void *(*func)(void **), size_t size) {
  for (char *p = o->mem; p < (char *)o->mem + o->loc; p = p + size) {
    printf("varr: Execution on %p\n", (*(char **)p));
    func((void **)p);
  }
}

/* Add new VArr reallocate in case of overflow */
void *varradd(varr *o, void *data, size_t size) {
  if (o == NULL) {
    return NULL;
  }

  char *p = (char *)o->mem;
  if (p + size >= p + o->size) {
    o->mem = realloc(o->mem, o->size * 2);
    o->size *= 2;
  }
  memcpy(p + o->loc, data, size);
  o->loc += size;
  printf("varr: Added = %p\n", (*(char **)data));
  return p;
}
#endif
