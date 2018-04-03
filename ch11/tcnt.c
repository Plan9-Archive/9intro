#include <u.h>
#include <libc.h>
#include <thread.h>

int	cnt;

void
incr(void *arg)
{
	int *cp;

	cp = arg;
	threadsetname("incrthread");
	for(;;){
		*cp += 1;
		print("cnt %d\n", *cp);
		yield();
	}
}

void
decr(void *arg)
{
	int *cp;

	cp = arg;
	threadsetname("decrthread");
	for(;;){
		if(*cp > 0)
			*cp -= 1;
		print("cnt %d\n", *cp);
		yield();
	}
}

void
threadmain(int, char *[])
{
	threadsetname("main");
	threadcreate(incr, &cnt, 8 * 1024);
	threadcreate(decr, &cnt, 8 * 1024);
	threadexits(nil);
}
