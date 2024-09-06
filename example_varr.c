#include <malloc.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#define INCLUDE_VARR
#include "varr.c"
#define LOGD(str, ...)

typedef struct _LinkListData {
  const char *name;
} _LinkListData;

typedef struct LinkList {
  _LinkListData data;
  struct LinkList *next;
} LinkList;



typedef struct LinkListInterface {
  LinkList *head;
  varr mem; /* List of all memories allocated */
} LinkListInterface;

typedef struct LinkListSatus {
  uint32_t status; /** Support for Enum ? **/
  const char *status_description;
} LinkListStatus;

static LinkList **GetNodeToAdd(LinkListInterface *list) {
  LOGD("GetNodeToAdd: Adding the node to the list\n");
  LinkList *node = list->head;
  if (node == NULL) {
    return &list->head;
  }
  for (; node->next != NULL; node = node->next) {
  }
  return &node->next;
}

void *LinkListInterface_AddHm(LinkListInterface *list, void *data,
                              size_t size) {
  return data;
}

LinkListStatus AddNode(LinkListInterface *list, const _LinkListData *data) {
  if (list->mem.mem == 0) {
    varrnew(&list->mem, sizeof(void *) * 4000);
  }
  LinkListStatus status = {0, "Success"};
  if (list == NULL) {
    printf("AddNode: list is NULL\n");
    return (LinkListStatus){-1};
  }

  LinkList *newnode = (LinkList *)malloc(sizeof(LinkList));
  if (newnode == NULL) {
    return (LinkListStatus){-1, "Criticial Error: Malloc Failed"};
  }
  varradd(&list->mem, &newnode, sizeof(void *));
  memset(newnode, 0, sizeof(LinkList));
  LinkList **addnode = GetNodeToAdd(list);
  *addnode = newnode;
  memcpy(&newnode->data, data, sizeof(newnode->data));
  newnode->next = NULL;
  printf("AddNode: Added Successfuly = %p\n", newnode);
  return status;
}

void PrintList(const LinkListInterface *list) {
  for (LinkList *node = list->head; node != NULL; node = node->next) {
    printf("Data: %s\n", node->data.name);
  }
}

void *cbvarfree(void **p) {
  printf("cbfree: Releaseing %p\n", (*p));
  free(*p);
  return p;
}

void *cbvarprint(void **p) {
  printf("cbprint: %s\n", ((_LinkListData *)*p)->name);
  return p;
}

int main_test1() {
  LinkListInterface test1 = {};
  AddNode(&test1, (_LinkListData[]){"01. Hello"});
  AddNode(&test1, (_LinkListData[]){"02. Hello"});
  AddNode(&test1, (_LinkListData[]){"03. Hello"});
  AddNode(&test1, (_LinkListData[]){"04. Hello"});
  PrintList(&test1);
  varrprintmem(&test1.mem, sizeof(void *));
  varrexec(&test1.mem, cbvarprint, sizeof(void *));
  varrexec(&test1.mem, cbvarfree, sizeof(void *));
  return 0;
}
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
  main_test1();
  main_test2();
  main_test3();
  main_test4();
  return 0;
}
