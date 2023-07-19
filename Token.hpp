#pragma once

#include "_Fwd.hpp"

#include "string_view.hpp"

#include <string>
#include <iosfwd>

namespace ImaginaryPlayer
{

class Cursor;

class Token
{
 private:
	      std::string                       _sText;
	      unsigned                          _iRow;
	      unsigned                          _iCol;

 public:
	std::string       GetText  ()                       const;
	lyb::string_view  GetTextQ ()                       const;
	Token            &SetText  (lyb::string_view value);
	Token            &AddText  (char value);
	Token            &AddText  (lyb::string_view value);
	
	unsigned          GetRow   ()                       const;
	Token            &SetRow   (unsigned value);
	
	unsigned          GetCol   ()                       const;
	Token            &SetCol   (unsigned value);
	
	Token            &SetFrom  (const Cursor &cursor);

 public:
	Token ();

 public:
	std::ostream &Put (std::ostream &os) const;
};

std::ostream &operator<< (std::ostream &os, const Token &token);

}
