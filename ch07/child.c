#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	switch(rfork(RFPROC|RFFDG|RFREND|RFNOWAIT)){
	case -1:
		sysfatal("fork failed");
	case 0:
		print("I'm the child\n");
		break;
	default:
		print("I'm the parent\n");
	}
	exits(nil);
}
