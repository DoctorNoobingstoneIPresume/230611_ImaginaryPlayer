#pragma once

#include "InputRange.hpp"

#include "string_view.hpp"

namespace ImaginaryPlayer
{

template <typename Char>
class InputRange_SV:
	public InputRange <Char>
{
 private:
	      lyb::basic_string_view <Char>        _sv;
	      std::size_t                          _pos;

 public:
	explicit InputRange_SV (lyb::basic_string_view <Char> sv):
		_sv  {sv},
		_pos {0}
	{}

 private:
	InputRange_SV *Do_Clone () const override
	{
		return new InputRange_SV {*this};
	}
	
	bool Do_Empty () override
	{
		// [2023-07-06] TODO:
		//   `Azzert (_pos <= _sv.size ());` (but this is already checked in `Do_Front` and `Do_PopFront`)
		//   and/or consider `>=` vs `==`.
		return _pos >= _sv.size ();
	}
	
	Char Do_Front () override
	{
		Azzert (_pos < _sv.size ());
		return _sv [_pos];
	}
	
	void Do_PopFront () override
	{
		Azzert (_pos < _sv.size ());
		++_pos;
	}
};

}
