#include "Song.hpp"

#include <iostream>
#include <sstream>

int main ()
{
	using namespace ImaginaryPlayer;
	
	const char *const pszExpected {"(artist \"Madonna\", song \"Frozen\", codec \"MP3\", length 329000)"};
	
	{
		const auto song = Song {}.SetArtistName ("Madonna").SetSongName ("Frozen").SetCodecName ("MP3").SetLength (std::chrono::minutes {5} + std::chrono::seconds {29});
		std::ostringstream os; { os << song; }
		Azzert (os.str () == pszExpected);
		std::cout << song << '\n';
		
		Song songNew;
		{
			std::istringstream is {pszExpected};
			is >> songNew;
			Azzert (is);
		}
		
		std::cout << "songNew:\n" << songNew << '\n';
		
		Azzert (songNew == song);
	}
}
