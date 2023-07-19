#include "Cursor.hpp"

namespace ImaginaryPlayer
{

unsigned Cursor::GetRow () const { return _iRow; }
unsigned Cursor::GetCol () const { return _iCol; }
unsigned Cursor::GetPos () const { return _iPos; }

Cursor &Cursor::SetRow (unsigned value) { _iRow = value; return *this; }
Cursor &Cursor::SetCol (unsigned value) { _iCol = value; return *this; }
Cursor &Cursor::SetPos (unsigned value) { _iPos = value; return *this; }

Cursor::Cursor ():
	_iRow {1},
	_iCol {1},
	_iPos {0}
{}

void Cursor::Process (char c)
{
	if (c == '\n')
		{ ++_iRow; _iCol = 1; }
	else
		++_iCol;
	
	++_iPos;
}

}
