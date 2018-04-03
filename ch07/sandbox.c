#include <u.h>
#include <libc.h>
#include <auth.h>

void
main(int argc, char *argv[])
{
	char *user;

	if(argc != 3){
		fprint(2, "usage: %s ns prog\n", argv0);
		exits("usage");
	}

	switch(rfork(RFPROC|RFNAMEG|RFNOMNT)){
	case -1:
		sysfatal("fork: %r");
	case 0:
		user = getuser();
		if(newns(user, argv[1]) < 0)
			sysfatal("newns: %r");
		execl(argv[2], argv[2], nil);
		sysfatal("exec: %r");
	default:
		waitpid();
		exits(nil);
	}
}
