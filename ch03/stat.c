#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	Dir *d;

	d = dirstat("/NOTICE");
	if(d == nil)
		sysfatal("dirstat: %r");

	print("file name: %s\n", d->name);
	print("file mode: 0%ulo\n", d->mode);
	print("file size: %lld bytes\n", d->length);

	free(d);
	exits(nil);
}
