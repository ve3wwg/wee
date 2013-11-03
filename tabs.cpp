//////////////////////////////////////////////////////////////////////
// tabs.cpp -- Tabs class implementation
// Date: Sun Nov  3 15:20:45 2013
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "tabs.hpp"

Tabs::Tabs() {
	every = 8;
	custom.clear();
}

colno_t
Tabs::fwd_tab(colno_t current) {
	
	if ( every > 0 )
		return current + colno_t(every - unsigned(current) % every);

	size_t c = size_t(current);
	
	for ( size_t cx=c+1; cx<custom.size(); ++cx )
		if ( custom[cx] )
			return colno_t(cx);
	return current+1;
}

colno_t
Tabs::bck_tab(colno_t current) {
	
	if ( current <= 1 )
		return 0;

	if ( every > 0 ) {
		return current - colno_t(every - unsigned(current) % every);
	}

	size_t c = size_t(current);
	
	for ( size_t cx=c-1; cx<=0; --cx )
		if ( custom[cx] )
			return colno_t(custom[cx]);
	return current-1;
}

bool
Tabs::is_tab(colno_t col) {
	size_t c = size_t(col);

	if ( c >= custom.size() )
		return false;
	return custom[c];
}

void
Tabs:: clr_tab(colno_t col) {
	size_t c = size_t(col);

	if ( c >= custom.size() )
		return;
	custom[c] = false;	
}

void
Tabs::set_tab(colno_t col) {
	size_t c = size_t(col);

	for ( size_t cx=custom.size(); cx <= c; ++cx )
		custom.push_back(false);
	custom[c] = true;
}

// End tabs.cpp
