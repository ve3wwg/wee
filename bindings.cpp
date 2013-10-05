//////////////////////////////////////////////////////////////////////
// bindings.cpp -- Key Bindings
// Date: Sat Oct  5 11:23:41 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "bindings.hpp"

Key_Bindings::Key_Bindings() {
}

Key_Bindings::~Key_Bindings() {
}

//////////////////////////////////////////////////////////////////////
// Allocate a new binding map for a node 
//////////////////////////////////////////////////////////////////////

int
Key_Bindings::allocmap() {
	int mapx = -1;

	auto it = freemaps.begin();
	if ( it != freemaps.end() ) {
		mapx = *it;
		freemaps.erase(mapx);
	} else	{
		bindmap_t newmap;
		mapx = bindmaps.size();
		bindmaps.push_back(newmap);
	}
	return mapx;
}

//////////////////////////////////////////////////////////////////////
// Deallocate a binding map
//////////////////////////////////////////////////////////////////////

void
Key_Bindings::freemap(int mapx) {
	bindmap_t& map = bindmaps[mapx];

	map.clear();
	freemaps.insert(mapx);
}

//////////////////////////////////////////////////////////////////////
// Bind a keysequence to a function
//////////////////////////////////////////////////////////////////////

bool
Key_Bindings::bind(const keysequ_t& sequ,bindproc_t proc) {
	bindmap_t *bmap = &rootmap;
	bool r = false;
	
	for ( size_t x=0; x<sequ.size(); ++x ) {
		keych_t keystroke = sequ[x];
		s_keynode& node = (*bmap)[keystroke];
		if ( x+1 < sequ.size() ) {
			if ( node.mapref < 0 )
				node.mapref = allocmap();
			bmap = &bindmaps[node.mapref];
		} else	{
			node.proc = proc;
			r = node.mapref < 0;		// True if func is not overriden
		}
	}
	return r;
}

//////////////////////////////////////////////////////////////////////
// Unbind a sequence
//////////////////////////////////////////////////////////////////////

bool
Key_Bindings::unbind(const keysequ_t& sequ) {
	bindmap_t *bmap = &rootmap;
	
	for ( size_t x=0; x<sequ.size(); ++x ) {
		keych_t keystroke = sequ[x];
		auto it = bmap->find(keystroke);
		if ( it != bmap->end() ) {
			s_keynode& node = it->second;
			if ( x+1 < sequ.size() ) {
				// Intermediate node:
				if ( node.mapref < 0 )
					return false;		// Not found
				bmap = &bindmaps[node.mapref];	// Enter next map
			} else	{
				// Leaf node:
				if ( node.mapref >= 0 ) {
					freemap(node.mapref);
					node.mapref = -1;
				}
				node.proc = 0;
				bmap->erase(keystroke);
				return true;
			}
		} else
			break;	// Not found
	}

	return false;
}

// End bindings.cpp
