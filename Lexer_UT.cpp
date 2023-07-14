#include "Lexer.hpp"
#include "InputRange_SV.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <cstring>

int main ()
{
	using namespace ImaginaryPlayer;
	
	const char *const input {"  Hello, there from \"   the most beautiful place   \" on...      Earth !!"};
	
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
	
	{
		InputRange_SV <char> inputrange {input};
		
		const auto result {Tokenize (&inputrange)};
		
		Azzert (result);
		
		Azzert (result->size () == nTokens);
		for (std::size_t iToken = 0; iToken < nTokens; ++iToken)
			Azzert (! std::strcmp ((*result).at (iToken).GetTextQ ().data (), apszTokens [iToken]));
	}
	
	{
		std::istringstream is (input);
		
		for (std::size_t iToken = 0; iToken < nTokens; ++iToken)
		{
			const auto optToken {ExtractToken (is)};
			Azzert (optToken);
			
			std::cout << std::setw (2) << iToken << ": `" << optToken->GetText () << "`\n";
			Azzert (! std::strcmp (optToken->GetTextQ ().data (), apszTokens [iToken]));
		}
		
		{
			const auto optToken {ExtractToken (is)};
			Azzert (! optToken);
		}
	}
	
	{
		std::cout << "ExtractIntegral:\n{\n";
		const auto guard = MakeScopeGuard ([] { std::cout << "}\n\n"; });
		for (int sign = 1; sign >= -1; sign -= 2)
		{
			int64_t x {0};
			int64_t y {1};
			for (;;)
			{
				const int64_t z {x + y};
				if (z < x)
					break;
				
				const int64_t zz {sign * z};
				std::cout << std::setw (20) << zz << "\n";
				
				std::ostringstream os; { os << sign * z; }
				std::istringstream is (os.str ());
				const lyb::optional <int64_t> opti {ExtractIntegral (is)};
				Azzert (opti);
				Azzert (*opti == sign * z);
				
				x = y;
				y = z;
			}
		}
	}
}
