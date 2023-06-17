#include "_Fwd.hpp"

#include <iostream>

#include <cstdlib>

namespace ImaginaryPlayer
{

[[noreturn]]
void
AzzertionHasFailed_Msg
(
	      lyb::string_view  sFileName,
	      unsigned          iLine,
	      lyb::string_view  sFunctionName,
	      lyb::string_view  sMessage
)
{
	std::cout.flush ();
	std::cerr
		<< "Azzertion has failed ("
		<< sFileName << ":" << iLine << " in function `" << sFunctionName << "`): "
		<< sMessage
		<< "...\n";
	std::abort ();
}

[[noreturn]]
void
AzzertionHasFailed
(
	      lyb::string_view  sFileName,
	      unsigned          iLine,
	      lyb::string_view  sFunctionName
)
{
	AzzertionHasFailed_Msg (sFileName, iLine, sFunctionName, "(No specific Azzert message.)");
}

}
