#include "Logging.hpp"

#include <iostream>
#include <sstream>

int main ()
{
	using namespace ImaginaryPlayer;
	
	{
		const auto s0
		{
			"\n"
			"\n"
			"Hello, there !"
		};
		
		const auto s1
		{
			"\n"
			"\n"
			"\tHello, there !\n"
		};
		
		const auto r1 {Indent (s0)};
		std::cout << r1 << "\n";
		Azzert (r1 == s1);
		
		const auto s2
		{
			"Title\n"
			"{\n"
			"\n"
			"\n"
			"\tHello, there !\n"
			"}\n"
		};
		
		//const auto r2 {IndentWithTitle (s0, "Title")};
		//std::cout << r2 << "\n";
		//Azzert (r2 == s2);
	}
}
