#include <u.h>
#include <libc.h>
#include <thread.h>
#include <draw.h>
#include <mouse.h>
#include <keyboard.h>

Channel		*sliderc;
Mousectl		*mctl;
Keyboardctl	*kctl;

void
mousethread(void *arg)
{
	Mousectl *mctl;
	Mouse m;
	int dx, val;

	mctl = arg;
	for(;;){
		recv(mctl->c, &m);
		if(m.buttons == 1){
			do{
				dx = m.xy.x - screen->r.min.x;
				val = dx * 100 / Dx(screen->r);
				sendul(sliderc, val);
				recv(mctl->c, &m);
			}while(m.buttons == 1);
		}
	}
}

void
resizethread(void *arg)
{
	Mousectl	*mctl;

	mctl = arg;
	for(;;){
		recvul(mctl->resizec);
		if(getwindow(display, Refnone) < 0)
			sysfatal("getwindow: %r");
		sendul(sliderc, ~0);	// Any number not in 0..100
	}
}

void
drawslider(int val)
{
	Rectangle setrect, unsetrect, markrect;
	int dx;
	Image *setcol, *unsetcol;

	setcol = allocimage(display, Rect(0, 0, 1, 1), screen->chan, 1, DYellow);
	unsetcol = allocimage(display, Rect(0, 0, 1, 1), screen->chan, 1, 0x777777FF);

	dx = Dx(screen->r) * val / 100;
	setrect = screen->r;
	unsetrect = screen->r;
	markrect = screen->r;

	setrect.max.x = setrect.min.x + dx;
	markrect.min.x = setrect.max.x;
	markrect.max.x = setrect.max.x + 2;
	unsetrect.min.x = markrect.max.x;
	
	draw(screen, setrect, setcol, nil, ZP);
	draw(screen, unsetrect, unsetcol, nil, ZP);
	draw(screen, markrect, display->black, nil, ZP);

	flushimage(display, 1);
}

void
sliderthread(void *)
{
	uint val, nval;

	val = ~0;
	for(;;){
		nval = recvul(sliderc);
		if(nval <= 100){
			if(nval == val)
				continue;
			val = nval;
		}
		drawslider(val);
		print("%d\n", val);
	}
}

void
terminate(void)
{
	closekeyboard(kctl);
	closemouse(mctl);
	closedisplay(display);
	threadexitsall(nil);
}

void
keyboardthread(void *a)
{
	Keyboardctl *kctl;
	Rune	r, rr;
	int nval;

	kctl = a;
	for(;;){
		recv(kctl->c, &r);
		switch(r){
		case Kdel:
		case Kesc:
		case 'q':
			terminate();
			break;
		default:
			if(utfrune("0123456789", r) != nil){
				recv(kctl->c, &rr);
				if(utfrune("0123456789", rr) != nil){
					nval = (r - '0') * 10 + (rr - '0');
					sendul(sliderc, nval);
				}
			}
		}
	}
}

void
threadmain(int, char *argv[])
{
	mctl = initmouse("/dev/mouse", nil);
	if(mctl == nil)
		sysfatal("initmouse: %r");

	kctl = initkeyboard("/dev/cons");
	if(kctl == nil)
		sysfatal("initkeyboard: %r");

	if(initdraw(nil, nil, argv[0]) < 0)
		sysfatal("initdraw: %r");

	sliderc = chancreate(sizeof(ulong), 0);

	threadcreate(resizethread, mctl, 8 * 1024);
	threadcreate(mousethread, mctl, 8 * 1024);
	threadcreate(keyboardthread, kctl, 8 * 1024);
	threadcreate(sliderthread, sliderc, 8 * 1024);

	sendul(sliderc, 50);
	threadexits(nil);
}
