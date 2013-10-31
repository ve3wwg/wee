//////////////////////////////////////////////////////////////////////
// view.hpp -- View class for buffer mapping to terminal window
// Date: Wed Oct 30 22:52:25 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef VIEW_HPP
#define VIEW_HPP

#include "term.hpp"
#include "buffer.hpp"

class View {
	Terminal	*term;		// Terminal associated with this view
	Cursor		*top;		// Top cursor, if any

public:	View(Terminal& term);
	~View();

	void associate(const Cursor& model);	// Ref to top line of buffer to display in view
};

#endif // VIEW_HPP

// End view.hpp
