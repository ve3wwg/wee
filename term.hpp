//////////////////////////////////////////////////////////////////////
// term.hpp -- Terminal Support
// Date: Sat Sep  7 16:15:27 2013   (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////

#ifndef TERM_HPP
#define TERM_HPP

#include "types.hpp"

#include <string>

typedef int	keych_t;

class Terminal {
	enum Colour {
		Black = 0,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White
	};

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

	Terminal& move(lineno_t y,colno_t x);
	Terminal& put(const std::string& text);
	Terminal& mvput(lineno_t y,colno_t x,const std::string& text);
};


#endif // TERM_HPP

// End term.hpp
