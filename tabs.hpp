///////////////////////////////////////////////////////////////////////
// tabs.hpp -- Tabs Class
// Date: Sun Nov  3 15:17:57 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef TABS_HPP
#define TABS_HPP

#include "types.hpp"

#include <vector>


class Tabs {
	unsigned		every;		// Every nth col when > 0
	std::vector<bool>	custom;		// Custom tab stops

public:	Tabs();

	colno_t fwd_tab(colno_t current);	// Return tabstop of next column
	colno_t bck_tab(colno_t current);	// Return prior tabstop

	bool is_tab(colno_t col);		// Is col a custom tab stop?
	void clr_tab(colno_t col);		// Clear custom tab stop
	void set_tab(colno_t col);		// Set custom tab stop

	inline unsigned get_every() { return every; } // Returns zero if custom tabs being used
	inline void set_every(colno_t col) { every = unsigned(col); }
};


#endif // TABS_HPP

// End tabs.hpp
