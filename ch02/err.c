#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	if(chdir("magic") < 0){
		fprint(2, "chdir failed: %r\n");
		exits("failed");
	}

	/* ... do other things ... */
	exits(nil);
}
