#include "Lexer.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <cstring>

int main ()
{
	using namespace ImaginaryPlayer;
	
	const char *const input {"  Hello, there from \"   the most beautiful place   \" on...      Earth !!"};
	
	const char *const apszExpected []
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
	
	const std::size_t nExpected = nelems (apszExpected);
	
	{
		std::istringstream is (input);
		
		for (std::size_t iExpected = 0; iExpected < nExpected; ++iExpected)
		{
			const auto optToken {ExtractToken (is)};
			Azzert (optToken);
			
			std::cout << std::setw (2) << iExpected << ": `" << optToken->GetText () << "`\n";
			Azzert (! std::strcmp (optToken->GetTextQ ().data (), apszExpected [iExpected]));
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
