#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	int fd[2];
	char buf[128];
	int nr;

	if(pipe(fd) < 0)
		sysfatal("Can't create a pipe: %r");
	write(fd[1], "Hello!\n", 7);
	write(fd[1], "there\n", 6);
	nr = read(fd[0], buf, sizeof buf);
	write(1, buf, nr);
	exits(nil);
}
