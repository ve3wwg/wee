//////////////////////////////////////////////////////////////////////
// path_lists.cpp -- Pathname class implementation
// Date: Mon Oct 28 21:29:53 2013
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/param.h>
#include <assert.h>

#include <sstream>
#include <vector>

#include "pathnames.hpp"

Pathname::Pathname() {
	import(0);
}

Pathname::Pathname(const char *path) {
	import(path);
}

void
Pathname::import(const char *path) {
	std::string p = path;
	std::vector<std::string> pvec;

	full = false;
	path_list.clear();

	if ( !path || !*path ) {
		pathstr = "";
		pathcstr = pathstr.c_str();
		return;
	}

	if ( p.size() > 0 && p[0] != '/' ) {
		char cwd[MAXPATHLEN+1];

		getcwd(cwd,MAXPATHLEN);
		cwd[MAXPATHLEN] = 0;
		p = cwd;
		p += "/";
		p += path;
	}

	full = p[0] == '/';

	char buf[p.size()+1];
	strcpy(buf,p.c_str());

	char *cp = full ? buf + 1 : buf;
	char *ep;

	for ( cp = strtok_r(cp,"/",&ep); cp; cp = strtok_r(0,"/",&ep) ) {
		std::string s = cp;
		pvec.push_back(cp);
	}

	bool f;

	do	{
		f = false;
		for ( auto it=pvec.begin(); it != pvec.end(); ++it ) {
			if ( *it == "." ) {
				pvec.erase(it);
				f = true;
				break;
			} else if ( *it == ".." && it+1 != pvec.end() ) {
				pvec.erase(it,it+2);
				f = true;
				break;
			}
		}
	} while ( f );

	pathstr.clear();
	std::stringstream s;

	if ( full )
		s << "/";

	for ( size_t px=0; px<pvec.size(); ++px ) {
		s << pvec[px];
		if ( px + 1 < pvec.size() )
			s << "/";
		path_list.push_back(pvec[px]);
	}

	pathstr = s.str();
	pathcstr = pathstr.c_str();
}

const std::string&
Pathname::pathname() const {
	return pathstr;
}

const char *
Pathname::cpathname() const {
	return pathcstr;
}

std::string
Pathname::basename() const {

	if ( path_list.size() <= 0 )
		return "";
	return *path_list.rbegin();
}

std::string
Pathname::dirname() const {

	if ( path_list.size() <= 1 )
		return "";

	std::stringstream s;

	if ( full )
		s << "/";

	size_t n = path_list.size();
	size_t x = 0;

	for ( auto it=path_list.begin(); x+1 < n; ++it ) {
		if ( x++ != 0 )
			s << "/";
		s << *it;
	}

	return s.str();
}

// End pathnames.cpp
