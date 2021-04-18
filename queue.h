#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"

void queue_init(clockqueue *queue);
void queue_append(clockqueue *queue, char *va, pte_t *pte);
void queue_remove(clockqueue *queue, pte_t *pte);

struct entry
{
    int prev;
    int next;
    char *va;
    pte_t *pte;
};

struct clockqueue
{
    entry buffer[CLOCKSIZE];
    int head;
    int tail;
};