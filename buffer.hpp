//////////////////////////////////////////////////////////////////////
// buffer.hpp -- Emacs buffer classes
// Date: Sun Oct 27 21:18:42 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "types.hpp"
#include "registry.hpp"
#include "pathnames.hpp"

#include <unordered_map>
#include <vector>

class Buffer;				// Forward declaration

typedef std::vector<std::string> Region;

typedef long		csrid_t;

class Cursor {
	csrid_t		csrid;		// ID of this cursor
	regid_t		bufid;		// Buffer ID
	lineno_t	lno;		// Line no.
	colno_t		col;		// Column no. within the line

	static std::unordered_map<regid_t,Cursor*> cursors_map;
	static csrid_t				   next_id;

public:	Cursor(const char *bufname,lineno_t lno,colno_t col);
	~Cursor();
	
	inline csrid_t id()		{ return csrid; }
	inline Buffer *buffer();
	inline lineno_t line()		{ return lno; }
	inline colno_t column()		{ return col; }

	// Static methods
	static Cursor *lookup(csrid_t id);
};



class Buffer {
	regid_t		bufid;		// Buffer's ID value 
	std::string	errmsg;		// Last deposited error message
	Pathname	pathname;	// Pathname of file loaded

	std::vector<std::string> content;

	static std::unordered_map<regid_t,Buffer*> buffers_map;
	static Registry buffer_registry;

protected:
	void init(const char *bufname);

public:	Buffer();
	Buffer(const char *name);
	~Buffer();

	const std::string& name() const;
	inline const std::string& error() const { return errmsg; }

	bool read_file(const std::string& pathname);

	void dump();

	// Static Methods :

	static Buffer *lookup(regid_t id);		// Locate a buffer by ID
	static regid_t get_id(const std::string& name);	// Lookup buffer's ID
	static Buffer *lookup(const std::string& name);	// Locate a buffer by name
};


#endif // BUFFER_HPP

// End buffer.hpp
