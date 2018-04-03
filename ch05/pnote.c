#include <u.h>
#include <libc.h>

int
handler(void *, char *msg)
{
	print("note: %s\n", msg);
	return 1;
}

void
main(int, char *[])
{
	atnotify(handler, 1);
	sleep(600 * 1000); // ten minutes to play
	print("done (%r)\n");
	exits(nil);
}
