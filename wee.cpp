///////////////////////////////////////////////////////////////////////
// wee.cpp -- wee main program
// Date: Sat Sep  7 16:04:51 2013  (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <assert.h>

#include "registry.hpp"
#include "term.hpp"
#include "dispatch.hpp"
#include "bindings.hpp"
#include "buffer.hpp"
#include "view.hpp"
#include "xeq.hpp"

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
// Initialize the main_bindings
//////////////////////////////////////////////////////////////////////

static void
init_bindings() {
	main_bindings.bind("^X^C",xeq_quit);
	main_bindings.bind("^XC",xeq_quit);
	main_bindings.bind("^N",xeq_nline);
	main_bindings.bind("^P",xeq_pline);
}

//////////////////////////////////////////////////////////////////////
// Main Routine
//////////////////////////////////////////////////////////////////////

int
main(int argc,char **argv) {
	struct termios ios;
	Dispatch disp;
	Dispatch::Action action;
	keych_t keystroke;
	bindproc_t proc;
	int rc;
	
	init_bindings();

	rc = tcgetattr(0,&saved_tty);		// Save current tty settings

	assert(!rc);
	atexit(tty_cleanup);			// Restore tty settings upon exit

	term.init();
	term.clear();

	rc = tcgetattr(0,&ios);			// Get current tty settings
	ios.c_lflag &= ~ISIG;			// No signal generation
	rc = tcsetattr(0,TCSAFLUSH,&ios);

	(void) new View(term);			// Create one view
	View& main = View::focus();		// Get the focus view
	Buffer mbuf("main");

	mbuf.read_file("wee.cpp");

	{
		Cursor *csr = mbuf.new_cursor();
		main.associate(*csr);
		delete csr;
	}

	for (;;) {
		View::refresh();
		keystroke = term.get();
		if ( (action = disp.dispatch(keystroke,proc,main_bindings)) == Dispatch::Exec && proc != 0 ) {
			term.mvclear_botline();
			int prefix = disp.get_prefix();
			bool have_prefix = disp.had_prefix();
			proc(prefix,have_prefix);
		} else if ( action == Dispatch::Failed ) {
			term.bottomf("[Key not bound]");
			term.flash();
		} else	{
			assert(action == Dispatch::More);
			std::string prefix, path;

			term.mvclear_botline();
			disp.get_pending(prefix,path);
			if ( prefix.size() != 0 ) {
				term.bottomf("%s %s",prefix.c_str(),path.c_str());
			} else	{
				term.bottomf("Key: %s",path.c_str());
			}
		}
	}

	return 0;
}

// End wee.cpp
