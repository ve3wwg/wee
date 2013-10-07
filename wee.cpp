//////////////////////////////////////////////////////////////////////
// wee.cpp -- wee main program
// Date: Sat Sep  7 16:04:51 2013  (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <assert.h>

#include "term.hpp"
#include "dispatch.hpp"
#include "bindings.hpp"

Terminal term;
Key_Bindings main_bindings;

static struct termios saved_tty;

//////////////////////////////////////////////////////////////////////
// Cleanup routine 
//////////////////////////////////////////////////////////////////////

static void
tty_cleanup() {
	term.fini();
	tcsetattr(0,TCSAFLUSH,&saved_tty);
}

//////////////////////////////////////////////////////////////////////
// Exit the editor
//////////////////////////////////////////////////////////////////////

static void
xeq_quit(int prefix,bool have_prefix) {
	(void) prefix;
	(void) have_prefix;
	exit(0);
}

//////////////////////////////////////////////////////////////////////
// Initialize the main_bindings
//////////////////////////////////////////////////////////////////////

static void
init_bindings() {
	main_bindings.bind("^X^C",xeq_quit);
}

//////////////////////////////////////////////////////////////////////
// Main Routine
//////////////////////////////////////////////////////////////////////

int
main(int argc,char **argv) {
	Dispatch disp;
	Dispatch::Action action;
	keych_t keystroke;
	bindproc_t proc;
	int rc;
	
	rc = tcgetattr(0,&saved_tty);		// Save current tty settings
	assert(!rc);
	atexit(tty_cleanup);			// Restore tty settings upon exit

	init_bindings();

	term.init();
	term.clear();

	for (;;) {
		keystroke = term.get();
		if ( (action = disp.dispatch(keystroke,proc,main_bindings)) == Dispatch::Exec && proc != 0 ) {
			int prefix = disp.get_prefix();
			bool have_prefix = disp.had_prefix();
			proc(prefix,have_prefix);
		} else if ( action == Dispatch::Failed ) {
			term.flash();
		}
	}

	return 0;
}

// End wee.cpp
