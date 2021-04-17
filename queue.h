#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"

void queue_int();
void queue_append(pte_t *pte);
void queue_remove();

typedef struct entry
{
    int prev;
    int next;
    pte_t *pte;
} entry;