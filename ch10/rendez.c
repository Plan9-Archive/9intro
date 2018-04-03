#include <u.h>
#include <libc.h>

int httpinit(void);
void httpservice(void);

void
main(int, char *[])
{
	int	childsts;

	switch(rfork(RFPROC|RFNOTEG|RFNOWAIT)){
	case -1:
		sysfatal("rfork: %r");
	case 0:
		if(httpinit() < 0)
			rendezvous(main, (void *)-1);
		else
			rendezvous(main, (void *)0);
		httpservice();	// do the job
		exits(nil);
	default:
		childsts = (int)rendezvous(main, (void *)0);
		if(childsts == 0)
			exits(nil);
		else
			sysfatal("httpinit failed");
	}
}

void
httpservice(void)
{
	sleep(60 * 1000);
}

int
httpinit(void)
{
	sleep(2 * 1000);
	return -1;
}
