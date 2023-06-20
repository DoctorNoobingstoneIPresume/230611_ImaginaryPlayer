#include "Song.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

namespace ImaginaryPlayer
{

std::string       Song::GetArtistName  ()                       const { return _sArtistName; }
lyb::string_view  Song::GetArtistNameQ ()                       const { return _sArtistName; }
Song             &Song::SetArtistName  (lyb::string_view value)       { _sArtistName = lyb::ViewToString (value); return *this; }

std::string       Song::GetSongName    ()                       const { return _sSongName; }
lyb::string_view  Song::GetSongNameQ   ()                       const { return _sSongName; }
Song             &Song::SetSongName    (lyb::string_view value)       { _sSongName   = lyb::ViewToString (value); return *this; }

std::string       Song::GetCodecName   ()                       const { return _sCodecName; }
lyb::string_view  Song::GetCodecNameQ  ()                       const { return _sCodecName; }
Song             &Song::SetCodecName   (lyb::string_view value)       { _sCodecName  = lyb::ViewToString (value); return *this; }

TimeRep           Song::GetLength      ()                       const { return _dtLength; }
Song             &Song::SetLength      (TimeRep value)                { _dtLength = value; return *this; }

Song::Song ():
	_dtLength {0}
{}

std::ostream &Song::Put (std::ostream &os) const
{
	std::ostringstream osTmp;
	{
		osTmp
			<< "artist" " \"" << _sArtistName << "\""
			<< ", "
			<< "song"   " \"" << _sSongName   << "\""
			<< ", "
			<< "codec"  " \"" << _sCodecName  << "\""
			<< ", "
			<< "length" " "   << _dtLength;
	}
	
	return os << osTmp.str ();
}

std::ostream &operator<< (std::ostream &os, const Song &object)
{
	return object.Put (os);
}

}
