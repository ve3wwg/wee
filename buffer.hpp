//////////////////////////////////////////////////////////////////////
// buffer.hpp -- Emacs buffer classes
// Date: Sun Oct 27 21:18:42 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "types.hpp"
#include "registry.hpp"
#include "pathnames.hpp"
#include "tabs.hpp"

#include <unordered_map>
#include <vector>

class Cursor;

class Buffer : public Tabs {
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

	inline regid_t get_id()		{ return bufid; }

	Cursor *new_cursor();		// Create a cursor for this buffer

	const std::string& name() const;
	inline const std::string& error() const { return errmsg; }
	inline size_t length() const		{ return content.size(); }
	inline const std::string& get_pathname() { return pathname.pathname(); }

	void get_line(std::string& text,lineno_t lno);
	void get_flat(std::string& text,std::vector<size_t>& pos,lineno_t lno);

	bool read_file(const std::string& pathname);

	void dump();

	// Static Methods :

	static Buffer *lookup(regid_t id);			// Locate a buffer by ID
	static regid_t lookup_id(const std::string& name);	// Lookup buffer's ID
	static Buffer *lookup(const std::string& name);		// Locate a buffer by name
};

#endif // BUFFER_HPP

// End buffer.hpp
