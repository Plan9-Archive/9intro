#include <u.h>
#include <libc.h>

static void
usage(void)
{
	fprint(2, "usage: %s [-b bufsz] infile outfile\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	char *buf;
	long nr, bufsz;
	int infd, outfd;

	bufsz = 8 * 1024;

	ARGBEGIN{
		case 'b':
			bufsz = atoi(EARGF(usage()));
			break;
		default:
			usage();
	}ARGEND;

	if(argc != 2)
		usage();

	buf = malloc(bufsz);
	if(buf == nil)
		sysfatal("no more memory");

	infd = open(argv[0], OREAD);
	if(infd < 0)
		sysfatal("%s: %s: %r", argv0, argv[0]);

	outfd = create(argv[1], OWRITE, 0664);
	if(outfd < 0)
		sysfatal("%s: %s: %r", argv0, argv[1]);

	while((nr = read(infd, buf, bufsz)) > 0)
		write(outfd, buf, nr);
	if(nr < 0)
		sysfatal("%s: %s: %r", argv0, argv[0]);

	close(infd);
	close(outfd);
	exits(nil);
}

	
	