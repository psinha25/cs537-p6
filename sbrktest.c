#include "types.h"
#include "user.h"
#include "param.h"
#include "stat.h"
#include "ptentry.h"

#define PGSIZE 4096

/*
 * To use this program, first add code to growproc() in proc.c
 * that prints the old and new values of "sz". Also add a print statement
 * to allocuvm every time a new page is allocated.
 *
 * Then observe the results when calling sbrk() with different values.
 */

int main(int argc, char *argv[])
{
    int num_entries = 15;
    struct pt_entry *entries = malloc(sizeof(struct pt_entry) * num_entries);
    getpgtable(entries, num_entries, 0);
    for (int i = 0; i < num_entries; ++i)
    {
        printf(1, "%d: pdx: %x, ptx: %x, ppage: %x, present: %d, writable: %d, user: %d, encrypted: %d, ref: %d\n",
               i, entries[i].pdx,
               entries[i].ptx, entries[i].ppage,
               entries[i].present, entries[i].writable,
               entries[i].user, entries[i].encrypted, entries[i].ref);
    }

    printf(1, "\nsbrk(PGSIZE)\n");
    sbrk(PGSIZE);

    // printf(1, "\nsbrk(PGSIZE / 2)\n");
    // sbrk(PGSIZE / 2);

    // printf(1, "\nsbrk(PGSIZE)\n");
    // sbrk(PGSIZE);

    // printf(1, "\nsbrk(1)\n");
    // sbrk(1);

    // printf(1, "\nsbrk((PGSIZE / 2) - 1)\n");
    // sbrk((PGSIZE / 2) - 1);

    getpgtable(entries, num_entries, 0);
    for (int i = 0; i < num_entries; ++i)
    {
        printf(1, "%d: pdx: %x, ptx: %x, ppage: %x, present: %d, writable: %d, user: %d, encrypted: %d, ref: %d\n",
               i, entries[i].pdx,
               entries[i].ptx, entries[i].ppage,
               entries[i].present, entries[i].writable,
               entries[i].user, entries[i].encrypted, entries[i].ref);
    }

    printf(1, "\n");
    free(entries);
    exit();
    return 0;
}