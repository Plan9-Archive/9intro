#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	char buffer[10];
	int nr, fd;

	fd = open("/NOTICE", OREAD);
	if(fd < 0)
		sysfatal("open: %r");

	while((nr = read(fd, buffer, sizeof buffer)) > 0)
		if(write(1, buffer, nr) != nr)
			sysfatal("write: %r");
	if(nr < 0)
		sysfatal("read: %r");

	exits(nil);
}
