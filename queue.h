#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"

void queue_int();
void queue_append(pte_t *pte);
void queue_remove();
void queue_sendtoend();

typedef struct entry
{
    int prev;
    int next;
    pte_t *pte;
} entry;