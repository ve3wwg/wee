//////////////////////////////////////////////////////////////////////
// buffer.cpp -- Buffer Clases Implementation
// Date: Sun Oct 27 21:24:03 2013
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "buffer.hpp"
#include "cursor.hpp"
#include "view.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

std::unordered_map<regid_t,Buffer*> 	Buffer::buffers_map;
Registry 				Buffer::buffer_registry;


Buffer::Buffer() {
	Buffer::init(0);
}

Buffer::Buffer(const char *bufname) {
	init(bufname);
}

void
Buffer::init(const char *bufname) {
	if ( bufname ) {
		bufid = buffer_registry.lookup(bufname); // Does this name already exist?
		if ( bufid != 0 )
			bufname = 0;			// Make a new buffer name up
		else	bufid = buffer_registry.create(bufname);// Register buffer name
	}

	if ( !bufname ) {
		std::stringstream s;

		s << "$buf" << bufid;
		bufid = buffer_registry.create(s.str()); // Returns 0 if the buffer exists
	}

	buffers_map[bufid] = this;			// Register this new buffer
}

Buffer::~Buffer() {
	View::buffer_destroyed(bufid);			// Disassociate from all views
	Cursor::destroyed(bufid);			// Tell cursors about this buffer's demise
	buffers_map.erase(bufid);			// Remove this buffer from list of all bufs
}

Cursor *
Buffer::new_cursor() {
	return new Cursor(bufid);
}

const std::string&
Buffer::name() const {
	return buffer_registry.reverse(bufid);
}

// Read a file into this buffer

bool
Buffer::read_file(const std::string& pathname) {
	this->pathname.import(pathname.c_str());

	std::string path = this->pathname.pathname();
	std::fstream is(path.c_str(),std::fstream::in);

	if ( !is.good() ) {
		errmsg = strerror(errno);
		return false;
	}

	content.clear();

	while ( is.good() ) {
		std::string line;
		getline(is,line);
		content.push_back(line);
	}

	is.close();
	errmsg = "";

	Cursor::reloaded(bufid);
	return true;
}

void
Buffer::get_line(std::string& text,lineno_t lno) {

	if ( lno < content.size() ) {
		text = content[lno];
	} else	{
		assert(lno == content.size());
		text.clear();
	}
}

void
Buffer::dump() {
	unsigned lno = 0;

	std::cout << "Buffer '" << name() << "' dump:\n";

	for ( auto it=content.begin(); it != content.end(); ++it, ++lno ) {
		std::cout << lno << " : " << *it << "\n";
	}
}

//////////////////////////////////////////////////////////////////////
// Static Methods for Buffer
//////////////////////////////////////////////////////////////////////

Buffer *
Buffer::lookup(regid_t id) {
	auto it = Buffer::buffers_map.find(id);
	if ( it == Buffer::buffers_map.end() )
		return 0;		// Not found
	return it->second;
}

regid_t
Buffer::lookup_id(const std::string& name) {
	return buffer_registry.lookup(name);
}

Buffer *
Buffer::lookup(const std::string& name) {
	regid_t id = Buffer::lookup_id(name);
	if ( !id )
		return 0;
	return Buffer::lookup(id);
}

// End buffer.cpp
