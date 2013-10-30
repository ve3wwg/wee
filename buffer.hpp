//////////////////////////////////////////////////////////////////////
// buffer.hpp -- Emacs buffer classes
// Date: Sun Oct 27 21:18:42 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "types.hpp"
#include "registry.hpp"

#include <unordered_map>

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
	~Buffer();

	const std::string& name() const;

	static Buffer *lookup(regid_t id);		// Locate a buffer by ID
	static Buffer *lookup(const std::string& name);	// Locate a buffer by name
};

extern Registry buffer_registry;

#endif // BUFFER_HPP

// End buffer.hpp
