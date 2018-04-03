#include <u.h>
#include <libc.h>

void
usage(void)
{
	fprint(2, "usage: %s [-nv] [-d delims] args\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	int nflag, vflag, i;
	char *delims = "[]";
	
	nflag = 0;
	vflag = 0;

	ARGBEGIN{
		case 'v':
			vflag = 1;
			break;
		case 'n':
			nflag = 1;
			break;
		case 'd':
			delims = ARGF();
			if(delims == nil || strlen(delims) < 2)
				usage();
			break;
		default:
			usage();
	}ARGEND;

	for(i = 0; i < argc; i++)
		if(vflag)
			print("%c%s%c ", delims[0], argv[i], delims[1]);
		else
			print("%s ", argv[i]);

	if(!nflag)
		print("\n");

	exits(nil);
}
