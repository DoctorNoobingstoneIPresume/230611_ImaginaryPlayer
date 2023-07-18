#pragma once

#include "_Fwd.hpp"

#include "string_view.hpp"

#include <iosfwd>
#include <string>
#include <functional>

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
	std::size_t hash_value () const noexcept;

 public:
	Song ();

 public:
	std::ostream &Put (std::ostream &os) const;
	std::istream &Get (std::istream &is);
};

std::ostream &operator<< (std::ostream &os, const Song &object);
std::istream &operator>> (std::istream &is,       Song &object);

bool operator== (const Song &x, const Song &y);
bool operator!= (const Song &x, const Song &y);

}

namespace std
{

template <>
struct hash <ImaginaryPlayer::Song>
{
	typedef ImaginaryPlayer::Song argument_type;
	typedef std::size_t           result_type;
	
	std::size_t operator() (const ImaginaryPlayer::Song &song) const noexcept;
};

}
