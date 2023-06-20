#include "Song.hpp"

#include <iostream>
#include <sstream>

int main ()
{
	using namespace ImaginaryPlayer;
	
	{
		const auto song = Song {}.SetArtistName ("Madonna").SetSongName ("Frozen").SetCodecName ("MP3").SetLength (std::chrono::minutes {5} + std::chrono::seconds {29});
		std::ostringstream os; { os << song; }
		Azzert (os.str () == "artist \"Madonna\", song \"Frozen\", codec \"MP3\", length 329000");
		std::cout << song << '\n';
	}
}
