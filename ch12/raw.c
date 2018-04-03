#include <u.h>
#include <libc.h>

void
usage(void)
{
	fprint(2, "usage: %s [-r]\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	char	buf[512];
	int raw, cfd, nr;

	raw = 0;
	cfd = -1;

	ARGBEGIN{
	case 'r':
		raw++;
		break;
	default:
		usage();
	}ARGEND;

	if(argc != 0)
		usage();

	if(raw){
		cfd = open("/dev/consctl", OWRITE);
		write(cfd, "rawon", 5);
	}

	while((nr = read(0, buf, sizeof(buf) - 1)) > 0){
		buf[nr] = 0;
		print("[%s]\n", buf);
	}
	if(raw)
		close(cfd);
	exits(nil);
}
