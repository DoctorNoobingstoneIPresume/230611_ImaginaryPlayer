#pragma once

#include "InputRange.hpp"

#ifndef IMAGINARYPLAYER_InputRange_Stream_iDebug
 #define IMAGINARYPLAYER_InputRange_Stream_iDebug 0
#endif

#include <iostream>
#if IMAGINARYPLAYER_InputRange_Stream_iDebug
 #include <sstream>
 #include <iomanip>
#endif

namespace ImaginaryPlayer
{

template <typename Char, class Traits = std::char_traits <Char>>
class InputRange_Stream:
	public InputRange <Char>
{
 private:
	typedef          std::basic_istream <Char, Traits>           stream_type;
	typedef typename stream_type                      ::int_type int_type;
	
	      stream_type                         *_pStream;
	      std::size_t                          _pos;
	      int_type                             _cBuffer;

 public:
	explicit InputRange_Stream (std::basic_istream <Char, Traits> *pStream):
		_pStream {pStream},
		_pos     {0},
		_cBuffer {Traits::eof ()}
	{
		GetOneCharIntoTheBuffer ();
	}

 private:
	void GetOneCharIntoTheBuffer ()
	{
		Azzert (! _pStream->fail ());
		Azzert (! _pStream->eof  ());
		_cBuffer = _pStream->get ();
		
		#if IMAGINARYPLAYER_InputRange_Stream_iDebug
		{
			std::ostringstream os;
			{
				os << std::hex << std::uppercase << _cBuffer << "h";
			}
			
			std::cout << "GetOneCharIntoTheBuffer => " << os.str () << ".\n";
		}
		#endif
		
		++_pos;
	}
	
 private:
	bool Do_Empty () override
	{
		return _cBuffer == Traits::eof ();
	}
	
	Char Do_Front () override
	{
		return static_cast <Char> (_cBuffer);
	}
	
	void Do_PopFront () override
	{
		GetOneCharIntoTheBuffer ();
	}
};

}
