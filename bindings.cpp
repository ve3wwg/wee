///////////////////////////////////////////////////////////////////////
// bindings.cpp -- Key Bindings
// Date: Sat Oct  5 11:23:41 2013   (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "bindings.hpp"
#include "keystrokes.hpp"

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
			r = node.mapref < 0;		// True if func is leaf node
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

//////////////////////////////////////////////////////////////////////
// Lookup a binding
//////////////////////////////////////////////////////////////////////

bindproc_t
Key_Bindings::lookup(const keysequ_t& sequ,bool &end) const {
	const bindmap_t *bmap = &rootmap;
	
	for ( size_t x=0; x<sequ.size(); ++x ) {
		keych_t keystroke = sequ[x];
		auto it = bmap->find(keystroke);
		if ( it != bmap->end() ) {
			const s_keynode& node = it->second;
			if ( x+1 < sequ.size() ) {
				// Intermediate node:
				if ( node.mapref < 0 ) {
					end = true;		// No further path
					return 0;		// Not found
				}
				bmap = &bindmaps[node.mapref];	// Enter next map
			} else	{
				// Leaf node:
				return node.proc;		// May be null
			}
		} else
			break;	// Not found
	}

	end = false;
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Protected : Recursive map bindings import
//////////////////////////////////////////////////////////////////////

void
Key_Bindings::import(bindmap_t& curmap,keysequ_t& path) {

	for ( auto it = curmap.begin(); it != curmap.end(); ++it ) {
		keych_t keystroke = it->first;
		s_keynode& node = it->second;
	
		path.push_back(keystroke);
		if ( node.proc )
			bind(path,node.proc);
		if ( node.mapref >= 0 )
			import(bindmaps[node.mapref],path);
		path.pop_back();
	}
}

//////////////////////////////////////////////////////////////////////
// Import (copy) bindings
//////////////////////////////////////////////////////////////////////

void
Key_Bindings::import(const Key_Bindings& b) {
	keysequ_t path;
	
	import(rootmap,path);
}

//////////////////////////////////////////////////////////////////////
// Bind using a string
//////////////////////////////////////////////////////////////////////

bool
Key_Bindings::bind(const char *binding,bindproc_t proc) {
	keysequ_t path;

	if ( !to_keysequ(path,binding) )
		return false;
	bind(path,proc);
	return true;
}

// End bindings.cpp
