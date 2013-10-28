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

#include <sstream>

Cursor::Cursor(const char *bufname,lineno_t lno,colno_t col) {
	this->bufid = buffer_registry.lookup(bufname);
	assert(bufid);

	this->lno = lno;
	this->col = col;
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
	regid_t id;

	(void)id;

	if ( !bufname ) {
		std::stringstream s;

		s << "$buf" << bufid;
		id = buffer_registry.create(s.str());
		assert(id == bufid);
	} else	{
		bufid = id = buffer_registry.create(bufname);
		assert(id != 0);
	}
}

const std::string&
Buffer::name() const {
	return buffer_registry.reverse(bufid);
}

// End buffer.cpp
