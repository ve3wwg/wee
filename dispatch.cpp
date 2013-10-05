//////////////////////////////////////////////////////////////////////
// dispatch.cpp -- Dispatch a keystroke
// Date: Sat Oct  5 16:57:24 2013   (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "dispatch.hpp"
#include "bindings.hpp"

Dispatch::Dispatch() : path() {
	state = Prefix;
	prefix = 1;
	have_prefix = false;
}

Dispatch::~Dispatch() {
}

Dispatch::Action
Dispatch::dispatch(keych_t keystroke,bindproc_t& proc,const Key_Bindings& bmap) {
	bool end = false;

	proc = 0;

	for (;;) {
		switch ( state ) {
		case Initial :
			path.clear();

			prefix_sign = 1;
			prefix = 1;
			have_prefix = false;

			if ( keystroke == '-' || keystroke == '+' ) {
				prefix_sign = keystroke == '-' ? (-1) : (+1);
				have_prefix = true;
				state = Prefix;
				return More;
			}

			if ( keystroke >= '0' && keystroke <= '9' ) {
				prefix_sign = +1;
				prefix = keystroke & 0x0F;
				have_prefix = true;
				state = Prefix;
				return More;
			}

			have_prefix = false;
			prefix_sign = +1;
			prefix = 1;
			state = Path;
			break;

		case Prefix :
			if ( keystroke >= '0' && keystroke <= '9' ) {
				if ( !prefix && keystroke == '0' )
					return More;
				prefix = prefix * 10 + (keystroke & 0x0F);
				return More;
			}
			state = Path;
			break;

		case Path :
			path.push_back(keystroke);
			proc = bmap.lookup(path,end);
			if ( proc ) {
				state = Initial;
				return Exec;
			}
			if ( end ) {
				state = Initial;
				return Failed;
			}
			return More;
		}
	}

	return Failed;
}

// End dispatch.cpp
