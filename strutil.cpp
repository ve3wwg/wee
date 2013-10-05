//////////////////////////////////////////////////////////////////////
// strutil.cpp -- String Utility Support
// Date: Sat Oct  5 10:56:05 2013   (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "strutil.hpp"

//////////////////////////////////////////////////////////////////////
// Return an uppercased copy of str
//////////////////////////////////////////////////////////////////////

std::string
to_upper_const(const std::string& str) {
	std::string uc_str = str;
	return to_upper(uc_str);
}

//////////////////////////////////////////////////////////////////////
// Modify str so that it becomes uppercased (returns ref to str)
//////////////////////////////////////////////////////////////////////

std::string&
to_upper(std::string& str) {
	size_t slen = str.size();
	char ch;

	for ( size_t x=0; x<slen; ++x ) {
		ch = str[x];
		if ( islower(ch) )
			str[x] = toupper(ch);
	}
	return str;
}

// End strutil.cpp
