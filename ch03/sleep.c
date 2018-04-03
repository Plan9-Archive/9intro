#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	print("process pid is %d, have fun.\n", getpid());
	sleep(600 * 1000); // ten minutes to play
	exits(nil);
}
