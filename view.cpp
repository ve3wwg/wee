//////////////////////////////////////////////////////////////////////
// view.cpp -- View class implementation
// Date: Wed Oct 30 22:54:08 2013
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "view.hpp"

View::View(Terminal& term) {
	this->term = &term;		// Keep ref to our physical screen
	top = 0;			// No cursor yet
}

View::~View() {
}

void
View::associate(const Cursor& model) {
}

// End view.cpp
