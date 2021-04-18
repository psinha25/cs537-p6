#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"

void queue_int();
void queue_append(entry *queue, char *va, pte_t *pte);
void queue_remove();

typedef struct entry
{
    int prev;
    int next;
    char *va;
    pte_t *pte;
} entry;

typedef struct clockqueue
{
    entry buffer[CLOCKSIZE];
    int head;
    int tail;
} clockqueue;