//////////////////////////////////////////////////////////////////////
// term.hpp -- Terminal Support
// Date: Sat Sep  7 16:15:27 2013   (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////

#ifndef TERM_HPP
#define TERM_HPP

#include "types.hpp"

#include <string>
#include <unordered_map>

typedef int	keych_t;

class Terminal {
public:	enum Colour {
		Black = 0,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White
	};

	enum Graphic {
		acs_ulcorner=0,		// upper left corner
		acs_llcorner,		// lower left corner
		acs_urcorner,		// upper right corner
		acs_lrcorner,		// lower right corner
		acs_ltee,		// tee pointing right
		acs_rtee,		// tee pointing left
		acs_btee,		// tee pointing up
		acs_ttee,		// tee pointing down
		acs_hline,		// horizontal line
		acs_vline,		// vertical line
		acs_plus=11,		// large plus or crossover
		acs_s1,			// scan line 1
		acs_s9=14,		// scan line 9
		acs_diamond,		// diamond
		acs_degree,		// degree symbol
		acs_plminus,		// plus/minus
		acs_bullet,		// bullet
		acs_larrow,		// arrow pointing left
		acs_rarrow,		// arrow pointing right
		acs_darrow,		// arrow pointing down
		acs_uarrow,		// arrow pointing up
		acs_board,		// board of squares
		acs_lantern,		// lantern symbol
		acs_block,		// solid square block
		acs_s3,			// scan line 3
		acs_s7,			// scan line 7
		acs_lequal,		// less/equal
		acs_gequal,		// greater/equal
		acs_pi,			// Pi
		acs_ckboard=127,	// checker board (stipple)
		acs_nequal=129,		// not equal
		acs_sterling=143	// UK pound sign
	};

private:

	bool		initialized;
	bool		has_colour;

	bool		has_underline;
	bool		has_reverse;
	bool		has_blink;
	bool		has_dim;
	bool		has_bold;
	bool		has_altcharset;

	lineno_t	scr_lines;
	colno_t		scr_cols;

	short		bgpair;

	void colours_init();
	void set_pair(short pair);

	static std::unordered_map<int,int>	acs_map;

public:	Terminal();
	~Terminal();

	void init();
	void fini();

	inline int get_lines()	{ return scr_lines; }
	inline int get_cols()	{ return scr_cols; }

	void clear();

	Terminal& set_fg(Colour c);
	Terminal& set_bg(Colour c);

	keych_t get();

	void flash();

	void mvbottom();
	void mvclear_botline();
	void bottomf(const char *format,...);

	Terminal& put(int acs);
	Terminal& move(lineno_t y,colno_t x);
	Terminal& put(const std::string& text);
	Terminal& mvput(lineno_t y,colno_t x,const std::string& text);
};


#endif // TERM_HPP

// End term.hpp
