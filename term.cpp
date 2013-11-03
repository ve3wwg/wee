///////////////////////////////////////////////////////////////////////
// term.cpp -- Terminal Support
// Date: Sat Sep  7 16:16:34 2013  (C) Warren Gay VE3WWG
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


std::unordered_map<int,long>	Terminal::wee_acs_map;


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

keych_t
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

	if ( wee_acs_map.size() == 0 ) {
		//////////////////////////////////////////////////////
		// Initialize the ACS Character Set
		//////////////////////////////////////////////////////
		static struct {
			Graphic	graphic;
			long	acs_int;
		} graphics[] = {
			{ acs_ulcorner,	ACS_ULCORNER },	// upper left corner
			{ acs_llcorner,	ACS_LLCORNER },	// lower left corner
			{ acs_urcorner,	ACS_URCORNER },	// upper right corner
			{ acs_lrcorner,	ACS_LRCORNER },	// lower right corner
			{ acs_ltee,	ACS_LTEE },	// tee pointing right
			{ acs_rtee,	ACS_RTEE },	// tee pointing left
			{ acs_btee,	ACS_BTEE },	// tee pointing up
			{ acs_ttee,	ACS_TTEE },	// tee pointing down
			{ acs_hline,	ACS_HLINE },	// horizontal line
			{ acs_vline,	ACS_VLINE },	// vertical line
			{ acs_plus,	ACS_PLUS },	// large plus or crossover
			{ acs_s1,	ACS_S1 },	// scan line 1
			{ acs_s9,	ACS_S9 },	// scan line 9
			{ acs_diamond,	ACS_DIAMOND },	// diamond
			{ acs_ckboard,	ACS_CKBOARD },	// checker board (stipple)
			{ acs_degree,	ACS_DEGREE },	// degree symbol
			{ acs_plminus,	ACS_PLMINUS },	// plus/minus
			{ acs_bullet,	ACS_BULLET },	// bullet
			{ acs_larrow,	ACS_LARROW },	// arrow pointing left
			{ acs_rarrow,	ACS_RARROW },	// arrow pointing right
			{ acs_darrow,	ACS_DARROW },	// arrow pointing down
			{ acs_uarrow,	ACS_UARROW },	// arrow pointing up
			{ acs_board,	ACS_BOARD },	// board of squares
			{ acs_lantern,	ACS_LANTERN },	// lantern symbol
			{ acs_block,	ACS_BLOCK },	// solid square block
			{ acs_s3,	ACS_S3 },	// scan line 3
			{ acs_s7,	ACS_S7 },	// scan line 7
			{ acs_lequal,	ACS_LEQUAL },	// less/equal
			{ acs_gequal,	ACS_GEQUAL },	// greater/equal
			{ acs_pi,	ACS_PI },	// Pi
			{ acs_nequal,	ACS_NEQUAL },	// not equal
			{ acs_sterling,	ACS_STERLING },	// UK pound sign
			{ acs_sterling , 0 }
		};

		for ( int x=0; graphics[x].acs_int != 0; ++x ) {
			int gx = int(graphics[x].graphic);
			wee_acs_map[gx] = graphics[x].acs_int;
		}
	}
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

void
Terminal::flash() {
	::flash();
}

void
Terminal::mvbottom() {
	int y = scr_lines - 1;
	int x = 0;
	::move(y,x);
}

void
Terminal::mvclear_botline() {
	mvbottom();
	::clrtoeol();
}

void
Terminal::bottomf(const char *format,...) {
	char temp[4096];
	va_list ap;

	va_start(ap,format);
	::vsnprintf(temp,sizeof temp,format,ap);
	va_end(ap);

	temp[sizeof temp-1] = 0;

	mvclear_botline();
	::addstr(temp);
}

Terminal&
Terminal::move(lineno_t y,colno_t x) {
	::move(y,x);
	return *this;
}

Terminal&
Terminal::put(int acs) {
	static int last_ch = -1;
	static int last_acs = '@';

	if ( last_ch == acs ) {
		::addch(last_acs);
	} else	{
		auto it = wee_acs_map.find(acs);
		if ( it == wee_acs_map.end() ) {
			::addch(acs);
		} else	{
			last_ch = acs;
			last_acs = it->second;
			::addch(last_acs);
		}
	}
	return *this;
}

Terminal&
Terminal::put(const std::string& text) {
	for ( size_t x=0; x < text.size(); ++x )
		put(int(text[x]));
	return *this;
}

Terminal&
Terminal::mvput(lineno_t y,colno_t x,const std::string& text) {
	::move(y,x);
	put(text);
	return *this;
}

// End term.cpp
