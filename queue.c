// #include "queue.h"
#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"

static int queue_findfree(clockqueue *queue)
{
    int i;
    for (i = 0; i < CLOCKSIZE; i++)
    {
        if (queue->buffer[i].pte == NULL &&
            queue->buffer[i].va == NULL &&
            queue->buffer[i].prev == -1 &&
            queue->buffer[i].next == -1)
            return i;
    }
    return -1;
}

static void send_to_end(clockqueue *queue)
{
    // There aren't any entries in the queue
    if (queue->head == -1)
        return;

    // Only one entry in the queue
    if (queue->head == queue->tail)
        return;

    // Temp variable for current head
    int old_head = queue->head;

    // Move head up the queue
    queue->head = queue->buffer[queue->head].next;
    queue->buffer[queue->head].prev = -1;

    // Move the previous head to end of queue
    queue->buffer[old_head].next = -1;
    queue->buffer[queue->tail].next = old_head;
    queue->buffer[old_head].prev = queue->tail;

    // Update the tail to be the previous head
    queue->tail = old_head;
}

static int find_victim(clockqueue *queue)
{
    int victim = -1;
    int curr;
    while (victim == -1)
    {
        curr = queue->head;
        pte_t *pte = queue->buffer[curr].pte;
        // Haven't found our victim
        if ((*pte & PTE_A) != 0)
        {
            // Flip PTE_A bit to 0
            *pte = *pte & ~PTE_A;
            send_to_end(queue);
        }
        // Found our victim
        else
        {
            // Encrypt the victim page
            mencrypt(queue->buffer[curr].va, 1);

            // Move the head to current head's next
            queue->head = queue->buffer[curr].next;
            // Set new head's previous
            queue->buffer[queue->head].prev = -1;
            victim = curr;
        }
    }
    return victim;
}

void queue_init(clockqueue *queue)
{
    int i;
    for (i = 0; i < CLOCKSIZE; i++)
    {
        queue->buffer[i].pte = NULL;
        queue->buffer[i].va = NULL;
        queue->buffer[i].prev = -1;
        queue->buffer[i].next = -1;
    }
    queue->head = -1;
    queue->tail = -1;
}

void queue_append(clockqueue *queue, char *va, pte_t *pte)
{
    int new_tail = queue_findfree(queue);

    // Number of decrypted is CLOCKSIZE, find victim
    if (new_tail == -1)
    {
        new_tail = find_victim(queue);
    }

    // Add to working set
    queue->buffer[new_tail].pte = pte;
    queue->buffer[new_tail].va = va;
    queue->buffer[new_tail].prev = queue->tail;
    queue->buffer[new_tail].next = -1;

    // Only update the current tails next if tail isn't -1
    if (queue->tail != -1)
        queue->buffer[queue->tail].next = new_tail;

    // Update tail
    queue->tail = new_tail;

    // Edge case where we are pushing the first process
    if (queue->head == -1)
    {
        queue->head = queue->tail;
    }

    // Decrypt the new page
    mdecrypt(va);
}

void queue_remove(clockqueue *queue, pte_t *pte)
{
    int i;
    for (i = 0; i < CLOCKSIZE; i++)
    {
        if (queue->buffer[i].pte == pte)
        {
            // Edge case: removing tail
            if (queue->tail == i)
            {
                queue->tail = queue->buffer[i].prev;
            }
            // Edge case: removing head
            if (queue->head == i)
            {
                queue->head = queue->buffer[i].next;
            }

            // Update the prev and next "pointers" of the
            // neightboring entries if of the proc we are removing
            // if those entries exist
            if (queue->buffer[i].prev != -1)
            {
                queue->buffer[queue->buffer[i].prev].next = queue->buffer[i].next;
            }
            if (queue->buffer[i].next != -1)
            {
                queue->buffer[queue->buffer[i].next].prev = queue->buffer[i].prev;
            }

            // "Free" the memory in the queue
            queue->buffer[i].pte = NULL;
            queue->buffer[i].va = NULL;
            queue->buffer[i].next = -1;
            queue->buffer[i].prev = -1;
            break;
        }
    }
}
