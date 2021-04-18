#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "ptentry.h"

int main(int argc, char *argv[]){
	struct pt_entry *entries = malloc(sizeof(struct pt_entry) * 15);
	char *buffer = malloc(sizeof(char) * 1024);
	getpgtable(entries, 15, 0);
	for(int i = 0; i < 15; ++i){
		printf(1, "%d: pdx: %x, ptx: %x, ppage: %x, present: %d, writable: %d, encrypted: %d, ref: %d\n", i, entries[i].pdx, entries[i].ptx, entries[i].ppage, entries[i].present, entries[i].writable, entries[i].encrypted, entries[i].ref);
	}
	free(entries);
	free(buffer);
	exit();
}