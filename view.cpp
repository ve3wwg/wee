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
	offset = 0;
	width = term.get_cols();	// By default, assume full width of terminal
}

View::~View() {
	views_map.erase(id);
	id = 0;
}

void
View::disassociate(regid_t bufid) {
	if ( bufid != top.get_bufid() )
		return;		// Does not affect this cursor
	top.disassociate();	// No longer associated with a buffer
}

void
View::associate(const Cursor& bufref) {
	top = bufref;
}

void
View::fetch_line(std::string& text,int tline) {
	text.clear();

	if ( top.get_bufid() ) {
		Buffer *buf = top.buffer();
		if ( buf ) {
			lineno_t lines = buf->length();	// # of lines in buffer
			lineno_t curline = top.line();
			if ( curline < lines ) {
				std::string temp;
				buf->get_line(temp,curline);

				if ( !offset || offset >= text.size() ) {
					text = temp;
				} else	{
					text = temp.substr(offset);
				}
			}
		}
	}

	if ( text.size() < width )
		text.append(' ',width - text.size());
	if ( text.size() > width )
		text.resize(width);
	assert(text.size() == width);
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
