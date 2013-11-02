//////////////////////////////////////////////////////////////////////
// cursor.hpp -- Cursor Class
// Date: Fri Nov  1 21:20:56 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "types.hpp"

#include <unordered_map>

class Buffer;

class Cursor {
	csrid_t		csrid;		// ID of this cursor
	regid_t		bufid;		// Buffer ID
	lineno_t	lno;		// Line no.
	colno_t		col;		// Column no. within the line
	bool		end_file;	// True if this cursor follows end of buffer

	typedef std::unordered_map<regid_t,Cursor*> csrmap_t;		// Map of cursors

	static csrmap_t				   cursors_map;		// csrid => Cursor *
	static csrid_t				   next_id;
	static std::unordered_map<regid_t,csrmap_t> buffers_map;	// Bufid => cursors map

protected:
	void register_cursor();
	void unregister_cursor();

public:	Cursor();
	Cursor(const Cursor& csr);
	Cursor(regid_t,lineno_t lno=0,colno_t col=0);
	Cursor(const char *bufname,lineno_t lno,colno_t col);
	~Cursor();
	
	inline csrid_t id()		{ return csrid; }
	Buffer *buffer();
	inline lineno_t line()		{ return lno; }
	inline colno_t column()		{ return col; }

	void reassociate(Buffer *buf);

	// Static methods
	static Cursor *lookup(csrid_t id);
	static void destroyed(regid_t bufid);
	static void reloaded(regid_t bufid);
};

#endif // CURSOR_HPP

// End cursor.hpp
