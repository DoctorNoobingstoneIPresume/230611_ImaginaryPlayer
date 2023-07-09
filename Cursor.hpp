#pragma once

#include "_Fwd.hpp"

namespace ImaginaryPlayer
{

// [2023-07-09]
// Cursor:
//   An object of this type holds the position of a cursor within a text:
//   row, column, and position (absolute index of character, across rows).
//   It is passed (by pointer or reference) to the "extract token from stream" function(s)
//   so that for each extracted token we can save the position of that token.

class Cursor
{
 private:
	      unsigned                  _iRow;
	      unsigned                  _iCol;
	      unsigned                  _iPos;

 public:
	unsigned GetRow () const;
	unsigned GetCol () const;
	unsigned GetPos () const;
	
	Cursor &SetRow (unsigned value);
	Cursor &SetCol (unsigned value);
	Cursor &SetPos (unsigned value);
	
 public:
	Cursor ();
	
	void Process (char c);
};

}
