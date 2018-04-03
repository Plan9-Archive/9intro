#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	int fd[2];
	int srvfd, nr;
	char buf[128];

	if(pipe(fd) < 0)
		sysfatal("pipe: %r");

	srvfd = create("/srv/echo", OWRITE, 0664);
	if(srvfd < 0)
		sysfatal("can't create at /srv: %r");

	if(fprint(srvfd, "%d", fd[1]) <0)
		sysfatal("can't post file descriptor: %r");

	close(fd[1]);
	while((nr = read(fd[0], buf, sizeof buf)) > 0)
		write(1, buf, nr);

	print("exiting...\n");
	exits(nil);
}

