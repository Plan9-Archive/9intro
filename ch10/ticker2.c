#include <u.h>
#include <libc.h>

typedef struct Msg Msg;
struct Msg {
	QLock	lck;		// to protect the other fields from races
	Rendez	newmsg;	// to sleep waiting for a new message
	char		*text;	// for the message
};

void wmsg(Msg *m, char *newtext);
char *rmsg(Msg *m);

/* implementation */
enum { Npanels = 20 };

Msg	msg;
int	exiting;

char *
rmsg(Msg *m)
{
	char *new;

	qlock(&m->lck);
	rsleep(&m->newmsg);
	new = strdup(m->text);
	qunlock(&m->lck);
	return new;
}

void
wmsg(Msg *m, char *newtext)
{
	qlock(&m->lck);
	free(m->text);
	m->text = strdup(newtext);
	rwakeupall(&m->newmsg);
	qunlock(&m->lck);
}

void
panelproc(int fd)
{
	char *text;

	while(!exiting){
		text = rmsg(&msg);
		write(fd, text, strlen(text));
		free(text);
	}
	fprint(2, "panel exiting\n");
	exits(nil);
}

void
reader(void)
{
	char	buf[512];
	int nr;

	while((nr = read(0, buf, sizeof(buf) - 1)) > 0){
		buf[nr] = 0;
		wmsg(&msg, buf);
	}

	buf[0] = 0;
	wmsg(&msg, buf);
	exiting = 1;

	exits(nil);
}

void
main(int, char *[])
{
	int i;

	msg.newmsg.l = &msg.lck;

	for(i = 0; i < Npanels; i++)
		if(rfork(RFPROC|RFMEM|RFNOWAIT) == 0)
			panelproc(1);
	reader();
	/* does not return */
}
