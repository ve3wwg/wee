//////////////////////////////////////////////////////////////////////
// wee.cpp -- wee main program
// Date: Sat Sep  7 16:04:51 2013
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <assert.h>

#include "term.hpp"

Terminal term;

static struct termios saved_tty;

static void
tty_cleanup() {
	term.fini();
	tcsetattr(0,TCSAFLUSH,&saved_tty);
}

int
main(int argc,char **argv) {
	int rc;
	
	rc = tcgetattr(0,&saved_tty);		// Save current tty settings
	assert(!rc);
	atexit(tty_cleanup);			// Restore tty settings upon exit

	term.init();
	term.clear();
	term.get();

	return 0;
}

// End wee.cpp
