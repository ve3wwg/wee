//////////////////////////////////////////////////////////////////////
// view.hpp -- View class for buffer mapping to terminal window
// Date: Wed Oct 30 22:52:25 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef VIEW_HPP
#define VIEW_HPP

#include "term.hpp"
#include "cursor.hpp"
#include "buffer.hpp"

#include <unordered_map>

class View {
	viewid_t	id;		// View ID
	Terminal	*term;		// Terminal associated with this view
	Cursor		top;		// Top cursor
	colno_t		offset;		// Column offset, else 0
	size_t		width;		// Window text width
	size_t		height;		// Height of the window
	lineno_t	statline;	// Status line number (relative to top)

	lineno_t	topline;	// Terminal line # of top line
	colno_t		left;		// Terminal leftmost column no.

	static viewid_t			next_id;
	static std::unordered_map<viewid_t,View*> views_map;
	static View			*main_view;

protected:
	void fetch_line(std::string& text,int tline);

public:	View(Terminal& term);
	~View();

	void associate(const Cursor& bufref);	// Ref to top line of buffer to display in view
	void disassociate(regid_t bufid);	// A buffer was destroyed

	void draw_status();			// Draw status line
	void draw();				// Draw window on terminal

	// Static methods

	static void buffer_destroyed(regid_t bufid);
	static View& focus();			// Return the main view (or focused view)
};

#endif // VIEW_HPP

// End view.hpp
