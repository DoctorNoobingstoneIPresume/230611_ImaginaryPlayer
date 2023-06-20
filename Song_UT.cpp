#include "Song.hpp"

#include <iostream>
#include <sstream>

int main ()
{
	using namespace ImaginaryPlayer;
	
	{
		const auto song = Song {}.SetArtistName ("Madonna").SetSongName ("Frozen").SetCodecName ("MP3").SetLength ((5 * 60 + 29) * 1000);
		std::ostringstream os; { os << song; }
		Azzert (os.str () == "artist \"Madonna\", song \"Frozen\", codec \"MP3\", length 329000xxx");
		std::cout << song << '\n';
	}
}
