///////////////////////////////////////////////////////////////////////
// keystrokes.hpp -- Keystrokes Support
// Date: Sat Oct  5 09:55:19 2013   (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////

#ifndef KEYSTROKES_HPP
#define KEYSTROKES_HPP

#include <string>
#include <vector>

#include "term.hpp"

typedef std::vector<keych_t> keysequ_t;

#define KEY_ERROR	((keych_t)(~0))

#define CONTROL(ch)	((ch)&0x1F)
#define KEY_ESC		CONTROL('[')

std::string to_text(keych_t ch);
keych_t to_keych(const char *symbol,int *widthp);

std::string to_text(const keysequ_t& path);
bool to_keysequ(keysequ_t& path,const char *text);

#endif // KEYSTROKES_HPP

// End keystrokes.hpp
