//////////////////////////////////////////////////////////////////////
// view.hpp -- View class for buffer mapping to terminal window
// Date: Wed Oct 30 22:52:25 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef VIEW_HPP
#define VIEW_HPP

#include "term.hpp"
#include "buffer.hpp"

#include <unordered_map>

class View {
	viewid_t	id;		// View ID
	Terminal	*term;		// Terminal associated with this view
	Cursor		*top;		// Top cursor, if any

	static viewid_t			next_id;
	static std::unordered_map<viewid_t,View*> views_map;

public:	View(Terminal& term);
	~View();

	void associate(const Cursor& model);	// Ref to top line of buffer to display in view
	void disassociate(regid_t bufid);	// A buffer was destroyed

	// Static methods

	static void buffer_destroyed(regid_t bufid);
};

#endif // VIEW_HPP

// End view.hpp
