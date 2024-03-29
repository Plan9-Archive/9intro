#include <u.h>
#include <libc.h>

int		cnt;
QLock	cntlck;

void
main(int, char *[])
{
	switch(rfork(RFPROC|RFMEM|RFNOWAIT)){
	case -1:
		sysfatal("fork: %r");
	case 0:
		for(;;){
			qlock(&cntlck);
			assert(cnt >= 0);
			cnt++;
			print("%d\n", cnt);
			qunlock(&cntlck);
		}
	default:
		for(;;){
			qlock(&cntlck);
			assert(cnt >= 0);
			if(cnt > 0)
				cnt--;
			print("%d\n", cnt);
			qunlock(&cntlck);
		}
	}
}
