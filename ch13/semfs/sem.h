typedef struct Sem Sem;
typedef struct QReq QReq;

struct Sem{
	Ref;
	int		id;		// index in array; qid.path
	char		*name;	// of file
	int		tickets;
	QReq	*reqs;	// reads (i.e., downs) pending
};

struct QReq{
	QReq	*next;	// in pending request list
	Req		*r;		// the request pending
};

#define Nsems 32
extern Sem *sems[];

extern Sem *newsem(char *name);
extern void closesem(Sem *s);
extern void queuereq(Sem *s, Req *r);
extern Req *dequeuereq(Sem *s);
extern int queuedreqs(Sem *s);
