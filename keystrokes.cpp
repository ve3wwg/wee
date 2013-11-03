///////////////////////////////////////////////////////////////////////
// keystrokes.cpp -- Keystrokes Support
// Date: Sat Oct  5 09:56:26 2013   (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include <ncurses.h>
#include <unordered_map>
#include <sstream>

#include "strutil.hpp"
#include "keystrokes.hpp"

static std::unordered_map<int,std::string>     fwd_keymap;
static std::unordered_map<std::string,keych_t> rev_keymap;
static bool initialized = false;

static void
initialize() {

	fwd_keymap[KEY_DOWN] = "^(DOWN)";
	fwd_keymap[KEY_UP] = "^(UP)";
	fwd_keymap[KEY_LEFT] = "^(LEFT)";
	fwd_keymap[KEY_RIGHT] = "^(RIGHT)";
	fwd_keymap[KEY_HOME] = "^(HOME)";
	fwd_keymap[KEY_BACKSPACE] = "^(BACKSPACE)";
	fwd_keymap[KEY_F0] = "^(F0)";
	fwd_keymap[KEY_F(1)] = "^(F1)";
	fwd_keymap[KEY_F(2)] = "^(F2)";
	fwd_keymap[KEY_F(3)] = "^(F3)";
	fwd_keymap[KEY_F(4)] = "^(F4)";
	fwd_keymap[KEY_F(5)] = "^(F5)";
	fwd_keymap[KEY_F(6)] = "^(F6)";
	fwd_keymap[KEY_F(7)] = "^(F7)";
	fwd_keymap[KEY_F(8)] = "^(F8)";
	fwd_keymap[KEY_F(9)] = "^(F9)";
	fwd_keymap[KEY_F(10)] = "^(F10)";
	fwd_keymap[KEY_F(11)] = "^(F11)";
	fwd_keymap[KEY_F(12)] = "^(F12)";
	fwd_keymap[KEY_F(13)] = "^(F13)";
	fwd_keymap[KEY_F(14)] = "^(F14)";
	fwd_keymap[KEY_F(15)] = "^(F15)";
	fwd_keymap[KEY_F(16)] = "^(F16)";
	fwd_keymap[KEY_F(17)] = "^(F17)";
	fwd_keymap[KEY_F(18)] = "^(F18)";
	fwd_keymap[KEY_F(19)] = "^(F19)";
	fwd_keymap[KEY_DL] = "^(DL)";
	fwd_keymap[KEY_IL] = "^(IL)";
	fwd_keymap[KEY_DC] = "^(DC)";
	fwd_keymap[KEY_IC] = "^(IC)";
	fwd_keymap[KEY_EIC] = "^(EIC)";
	fwd_keymap[KEY_CLEAR] = "^(CLEAR)";
	fwd_keymap[KEY_EOS] = "^(EOS)";
	fwd_keymap[KEY_EOL] = "^(EOL)";
	fwd_keymap[KEY_SF] = "^(SF)";
	fwd_keymap[KEY_SR] = "^(SR)";
	fwd_keymap[KEY_NPAGE] = "^(NPAGE)";
	fwd_keymap[KEY_PPAGE] = "^(PPAGE)";
	fwd_keymap[KEY_STAB] = "^(STAB)";
	fwd_keymap[KEY_CTAB] = "^(CTAB)";
	fwd_keymap[KEY_CATAB] = "^(CATAB)";
	fwd_keymap[KEY_ENTER] = "^M";
	fwd_keymap[KEY_PRINT] = "^(PRINT)";
	fwd_keymap[KEY_LL] = "^(LL)";
	fwd_keymap[KEY_A1] = "^(A1)";
	fwd_keymap[KEY_A3] = "^(A3)";
	fwd_keymap[KEY_B2] = "^(B2)";
	fwd_keymap[KEY_C1] = "^(C1)";
	fwd_keymap[KEY_C3] = "^(C3)";
	fwd_keymap[KEY_BTAB] = "^(BTAB)";
	fwd_keymap[KEY_BEG] = "^(BEG)";
	fwd_keymap[KEY_CANCEL] = "^(CANCEL)";
	fwd_keymap[KEY_CLOSE] = "^(CLOSE)";
	fwd_keymap[KEY_COMMAND] = "^(COMMAND)";
	fwd_keymap[KEY_COPY] = "^(COPY)";
	fwd_keymap[KEY_CREATE] = "^(CREATE)";
	fwd_keymap[KEY_END] = "^(END)";
	fwd_keymap[KEY_EXIT] = "^(EXIT)";
	fwd_keymap[KEY_FIND] = "^(FIND)";
	fwd_keymap[KEY_HELP] = "^(HELP)";
	fwd_keymap[KEY_MARK] = "^(MARK)";
	fwd_keymap[KEY_MESSAGE] = "^(MESSAGE)";
	fwd_keymap[KEY_MOVE] = "^(MOVE)";
	fwd_keymap[KEY_NEXT] = "^(NEXT)";
	fwd_keymap[KEY_OPEN] = "^(OPEN)";
	fwd_keymap[KEY_OPTIONS] = "^(OPTIONS)";
	fwd_keymap[KEY_PREVIOUS] = "^(PREVIOUS)";
	fwd_keymap[KEY_REDO] = "^(REDO)";
	fwd_keymap[KEY_REFERENCE] = "^(REFERENCE)";
	fwd_keymap[KEY_REFRESH] = "^(REFRESH)";
	fwd_keymap[KEY_REPLACE] = "^(REPLACE)";
	fwd_keymap[KEY_RESTART] = "^(RESTART)";
	fwd_keymap[KEY_RESUME] = "^(RESUME)";
	fwd_keymap[KEY_SAVE] = "^(SAVE)";
	fwd_keymap[KEY_SBEG] = "^(SBEG)";
	fwd_keymap[KEY_SCANCEL] = "^(SCANCEL)";
	fwd_keymap[KEY_SCOMMAND] = "^(SCOMMAND)";
	fwd_keymap[KEY_SCOPY] = "^(SCOPY)";
	fwd_keymap[KEY_SCREATE] = "^(SCREATE)";
	fwd_keymap[KEY_SDC] = "^(SDC)";
	fwd_keymap[KEY_SDL] = "^(SDL)";
	fwd_keymap[KEY_SELECT] = "^(SELECT)";
	fwd_keymap[KEY_SEND] = "^(SEND)";
	fwd_keymap[KEY_SEOL] = "^(SEOL)";
	fwd_keymap[KEY_SEXIT] = "^(SEXIT)";
	fwd_keymap[KEY_SFIND] = "^(SFIND)";
	fwd_keymap[KEY_SHELP] = "^(SHELP)";
	fwd_keymap[KEY_SHOME] = "^(SHOME)";
	fwd_keymap[KEY_SIC] = "^(SIC)";
	fwd_keymap[KEY_SLEFT] = "^(SLEFT)";
	fwd_keymap[KEY_SMESSAGE] = "^(SMESSAGE)";
	fwd_keymap[KEY_SMOVE] = "^(SMOVE)";
	fwd_keymap[KEY_SNEXT] = "^(SNEXT)";
	fwd_keymap[KEY_SOPTIONS] = "^(SOPTIONS)";
	fwd_keymap[KEY_SPREVIOUS] = "^(SPREVIOUS)";
	fwd_keymap[KEY_SPRINT] = "^(SPRINT)";
	fwd_keymap[KEY_SREDO] = "^(SREDO)";
	fwd_keymap[KEY_SREPLACE] = "^(SREPLACE)";
	fwd_keymap[KEY_SRIGHT] = "^(SRIGHT)";
	fwd_keymap[KEY_SRSUME] = "^(SRSUME)";
	fwd_keymap[KEY_SSAVE] = "^(SSAVE)";
	fwd_keymap[KEY_SSUSPEND] = "^(SSUSPEND)";
	fwd_keymap[KEY_SUNDO] = "^(SUNDO)";
	fwd_keymap[KEY_SUSPEND] = "^(SUSPEND)";
	fwd_keymap[KEY_UNDO] = "^(UNDO)";

	rev_keymap["^(DOWN)"] = KEY_DOWN;
	rev_keymap["^(UP)"] = KEY_UP;
	rev_keymap["^(LEFT)"] = KEY_LEFT;
	rev_keymap["^(RIGHT)"] = KEY_RIGHT;
	rev_keymap["^(HOME)"] = KEY_HOME;
	rev_keymap["^(BACKSPACE)"] = KEY_BACKSPACE;
	rev_keymap["^(F0)"] = KEY_F0;
	rev_keymap["^(F1)"] = KEY_F(1);
	rev_keymap["^(F2)"] = KEY_F(2);
	rev_keymap["^(F3)"] = KEY_F(3);
	rev_keymap["^(F4)"] = KEY_F(4);
	rev_keymap["^(F5)"] = KEY_F(5);
	rev_keymap["^(F6)"] = KEY_F(6);
	rev_keymap["^(F7)"] = KEY_F(7);
	rev_keymap["^(F8)"] = KEY_F(8);
	rev_keymap["^(F9)"] = KEY_F(9);
	rev_keymap["^(F10)"] = KEY_F(10);
	rev_keymap["^(F11)"] = KEY_F(11);
	rev_keymap["^(F12)"] = KEY_F(12);
	rev_keymap["^(F13)"] = KEY_F(13);
	rev_keymap["^(F14)"] = KEY_F(14);
	rev_keymap["^(F15)"] = KEY_F(15);
	rev_keymap["^(F16)"] = KEY_F(16);
	rev_keymap["^(F17)"] = KEY_F(17);
	rev_keymap["^(F18)"] = KEY_F(18);
	rev_keymap["^(F19)"] = KEY_F(19);
	rev_keymap["^(DL)"] = KEY_DL;
	rev_keymap["^(IL)"] = KEY_IL;
	rev_keymap["^(DC)"] = KEY_DC;
	rev_keymap["^(IC)"] = KEY_IC;
	rev_keymap["^(EIC)"] = KEY_EIC;
	rev_keymap["^(CLEAR)"] = KEY_CLEAR;
	rev_keymap["^(EOS)"] = KEY_EOS;
	rev_keymap["^(EOL)"] = KEY_EOL;
	rev_keymap["^(SF)"] = KEY_SF;
	rev_keymap["^(SR)"] = KEY_SR;
	rev_keymap["^(NPAGE)"] = KEY_NPAGE;
	rev_keymap["^(PPAGE)"] = KEY_PPAGE;
	rev_keymap["^(STAB)"] = KEY_STAB;
	rev_keymap["^(CTAB)"] = KEY_CTAB;
	rev_keymap["^(CATAB)"] = KEY_CATAB;
	rev_keymap["^M"] = KEY_ENTER;
	rev_keymap["^(PRINT)"] = KEY_PRINT;
	rev_keymap["^(LL)"] = KEY_LL;
	rev_keymap["^(A1)"] = KEY_A1;
	rev_keymap["^(A3)"] = KEY_A3;
	rev_keymap["^(B2)"] = KEY_B2;
	rev_keymap["^(C1)"] = KEY_C1;
	rev_keymap["^(C3)"] = KEY_C3;
	rev_keymap["^(BTAB)"] = KEY_BTAB;
	rev_keymap["^(BEG)"] = KEY_BEG;
	rev_keymap["^(CANCEL)"] = KEY_CANCEL;
	rev_keymap["^(CLOSE)"] = KEY_CLOSE;
	rev_keymap["^(COMMAND)"] = KEY_COMMAND;
	rev_keymap["^(COPY)"] = KEY_COPY;
	rev_keymap["^(CREATE)"] = KEY_CREATE;
	rev_keymap["^(END)"] = KEY_END;
	rev_keymap["^(EXIT)"] = KEY_EXIT;
	rev_keymap["^(FIND)"] = KEY_FIND;
	rev_keymap["^(HELP)"] = KEY_HELP;
	rev_keymap["^(MARK)"] = KEY_MARK;
	rev_keymap["^(MESSAGE)"] = KEY_MESSAGE;
	rev_keymap["^(MOVE)"] = KEY_MOVE;
	rev_keymap["^(NEXT)"] = KEY_NEXT;
	rev_keymap["^(OPEN)"] = KEY_OPEN;
	rev_keymap["^(OPTIONS)"] = KEY_OPTIONS;
	rev_keymap["^(PREVIOUS)"] = KEY_PREVIOUS;
	rev_keymap["^(REDO)"] = KEY_REDO;
	rev_keymap["^(REFERENCE)"] = KEY_REFERENCE;
	rev_keymap["^(REFRESH)"] = KEY_REFRESH;
	rev_keymap["^(REPLACE)"] = KEY_REPLACE;
	rev_keymap["^(RESTART)"] = KEY_RESTART;
	rev_keymap["^(RESUME)"] = KEY_RESUME;
	rev_keymap["^(SAVE)"] = KEY_SAVE;
	rev_keymap["^(SBEG)"] = KEY_SBEG;
	rev_keymap["^(SCANCEL)"] = KEY_SCANCEL;
	rev_keymap["^(SCOMMAND)"] = KEY_SCOMMAND;
	rev_keymap["^(SCOPY)"] = KEY_SCOPY;
	rev_keymap["^(SCREATE)"] = KEY_SCREATE;
	rev_keymap["^(SDC)"] = KEY_SDC;
	rev_keymap["^(SDL)"] = KEY_SDL;
	rev_keymap["^(SELECT)"] = KEY_SELECT;
	rev_keymap["^(SEND)"] = KEY_SEND;
	rev_keymap["^(SEOL)"] = KEY_SEOL;
	rev_keymap["^(SEXIT)"] = KEY_SEXIT;
	rev_keymap["^(SFIND)"] = KEY_SFIND;
	rev_keymap["^(SHELP)"] = KEY_SHELP;
	rev_keymap["^(SHOME)"] = KEY_SHOME;
	rev_keymap["^(SIC)"] = KEY_SIC;
	rev_keymap["^(SLEFT)"] = KEY_SLEFT;
	rev_keymap["^(SMESSAGE)"] = KEY_SMESSAGE;
	rev_keymap["^(SMOVE)"] = KEY_SMOVE;
	rev_keymap["^(SNEXT)"] = KEY_SNEXT;
	rev_keymap["^(SOPTIONS)"] = KEY_SOPTIONS;
	rev_keymap["^(SPREVIOUS)"] = KEY_SPREVIOUS;
	rev_keymap["^(SPRINT)"] = KEY_SPRINT;
	rev_keymap["^(SREDO)"] = KEY_SREDO;
	rev_keymap["^(SREPLACE)"] = KEY_SREPLACE;
	rev_keymap["^(SRIGHT)"] = KEY_SRIGHT;
	rev_keymap["^(SRSUME)"] = KEY_SRSUME;
	rev_keymap["^(SSAVE)"] = KEY_SSAVE;
	rev_keymap["^(SSUSPEND)"] = KEY_SSUSPEND;
	rev_keymap["^(SUNDO)"] = KEY_SUNDO;
	rev_keymap["^(SUSPEND)"] = KEY_SUSPEND;
	rev_keymap["^(UNDO)"] = KEY_UNDO;

	initialized = true;
}

//////////////////////////////////////////////////////////////////////
// Convert a single keystroke into text for display purposes
//////////////////////////////////////////////////////////////////////

std::string
to_text(keych_t ch) {
	char temp[16];

	if ( !initialized )
		initialize();

	if ( ch < keych_t(' ') ) {
		if ( ch == 0x1E )	// == ^^
			return "^(^)";
		temp[0] = '^';
		temp[1] = '@' + ch;
		temp[2] = 0;
		return temp;
	} else if ( ch >= keych_t(' ') && ch < 0x7F ) {
		temp[0] = char(ch);
		temp[1] = 0;
		return temp;
	} else if ( ch == 0x7F ) {
		return "^?";
	} else	{
		auto it = fwd_keymap.find(int(ch));
		if ( it != fwd_keymap.end() )
			return it->second;
	}
	return "^!";
}

//////////////////////////////////////////////////////////////////////
// Convert string representation into a keych_t :
// 
// ARGUMENTS:
//	symbol		Input text to parse
//	widthp		Ptr to returned text width,
//			or 0 if not required;
//	For Example:
//		"A^(LEFT)B" returns:
//			'A' and *widthp = 1 for "A"
//		"^(LEFT)B" returns:
//			KEY_LEFT and *widthp = 7 for "^(LEFT)"
// RETURNS:
//	keych_t		Curses keystroke
//////////////////////////////////////////////////////////////////////

keych_t 
to_keych(const char *symbol,int *widthp) {
	keych_t ch = '?';
	int width = 1;

	if ( !initialized )
		initialize();

	if ( *symbol == '^' ) {
		if ( !strncmp(symbol,"^(^)",width=4) ) {
			ch = 0x1E;
		} else if ( symbol[1] != '(' ) {
			ch = int(symbol[1]) & 0x1F;
			width = 2;
		} else if ( symbol[1] == '(' ) {
			// ^(LEFT)
			// 0123456
			const char *rbrkt = strchr(symbol,')');
			if ( rbrkt != 0 ) {
				width = (rbrkt - symbol) + 1;
				char tsym[width+1];
				
				strncpy(tsym,symbol,width)[width] = 0;
				std::string uc_symbol = to_upper_const(tsym);

				auto it = rev_keymap.find(uc_symbol);
				if ( it != rev_keymap.end() )
					ch = it->second;
				else	ch = KEY_ERROR;
			} else	{
				ch = KEY_ERROR;
				width = 2;
			}
		}
	} else if ( *symbol >= ' ' && *symbol <= '~' ) {
		ch = keych_t(*symbol);
		width = 1;
	} else	{
		ch = KEY_ERROR;
		width = 1;
	}

	if ( widthp != 0 )
		*widthp = width;

	return ch;
}

//////////////////////////////////////////////////////////////////////
// Convert a keystroke sequence to text
//////////////////////////////////////////////////////////////////////

std::string
to_text(const keysequ_t& path) {
	std::stringstream s;

	for ( size_t x=0; x<path.size(); ++x ) {
		std::string ks = to_text(path[x]);
		s << ks;
	}

	return s.str();
}

//////////////////////////////////////////////////////////////////////
// Convert a text keystroke sequence to a keysequ_t
//////////////////////////////////////////////////////////////////////

bool
to_keysequ(keysequ_t& path,const char *text) {
	const char *cp;
	int width;

	path.clear();

	for ( cp=text; *cp; cp += width ) {
		keych_t keystroke = to_keych(cp,&width);
		if ( keystroke == KEY_ERROR )
			return false;
		path.push_back(keystroke);
	}
	return true;
}

// End keystrokes.cpp
