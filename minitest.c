#include "types.h"
#include "user.h"
#include "param.h"
#include "stat.h"
#include "ptentry.h"

#define PGSIZE 4096

int main(int argc, char *argv[])
{
    char *ptr = sbrk(PGSIZE); // Allocate one page
    struct pt_entry *pt_entry = malloc(sizeof(struct pt_entry));
    // Get the page table information for newly allocated page
    // and the page should be encrypted at this point
    getpgtable(pt_entry, 1, 0);
    ptr[0] = 0x0;
    // The page should now be decrypted and put into the clock queue
    getpgtable(pt_entry, 1, 1);
}