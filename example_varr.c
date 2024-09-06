#include <malloc.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#define INCLUDE_VARR
#include "varr.c"
#define LOGD(str, ...)


void *structprint(void **p) {
  printf("String %s\n", (char *)p);
  return NULL;
}

void *strnarrprint(void **p) {
  char *ptr = (char *)p;
  for (; ptr && *ptr; ptr = ptr + strlen(ptr) + 1) {
    printf("String %s\n", ptr);
  }
  return NULL;
}

void *strarrprint(void **p) {
  printf("String %s\n", (char *)*p);
  return NULL;
}

int main_test2() {
  varr p;
  varrnew(&p, 15000);
  char *p1 = "01.HelloWorld";
  char *p2 = "02.HelloWorld";
  varradd(&p, &p1, sizeof(void *));
  varradd(&p, &p2, sizeof(void *));

  varrexec(&p, strarrprint, sizeof(void *));
  return 0;
}

int main_test3() {
  varr p;
  varrnew(&p, 15000);

  varradd(&p, "03.HelloWorld", sizeof("01.HelloWorld"));
  varradd(&p, "04.HelloWorld", sizeof("02.HelloWorld"));

  varrexec(&p, structprint, sizeof("02.HelloWorld"));
  return 0;
}

int main_test4() {
  varr p;
  varrnew(&p, 15000);

  varradd(&p, "05.HelloWorld1", sizeof("01.HelloWorld1"));
  varradd(&p, "06.HelloWorld12", sizeof("02.HelloWorld12"));
  varradd(&p, "07.HelloWorld123", sizeof("02.HelloWorld123"));
  varradd(&p, "07.HelloWorld123", sizeof("02.HelloWorld123"));

  varrexecmem(&p, strnarrprint);
  return 0;
}

int main() {  
  main_test2();
  main_test3();
  main_test4();
  return 0;
}
