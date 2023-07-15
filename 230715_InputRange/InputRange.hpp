#pragma once

#include "_Fwd.hpp"

#include <stdexcept>
#include <typeinfo>
#include <memory>

namespace ImaginaryPlayer
{

template <typename T>
class InputRange
{
 public:
	virtual ~InputRange () = default;

 protected:
	InputRange (const InputRange &rhs) = default;

 private:
	InputRange &operator= (const InputRange &rhs) = delete;

 protected:
	InputRange () = default;

 private:
	virtual InputRange *Do_Clone () const { throw std::runtime_error {"InputRange::Do_Clone has not been implemented (=> not even a forward range)."}; }
	virtual bool Do_Empty () = 0;
	virtual T Do_Front () = 0;
	virtual void Do_PopFront () = 0;

 public:
	std::unique_ptr <InputRange> Clone () const
	{
		InputRange *const pResult = Do_Clone ();
		Azzert (pResult != this);
		Azzert (typeid (*pResult) == typeid (*this));
		return std::unique_ptr <InputRange> {pResult};
	}
	
	bool Empty ()
	{
		return Do_Empty ();
	}
	
	T Front ()
	{
		Azzert (! Do_Empty ());
		return Do_Front ();
	}
	
	void PopFront ()
	{
		Azzert (! Do_Empty ());
		return Do_PopFront ();
	}
};

/*
template <typename Char>
class InputRange_StringView:
	public InputRange <Char>
{
 private:
	      lyb::basic_string_view <Char>           _sv;
	      std::size_t                             _pos;

 public:
	explicit InputRange_StringView (lyb::basic_string_view <Char> sv):
		_sv  {sv},
		_pos {0}
	{}

 private:
	bool Do_Empty () override
	{
		Azzert (_pos <= _sv.size ());
		return _pos < _sv.size ();
	}
	
	Char Do_Front () override
	{
		return _sv [_pos];
	}
	
	void Do_PopFront () override
	{
		++_pos;
	}
};
*/

}
