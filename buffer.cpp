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

#include <iostream>
#include <sstream>
#include <fstream>

std::unordered_map<regid_t,Cursor*> 	Cursor::cursors_map;
regid_t				   	Cursor::next_id = 100;

std::unordered_map<regid_t,Buffer*> 	Buffer::buffers_map;
Registry 				Buffer::buffer_registry;

//////////////////////////////////////////////////////////////////////
// Cursors
//////////////////////////////////////////////////////////////////////

Cursor::Cursor(const char *bufname,lineno_t lno,colno_t col) {

	this->csrid = next_id++;
	this->bufid = Buffer::get_id(bufname);
	assert(bufid);

	this->lno = lno;
	this->col = col;

	cursors_map[this->csrid] = this;
}

Cursor::~Cursor() {
	auto it = cursors_map.find(csrid);
	assert(it != cursors_map.end());
	cursors_map.erase(it);
}

//////////////////////////////////////////////////////////////////////
// Static methods for Cursors
//////////////////////////////////////////////////////////////////////

Cursor *
Cursor::lookup(csrid_t id) {

	auto it = Cursor::cursors_map.find(id);
	if ( it == Cursor::cursors_map.end() )
		return 0;		// Not found
	return it->second;
}

//////////////////////////////////////////////////////////////////////
// Buffers
//////////////////////////////////////////////////////////////////////

Buffer *
Cursor::buffer() {
 	return Buffer::lookup(bufid);
}


Buffer::Buffer() {
	Buffer::init(0);
}

Buffer::Buffer(const char *bufname) {
	init(bufname);
}

void
Buffer::init(const char *bufname) {
	bufid = buffer_registry.get_next_id();
	regid_t id = 0;

	if ( bufname ) {
		id = buffer_registry.lookup(bufname);	// Does this name already exist?
		if ( id != 0 )
			bufname = 0;			// Make a new buffer name up
	}

	if ( !bufname ) {
		std::stringstream s;

		s << "$buf" << bufid;
		id = buffer_registry.create(s.str());	// Returns 0 if the buffer exists
	}

	buffers_map[bufid] = this;			// Register this new buffer
}

Buffer::~Buffer() {
	buffers_map.erase(bufid);
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

std::cout << "Reading '" << path << "' into buffer\n";

	std::fstream is(path.c_str(),std::fstream::in);

	if ( !is.good() ) {
		errmsg = strerror(errno);
		return false;
	}

	content.clear();

	while ( is.good() ) {
		std::string line;
		getline(is,line);
std::cout << "Read: " << line << "\n";
		content.push_back(line);
	}

std::cout << "EOF\n";

	is.close();
	errmsg = "";
	return true;
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
Buffer::get_id(const std::string& name) {
	return buffer_registry.lookup(name);
}

Buffer *
Buffer::lookup(const std::string& name) {
	regid_t id = Buffer::get_id(name);
	if ( !id )
		return 0;
	return Buffer::lookup(id);
}

// End buffer.cpp
