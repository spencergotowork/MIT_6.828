// test reads and writes to a large bss

#include <inc/lib.h>

#define ARRAYSIZE (1024*1024)

uint32_t bigarray[ARRAYSIZE];

void
umain(int argc, char **argv)
{
	int i;

	cprintf("Making sure bss works right...\n");
	// cprintf("the fuck lab...\n");
	// cprintf("++++++++++++++++++++++++if (bigarray[i] != 0)+++++++++++++++++++++++++\n");
	for (i = 0; i < ARRAYSIZE; i++)
		if (bigarray[i] != 0)
			panic("bigarray[%d] isn't cleared!\n", i);
	// cprintf("------------------------if (bigarray[i] != 0)-------------------------\n");

	for (i = 0; i < ARRAYSIZE; i++)
		bigarray[i] = i;

	// cprintf("++++++++++++++++++++++++if (bigarray[i] != i)+++++++++++++++++++++++++\n");
	for (i = 0; i < ARRAYSIZE; i++)
		if (bigarray[i] != i)
			panic("bigarray[%d] didn't hold its value!\n", i);
	// cprintf("------------------------if (bigarray[i] != i)-------------------------\n");

	cprintf("Yes, good.  Now doing a wild write off the end...\n");
	bigarray[ARRAYSIZE+1024] = 0;
	panic("SHOULD HAVE TRAPPED!!!");
}
