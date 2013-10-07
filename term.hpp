//////////////////////////////////////////////////////////////////////
// term.hpp -- Terminal Support
// Date: Sat Sep  7 16:15:27 2013   (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////

#ifndef TERM_HPP
#define TERM_HPP

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

	int		scr_lines;
	int		scr_cols;

	short		bgpair;

	void colours_init();
	void set_pair(short pair);

public:	Terminal();
	~Terminal();

	void init();
	void fini();

	void clear();

	Terminal& set_fg(Colour c);
	Terminal& set_bg(Colour c);

	keych_t get();

	void flash();
};


#endif // TERM_HPP

// End term.hpp
