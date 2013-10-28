//////////////////////////////////////////////////////////////////////
// buffer.hpp -- Emacs buffer classes
// Date: Sun Oct 27 21:18:42 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "types.hpp"
#include "registry.hpp"


class Cursor {
	regid_t		bufid;		// Buffer ID
	lineno_t	lno;		// Line no.
	colno_t		col;		// Column no. within the line

public:	Cursor(const char *bufname,lineno_t lno,colno_t col);
	
};


class Buffer {
	regid_t		bufid;		// Buffer's ID value 

protected:
	void init(const char *bufname);

public:	Buffer();
	Buffer(const char *name);
	const std::string& name() const;
};

extern Registry buffer_registry;

#endif // BUFFER_HPP

// End buffer.hpp
