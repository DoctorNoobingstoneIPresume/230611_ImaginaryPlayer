#pragma once

#include "_Fwd.hpp"

#include "string_view.hpp"

#include <iosfwd>
#include <string>

namespace ImaginaryPlayer
{

class Song
{
 private:
	      std::string                       _sArtistName;
	      std::string                       _sSongName;
	      std::string                       _sCodecName;
	      Duration                          _dtLength;

 public:
	std::string       GetArtistName  ()                       const;
	lyb::string_view  GetArtistNameQ ()                       const;
	Song             &SetArtistName  (lyb::string_view value);
	
	std::string       GetSongName    ()                       const;
	lyb::string_view  GetSongNameQ   ()                       const;
	Song             &SetSongName    (lyb::string_view value);
	
	std::string       GetCodecName   ()                       const;
	lyb::string_view  GetCodecNameQ  ()                       const;
	Song             &SetCodecName   (lyb::string_view value);
	
	Duration          GetLength      ()                       const;
	Song             &SetLength      (Duration value);

 public:
	Song ();

 public:
	std::ostream &Put (std::ostream &os) const;
};

std::ostream &operator<< (std::ostream &os, const Song &object);

}