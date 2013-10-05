//////////////////////////////////////////////////////////////////////
// term.cpp -- Terminal Support
// Date: Sat Sep  7 16:16:34 2013
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <ncurses.h>

#include "term.hpp"

Terminal::Terminal() {
	initialized 	= false;
	has_colour 	= false;
	has_underline	= false;
	has_reverse	= false;
	has_blink	= false;
	has_dim		= false;
	has_bold	= false;
	has_altcharset	= false;

	scr_lines	= -1;
	scr_cols	= -1;
}

Terminal::~Terminal() {
	if ( initialized )
		fini();
}

void
Terminal::clear() {
	::clear();
}

Terminal::keych_t
Terminal::get() {
	return ::getch();
}

void
Terminal::colours_init() {
	static short colours[] = {
		COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
		COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE };
	short px1, px2, px, fgx, fg;
	
	//////////////////////////////////////////////////////////////
	// Note curses assumes pair 0 is FG(WHITE)+BG(BLACK),
	// actually the terminal's default. So we map all other
	// combos to other pairs :
	//////////////////////////////////////////////////////////////

	for ( px1=0; px1 < 8; ++px1 ) {
		fgx = White - px1;
		for ( px2=0; px2 < 8; ++px2 ) {
#ifdef USE_NCURSES
			{       /* ncurses can set pair 0 */
#else
			if ( px1 || px2 ) {
#endif
				px = ( px1 << 3 ) | px2;
				fg = colours[fgx];
				init_pair(px,fg,colours[px2]);
			}
		}
	}
}                                								    

void
Terminal::init() {
	short full = 850;	// Full brightness, but not bold

	::initscr();
	initialized = true;

	if ( has_colors() ) {
		has_colour = true;
		::start_color();
		if ( can_change_color() && COLORS > 0 ) {
			/* Max brightness is 1000, 0 is black */
			init_color(COLOR_BLACK,0,0,0);
			init_color(COLOR_GREEN,100,full,0);
			init_color(COLOR_RED,full,0,0);
			init_color(COLOR_BLUE,300,300,full);
			init_color(COLOR_CYAN,0,full,full);
			init_color(COLOR_MAGENTA,full,0,full);
			init_color(COLOR_YELLOW,full,full,0);
			init_color(COLOR_WHITE,full,full,full);
                }
		if ( COLOR_PAIRS >= 64 )
			colours_init();		// Map fg/bg colours to pair #
		else	has_colour = false;	// Can't support colour with < 64 pairs
	} else	{
		has_colour = false;
	}

	::intrflush(stdscr,FALSE);
	::keypad(stdscr,TRUE);
	::noecho();
	::noraw();
	::cbreak();
	::nonl();
	::def_prog_mode();

	chtype attrs = ::termattrs();		// Get supported attributes

	has_underline	= !!(attrs & A_UNDERLINE);
	has_reverse	= !!(attrs & A_REVERSE);
	has_blink	= !!(attrs & A_BLINK);
	has_dim		= !!(attrs & A_DIM);
	has_bold	= !!(attrs & A_BOLD);
	has_altcharset	= !!(attrs & A_ALTCHARSET);

	scr_lines = LINES;
	scr_cols  = COLS;

	if ( has_colour )
		set_fg(White).set_bg(Black);
}

Terminal&
Terminal::set_fg(Colour c) {
	int fgx = int(White) - int(c);
	short pair;
	attr_t a;

	attr_get(&a,&pair,0);
	pair = ((fgx << 3) & 070) | ( pair & 0007 );
	set_pair(pair);
	bgpair = pair;

	return *this;
}

Terminal&
Terminal::set_bg(Colour c) {
	short pair;
	attr_t a;

	attr_get(&a,&pair,0);
	pair = ( pair & 070 ) | ( int(c) & 7 );
	set_pair(pair);
	bgpair = pair;

	return *this;
}

void
Terminal::set_pair(short pair) {
	attr_t sv_attr;
	short sv_pair;
	
	attr_get(&sv_attr,&sv_pair,0);
	attr_set(sv_attr,pair,0);
}

void
Terminal::fini() {
	assert(initialized);
	::endwin();
}

// End term.cpp
