#include "Lexer.hpp"
#include "InputRange_SV.hpp"

#include <cstring>

int main ()
{
	using namespace ImaginaryPlayer;
	
	{
		const char *const input {"  Hello, there from \"   the most beautiful place   \" on...      Earth !!"};
		InputRange_SV <char> inputrange {input};
		
		const auto result {Tokenize (&inputrange)};
		
		Azzert (result);
		
		const char *const apszTokens []
		{
			"Hello",
			",",
			"there",
			"from",
			"   the most beautiful place   ",
			"on",
			".",
			".",
			".",
			"Earth",
			"!",
			"!"
		};
		
		const std::size_t nTokens = nelems (apszTokens);
		Azzert (result->size () == nTokens);
		for (std::size_t iToken = 0; iToken < nTokens; ++iToken)
			Azzert (! std::strcmp ((*result).at (iToken).GetTextQ ().data (), apszTokens [iToken]));
	}
}
