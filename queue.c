#include "queue.h"

// Queue is an array holding entry structs
entry queue[CLOCKSIZE];

// Head and tail "pointer" in queue
int head;
int tail;

static int queue_findfree()
{
    int i;
    for (int i = 0; i < CLOCKSIZE; i++)
    {
        if (queue[i].pte == NULL && queue[i].prev == -1 && queue[i].next == -1)
            return i;
    }
    return -1;
}

static void send_to_end()
{
    // There aren't any entries in the queue
    if (head == -1)
        return;

    // Only one entry in the queue
    if (head == tail)
        return;

    // Temp variable for current head
    int old_head = head;

    // Move head up the queue
    head = queue[head].next;
    queue[head].prev = -1;

    // Move the previous head to end of queue
    queue[old_head].next = -1;
    queue[tail].next = old_head;
    queue[old_head].prev = tail;

    // Update the tail to be the previous head
    tail = old_head;
}

static int find_victim()
{
    int victim = -1;
    int curr;
    char *ka; // Kernal address of the uva
    while (victim == -1)
    {
        curr = head;
        pte_t *pte = queue[curr].pte;
        // Haven't found our victim
        if (*pte & PTE_A != 0)
        {
            // Flip PTE_A bit to 0
            *pte = *pte & ~PTE_A;
            send_to_end();
        }
        // Found our victim
        else
        {
            // Get kernel address in this PTE
            ka = (char *)P2V(PTE_ADDR(*pte));
            // Do encryption if not already encrypted - should always encrypt
            if (!(*pte & PTE_E))
            {
                for (int i = 0; i < PGSIZE; i++)
                {
                    *(ka + i) ^= 0xFF;
                }
            }

            // PTE bits are properly set(PTE_E = 1 and PTE_P = 0)
            *pte = *pte & ~PTE_P;
            *pte = *pte | PTE_E;

            // Move the head to current head's next
            head = queue[curr].next;
            // Set new head's previous
            queue[head].prev = -1;
            victim = curr;
        }
    }
    return victim;
}

void queue_init()
{
    int i;
    for (int i = 0; i < CLOCKSIZE; i++)
    {
        queue[i].pte = NULL;
        queue[i].prev = -1;
        queue[i].next = -1;
    }
    head = -1;
    tail = -1;
}

void queue_append(pte_t *pte)
{
    int new_tail = queue_findfree();

    // Number of decrypted is CLOCKSIZE, find victim
    if (new_tail == -1)
    {
        new_tail = find_vicim();
    }

    // Add to working set
    queue[new_tail].pte = pte;
    queue[new_tail].prev = tail;
    queue[new_tail].next = -1;

    // Only update the current tails next if tail isn't -1
    if (tail != -1)
        queue[tail].next = new_tail;

    // Update tail
    tail = new_tail;

    // Edge case where we are pushing the first process
    if (head == -1)
    {
        head = tail;
    }
}

void queue_remove(pte_t *pte)
{
    int i;
    for (i = 0; i < CLOCKSIZE; i++)
    {
        if (queue[i].pte == pte)
        {
            // Edge case: removing tail
            if (tail == i)
            {
                tail = queue[i].prev;
            }
            // Edge case: removing head
            if (head == i)
            {
                head = queue[i].next;
            }

            // Update the prev and next "pointers" of the
            // neightboring entries if of the proc we are removing
            // if those entries exist
            if (queue[i].prev != -1)
            {
                queue[queue[i].prev].next = queue[i].next;
            }
            if (queue[i].next != -1)
            {
                queue[queue[i].next].prev = queue[i].prev;
            }

            // "Free" the memory in the queue
            queue[i].pte = NULL;
            queue[i].next = -1;
            queue[i].prev = -1;
            break;
        }
    }
}
