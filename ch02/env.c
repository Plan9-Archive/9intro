#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	char *home;

	home = getenv("home");
	if(home == nil)
		sysfatal("we are homeless");
	print("home: %s\n", home);

	exits(nil);
}

