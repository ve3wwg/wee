//////////////////////////////////////////////////////////////////////
// pathnames.hpp -- Pathname class
// Date: Mon Oct 28 21:28:32 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef PATHNAMES_HPP
#define PATHNAMES_HPP

#include <string>
#include <list>

class Pathname {
	bool			full;		// True if this is a full path
	std::list<std::string>	path_list;	// The list of pathname components
	std::string		pathstr;	// String form of pathname
	const char 		*pathcstr;

protected:
	void init(const char *path);

public:	Pathname();
	Pathname(const char *path);
	const std::string& pathname() const;
	const char * cpathname() const;
};


#endif // PATHNAMES_HPP

// End pathnames.hpp
