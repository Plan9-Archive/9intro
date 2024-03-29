#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	char	buf[512];
	char	out[UTFmax];
	Rune	r;
	int nr, irl, orl;
	char	*s;

	while((nr = read(0, buf, sizeof buf)) > 0){
		s = buf;
		while(nr > 0){
			irl = chartorune(&r, s);
			s += irl;
			nr -= irl;
			r = toupperrune(r);
			orl = runetochar(out, &r);
			write(1, out, orl);
		}
	}
	exits(nil);
}
