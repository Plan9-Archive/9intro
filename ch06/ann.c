#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	char dir[40];

	if(announce("tcp!*!9988", dir) < 0)
		sysfatal("announce: %r");
	print("announced in %s\n", dir);
	for(;;)
		sleep(1000);
}

	