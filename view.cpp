///////////////////////////////////////////////////////////////////////
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
View					*View::main_view = 0;


View::View(Terminal& term) {
	id = next_id++;			// View ID
	this->term = &term;		// Keep ref to our physical screen
	views_map[id] = this;
	offset = 0;
	width = term.get_cols();	// By default, assume full width of terminal
	height = term.get_lines() - 1;	// Leave bottom line for prompting

	if ( !main_view )
		main_view = this;	// First view becomes the main view

	topline = 0;			// Assume top terminal line
	left = 0;			// Assume leftmost terminal column
	statline = height - 1;		// Location of status line
	dirty = true;
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
	point.disassociate();
	dirty = true;
}

void
View::associate(const Cursor& bufref) {
	top = bufref;
	point = top;
	dirty = true;
}

void
View::draw_status() {
	std::string text;
	std::string bufname, pathname;

	Buffer *buf = top.buffer();
	if ( buf ) {
		bufname = buf->name();
		pathname = buf->get_pathname();
	}

	text.append(256,char(Terminal::acs_hline));
	text.replace(5,10," Wee-1.0 (");

	size_t mode_pos = 15;

	text.replace(mode_pos,2,") ");
	text.resize(width);

	size_t pos = mode_pos + 2 + 2;
	text[pos++] = char(Terminal::acs_rtee);

	if ( bufname.size() > 0 ) {
		size_t end = pos + bufname.size();

		if ( end >= text.size() )
			bufname.resize(text.size() - pos - 2 );
		text.replace(pos,bufname.size(),bufname);
		pos += bufname.size();
	}
	text[pos++] = char(Terminal::acs_ltee);

	pos += 2;
	text[pos++] = char(Terminal::acs_rtee);

	text.replace(pos,6,"File: ");
	pos += 6;

	if ( pos + pathname.size() > text.size() )
		pathname.resize(text.size() - pos);

	text.replace(pos,pathname.size(),pathname);
	pos += pathname.size();

	if ( pos < text.size() )
		text[pos++] = char(Terminal::acs_ltee);

	text.resize(width);
	term->mvput(statline,left,text);
}

void
View::reposition() {

	if ( point.line() < top.line() ) {
		top.set_line(point.line());
		dirty = true;
	}

	lineno_t h = point.line() - top.line();

	if ( h >= height-1 ) {
		dirty = true;
		if ( height-1 <= 3 ) {
			top.set_line(point.line()-(height-1)+1);
		} else	{
			top.set_line(point.line()-(height-1-3)+1);
		}
	}
}

void
View::draw_point() {
	assert(point.line() >= top.line());
	assert(point.line() - top.line() < height);

	lineno_t y = point.line() - top.line();
	colno_t x = point.column();

	std::string temp;
	std::vector<size_t> pos;

	Buffer *buf = top.buffer();
	buf->get_flat(temp,pos,point.line());

	if ( pos.size() <= 0 )
		x = 0;
	else if ( size_t(x) >= pos.size() )
		x = pos[pos.size() - 1];

	if ( x <= offset )
		x = 0;
	else	x -= offset;

	y += topline;
	x += left;

	term->move(y,x);
	term->refresh();
}

void
View::draw() {
	reposition();
	if ( dirty ) {
		for ( size_t vx=0; vx < height; ++vx ) {
			lineno_t y = topline + vx;
			std::string text;

			fetch_line(text,vx);
			term->mvput(y,left,text);
		}
		draw_point();
		draw_status();
		dirty = false;
	}
}

void
View::fetch_line(std::string& text,int vline) {
	text.clear();

	if ( top.get_bufid() ) {
		Buffer *buf = top.buffer();
		if ( buf ) {
			lineno_t lines = buf->length();	// # of lines in buffer
			lineno_t curline = top.line() + vline;
			if ( curline < lines ) {
				std::string temp;
				std::vector<size_t> pos;

				buf->get_flat(temp,pos,curline);

				if ( !offset || offset >= text.size() ) {
					text = temp;
				} else	{
					text = temp.substr(offset);
				}
			}
		}
	}

	if ( text.size() < width )
		text.append(width - text.size(),' ');
	else if ( text.size() > width )
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

// Return the main or focused view

View&
View::focus() {
	return *main_view;
}

void
View::refresh() {
	for ( auto it = views_map.begin(); it != views_map.end(); ++it ) {
		View *view = it->second;
		view->draw();
	}

	View& main = View::focus();
	main.draw_point();
}

// End view.cpp
