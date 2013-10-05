//////////////////////////////////////////////////////////////////////
// dispatch.hpp -- Dispatch a keystroke
// Date: Sat Oct  5 16:53:21 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef DISPATCH_HPP
#define DISPATCH_HPP

#include "term.hpp"
#include "bindings.hpp"

class Dispatch {
public:	enum Action {
		More,		// More input required
		Exec,		// Execute proc
		Failed		// Unknown keystroke path
	};
private:
	enum State {
		Initial,	// Start of new keystroke
		Prefix,		// Gathering prefix
		Path		// Gathering path
	};

	State		state;
	int		prefix_sign;
	int		prefix;
	bool		have_prefix;

	keysequ_t	path;

public:	Dispatch();
	~Dispatch();

	Action dispatch(keych_t keystroke,bindproc_t& proc,const Key_Bindings& bmap);
};

#endif // DISPATCH_HPP

// End dispatch.hpp
