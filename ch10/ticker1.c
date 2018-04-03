#include <u.h>
#include <libc.h>

enum { Npanels = 2 };

typedef struct Msg Msg;
struct Msg {
	RWLock	lck;		// to protect the other fields from races
	char		*text;	// for the message
	ulong	vers;		// for the message version
};

Msg	msg;
int	exiting;

void
reader(void)
{
	char	buf[512];
	int nr;

	while((nr = read(0, buf, sizeof(buf) -1)) > 0){
		buf[nr] = 0;
		wlock(&msg.lck);
		free(msg.text);
		msg.text = strdup(buf);
		msg.vers++;
		wunlock(&msg.lck);
	}
	exiting = 1;
	exits(nil);
}

void
panelproc(int fd)
{
	ulong lastvers;
	char *text;

	lastvers = -1;
	text = nil;

	do{
		rlock(&msg.lck);
		if(msg.text != nil && lastvers != msg.vers){
			text = strdup(msg.text);
			lastvers = msg.vers;
		}
		runlock(&msg.lck);
		if(text != nil){
			write(fd, text, strlen(text));
			free(text); 
			text = nil;
		}
		sleep(5 * 1000);
	}while(!exiting);
	fprint(2, "panel exiting\n");
	exits(nil);
}

void
main(int, char *[])
{
	int i;

	for(i = 0; i < Npanels; i++)
		if(rfork(RFPROC|RFMEM|RFNOWAIT) == 0)
			panelproc(1);
	reader();
	/* does not return */
}
