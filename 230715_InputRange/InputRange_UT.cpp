#include "InputRange_SV.hpp"
#include "InputRange_Stream.hpp"

#include <array>
#include <sstream>

#include <cstring>

int main ()
{
	using namespace ImaginaryPlayer;
	
	const char *const psz {"Hello, World !"};
	const std::string s {psz};
	std::istringstream is {psz};
	
	const std::array <std::unique_ptr <InputRange <char>>, 3> _contpRanges
	{
		std::make_unique <InputRange_SV     <char>> (lyb::string_view {psz}),
		std::make_unique <InputRange_SV     <char>> (lyb::string_view {s  }),
		std::make_unique <InputRange_Stream <char>> (&is)
	};
	
	const std::size_t cc = std::strlen (psz);
	Azzert (s.size () == cc);
	
	for (auto &pRange: _contpRanges)
	{
		std::cout << "InputRange\n{\n";
		std::size_t ic {0};
		for (; ! pRange->Empty (); pRange->PopFront (), ++ic)
		{
			const char c {pRange->Front ()};
			Azzert (c == psz [ic]);
		}
		
		Azzert (ic == cc);
		std::cout << "}\n\n";
	}
}
