#include "Token.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

namespace ImaginaryPlayer
{

std::string       Token::GetText  ()                       const { return _sText; }
lyb::string_view  Token::GetTextQ ()                       const { return _sText; }
Token            &Token::SetText  (lyb::string_view value)       { _sText = lyb::ViewToString (value); return *this; }
Token            &Token::AddText  (char value)                   { _sText.append (1, value); return *this; }
Token            &Token::AddText  (lyb::string_view value)       { _sText.append (value.begin (), value.end ()); return *this; }

unsigned          Token::GetRow   ()                       const { return _iRow; }
Token            &Token::SetRow   (unsigned value)               { _iRow = value; return *this; }

unsigned          Token::GetCol   ()                       const { return _iCol; }
Token            &Token::SetCol   (unsigned value)               { _iCol = value; return *this; }

Token::Token ():
	_iRow {0},
	_iCol {0}
{}

std::ostream &Token::Put (std::ostream &os) const
{
	std::ostringstream osTmp;
	{
		osTmp << "@" << _iRow << ":" << _iCol << " `" << _sText << "`";
	}
	
	return os << osTmp.str ();
}

std::ostream &operator<< (std::ostream &os, const Token &token)
{
	return token.Put (os);
}

}
