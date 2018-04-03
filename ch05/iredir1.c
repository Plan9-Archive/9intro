#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	int fd;

	switch(fork()){
	case -1:
		sysfatal("fork failed");
	case 0:
		fd = open("/NOTICE", OREAD);
		if(fd < 0)
			sysfatal("open: %r");
		if(dup(fd, 0) < 0)
			sysfatal("dup: %r");
		close(fd);
		execl("/bin/cat", "cat", nil);
		sysfatal("exec: %r");
	default:
		waitpid();
	}
	exits(nil);
}
