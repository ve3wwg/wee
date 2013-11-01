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

viewid_t				View::next_id = 1;
std::unordered_map<viewid_t,View*>	View::views_map;


View::View(Terminal& term) {
	id = next_id++;			// View ID
	this->term = &term;		// Keep ref to our physical screen
	top = 0;			// No cursor yet
	views_map[id] = this;
}

View::~View() {
	views_map.erase(id);
	id = 0;
}

void
View::disassociate(regid_t bufid) {
	return;		// FIX ME FINISH ME
}

void
View::associate(const Cursor& model) {
}

//////////////////////////////////////////////////////////////////////
// Static Methods
//////////////////////////////////////////////////////////////////////

void
View::buffer_destroyed(regid_t bufid) {
	for ( auto it = views_map.begin(); it != views_map.end(); ++it ) {
		View *view = it->second;
		view->disassociate(bufid);
	}
}

// End view.cpp
