#include <u.h>
#include <libc.h>

void
main(int argc, char *argv[])
{
	int nflag, vflag, i;

	nflag = 0;
	vflag = 0;

	ARGBEGIN{
		case 'v':
			vflag = 1;
			break;
		case 'n':
			nflag = 1;
			break;
		default:
			fprint(2, "usage: %s [-nv] args\n", argv0);
			exits("usage");
	}ARGEND;

	for(i = 0; i < argc; i++)
		if(vflag)
			print("[%s] ", argv[i]);
		else
			print("%s ", argv[i]);

	if(!nflag)
		print("\n");

	exits(nil);
}
