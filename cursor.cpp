//////////////////////////////////////////////////////////////////////
// cursor.cpp -- Cursor class implementation
// Date: Fri Nov  1 21:22:27 2013
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "cursor.hpp"
#include "buffer.hpp"


Cursor::csrmap_t			Cursor::cursors_map;		// csrid => Cursor *
std::unordered_map<regid_t,Cursor::csrmap_t> Cursor::buffers_map;	// Bufid => cursors map
regid_t				   	Cursor::next_id = 100;


Cursor::Cursor() {
	csrid = next_id++;
	bufid = 0;
	lno = 0;
	col = 0;
	end_file = 0;

	register_cursor();
}

Cursor::Cursor(const Cursor& csr) {
	(*this) = csr;
	this->csrid = next_id++;
	register_cursor();
}

Cursor::Cursor(const char *bufname,lineno_t lno,colno_t col) {

	this->csrid = next_id++;
	this->bufid = Buffer::lookup_id(bufname);
	assert(bufid);

	this->lno = lno;
	this->col = col;

	Buffer *buf = Buffer::lookup(this->bufid);
	size_t lines = buf->length();

	if ( this->lno >= lines ) {
		this->lno = lines + 1;
		end_file = true;
	} else	end_file = false;

	register_cursor();
}

Cursor::Cursor(regid_t bufid,lineno_t lno,colno_t col) {

	csrid = next_id++;
	this->bufid = bufid;
	this->lno = lno;
	this->col = col;
	end_file = false;
	register_cursor();
}

Cursor::~Cursor() {
	unregister_cursor();
}

void
Cursor::register_cursor() {
	cursors_map[csrid] = this;
	csrmap_t& map = buffers_map[bufid];
	map[csrid] = this;
}

void
Cursor::unregister_cursor() {
	csrmap_t& map = buffers_map[bufid];
	map.erase(csrid);
	cursors_map.erase(csrid);
}

// Disassociate from all buffers

void
Cursor::disassociate() {
	csrmap_t& omap = buffers_map[bufid];
	omap.erase(csrid);
	if ( omap.size() == 0 )
		buffers_map.erase(bufid);

	bufid = 0;
	csrmap_t nmap = buffers_map[bufid];
	nmap[csrid] = this;
}

// Reassociate cursor to new buffer

void
Cursor::reassociate(Buffer *buf) {
	csrmap_t& omap = buffers_map[bufid];
	omap.erase(csrid);
	if ( omap.size() == 0 )
		buffers_map.erase(bufid);

	bufid = buf->get_id();
	csrmap_t nmap = buffers_map[bufid];
	nmap[csrid] = this;
}

Buffer *
Cursor::buffer() {
	return Buffer::lookup(bufid);
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

// Buffer bufid was/is destroyed

void
Cursor::destroyed(regid_t bufid) {
	csrmap_t& omap = buffers_map[bufid];
	csrmap_t& zmap = buffers_map[0];

	for ( auto it = omap.begin(); it != omap.end(); ++it ) {
		Cursor *csr = it->second;
		csr->bufid = 0;
		zmap[csr->bufid] = csr;
	}
	buffers_map.erase(bufid);
}

// Buffer was reloaded with new content

void
Cursor::reloaded(regid_t bufid) {
	csrmap_t& map = buffers_map[bufid];

	for ( auto it = map.begin(); it != map.end(); ++it ) {
		Cursor *csr = it->second;
		if ( csr->end_file ) {
			Buffer *buf = Buffer::lookup(bufid);
			size_t lines = buf->length();
			csr->lno = lines+1;
		} else	csr->lno = 0;
	}
}

// End cursor.cpp
