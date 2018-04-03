#include <u.h>
#include <libc.h>

enum { Nmsgs = 4 };

typedef struct Buffer  Buffer;
struct Buffer {
	QLock	lck;
	char		*msgs[Nmsgs];		// messages in buffer
	int		hd;				// head of the queue
	int		tl;				// tail -- First empty slot
	int		nmsgs;			// number of messages in buffer
	Rendez	isfull;			// to sleep because of no roo for put
	Rendez	isempty;			// to sleep when nothing to get
};

void
put(Buffer *b, char *msg)
{
	qlock(&b->lck);
	if(b->nmsgs == Nmsgs)
		rsleep(&b->isfull);
	b->msgs[b->tl] = strdup(msg);
	b->tl = ++b->tl % Nmsgs;
	if(b->nmsgs++ == 0)
		rwakeup(&b->isempty);
	qunlock(&b->lck);
}

char *
get(Buffer *b)
{
	char *msg;

	qlock(&b->lck);
	if(b->nmsgs == 0)
		rsleep(&b->isempty);
	msg = b->msgs[b->hd];
	b->hd = ++b->hd % Nmsgs;
	if(b->nmsgs-- == Nmsgs)
		rwakeup(&b->isfull);
	qunlock(&b->lck);
	return msg;
}

void
init(Buffer *b)
{
	// release all locks, set everthing to null values
	memset(b, 0, sizeof(*b));
	// set the locks used by the Rendezes
	b->isempty.l = &b->lck;
	b->isfull.l = &b->lck;
}

void
producer(Buffer *b, char id)
{
	char	msg[20];
	int i;

	for(i = 0; i < 5; i++){
		seprint(msg, msg+20, "%c%d", id, i);
		put(b, msg);
	}

	msg[0] = 0;
	put(b, msg);

	exits(nil);
}

void
consumer(Buffer *b)
{
	char *msg;

	while(strlen(msg = get(b))){
		print("%s ", msg);
		free(msg);
	}
	exits(nil);
}

Buffer buf;

void
main(int, char *[])
{
	init(&buf);
	if(rfork(RFPROC|RFMEM|RFNOWAIT) == 0)
		producer(&buf, 'a');
	if(rfork(RFPROC|RFMEM|RFNOWAIT) == 0)
		producer(&buf, 'b');
	if(rfork(RFPROC|RFMEM|RFNOWAIT) == 0)
		consumer(&buf);
	else
		consumer(&buf);
}
