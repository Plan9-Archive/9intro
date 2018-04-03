#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	int	lfd, dfd;
	long	nr;
	char	adir[40], ldir[40], buf[1024];

	if(announce("tcp!*!9988", adir) < 0)
		sysfatal("announce: %r");
	print("announced tcp!*!9988 in %s\n", adir);
	for(;;){
		lfd = listen(adir, ldir);
		if(lfd < 0)
			sysfatal("listen: %r");

		dfd = accept(lfd, ldir);
		if(dfd < 0)
			sysfatal("can't accept: %r");

		close(lfd);
		print("accepted call at %s\n", ldir);

		// do the job
		while((nr = read(dfd, buf, sizeof buf)) > 0)
			write(dfd, buf, nr);

		print("terminated call at %s\n", ldir);
		close(dfd);
	}
}
