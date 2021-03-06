//////////////////////////////////////////////////////////////////////
// xeq.cpp -- Execution functions for key bindings
// Date: Tue Nov  5 20:58:18 2013
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "view.hpp"
#include "xeq.hpp"

//////////////////////////////////////////////////////////////////////
// Exit the editor
//////////////////////////////////////////////////////////////////////

bool
xeq_quit(int prefix,bool have_prefix) {
	(void) prefix;
	(void) have_prefix;
	exit(0);
	return true;
}

//////////////////////////////////////////////////////////////////////
// Next Line
//////////////////////////////////////////////////////////////////////

bool
xeq_nline(int prefix,bool have_prefix) {
	View& main = View::focus();
	Cursor& point = main.get_point();

	if ( !have_prefix )
		prefix = 1;
	return point.nline(prefix);
}

//////////////////////////////////////////////////////////////////////
// Previous Line
//////////////////////////////////////////////////////////////////////

bool
xeq_pline(int prefix,bool have_prefix) {
	View& main = View::focus();
	Cursor& point = main.get_point();

	if ( !have_prefix )
		prefix = 1;
	return point.pline(prefix);
}

// End xeq.cpp
