#include <u.h>
#include <libc.h>
#include <bio.h>

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
	Biobuf *bin, *bout;

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

	bin = Bopen(argv[0], OREAD);
	if(bin == nil)
		sysfatal("%s: %s: %r", argv0, argv[0]);

	bout = Bopen(argv[1], OWRITE);
	if(bout == nil)
		sysfatal("%s: %s: %r", argv0, argv[1]);

	while((nr = Bread(bin, buf, bufsz)) > 0)
		Bwrite(bout, buf, nr);
	if(nr < 0)
		sysfatal("%s: %s: %r", argv0, argv[1]);

	Bterm(bin);
	Bterm(bout);
	exits(nil);
}
