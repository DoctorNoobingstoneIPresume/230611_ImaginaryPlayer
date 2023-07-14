#include "Song.hpp"
#include "Lexer.hpp"

#include <string>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>

#include <cstring>

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

Duration          Song::GetLength      ()                       const { return _dtLength; }
Song             &Song::SetLength      (Duration value)               { _dtLength = value; return *this; }

Song::Song ():
	_dtLength {0}
{}

std::ostream &Song::Put (std::ostream &os) const
{
	std::ostringstream osTmp;
	{
		osTmp
			<< '('
			<< "artist" " \"" << _sArtistName << "\""
			<< ", "
			<< "song"   " \"" << _sSongName   << "\""
			<< ", "
			<< "codec"  " \"" << _sCodecName  << "\""
			<< ", "
			<< "length" " "   << _dtLength.count ()
			<< ')';
	}
	
	return os << osTmp.str ();
}

std::istream &Song::Get (std::istream &is)
{
	const auto optcontTokens {Tokenize_paren (is)};
	if (! optcontTokens)
		{ is.setstate (std::ios_base::failbit); return is; }
	
	const std::vector <Token> &contTokens {*optcontTokens};
	const std::size_t          nTokens    {contTokens.size ()};
	
	      bool                 bSuccess   {false};
	      unsigned             iState     {0};
	      std::string          sKey;
	      std::size_t          iToken     {0};
	
	for (const auto &token: contTokens)
	{
		const std::string sText {token.GetText ()};
		
		if (! iState)
		{
			Azzert (sText == "(");
			iState = 1;
		}
		else
		if (iState == 1)
		{
			Azzert (sKey.empty ());
			
			if (sText == ")")
				{ bSuccess = true; break; }
			else
				{ sKey = sText; iState = 2; }
		}
		else
		if (iState == 2)
		{
			Azzert (! sKey.empty ());
			
			if (sText == ")")
				{ bSuccess = false; break; }
			else
			{
				std::istringstream isText {sText};
				
				if (sKey == "artist")
					SetArtistName (sText);
				else
				if (sKey == "song")
					SetSongName   (sText);
				else
				if (sKey == "codec")
					SetCodecName  (sText);
				else
				if (sKey == "length")
				{
					const auto optvalue = ExtractIntegral (isText);
					if (! optvalue)
						{ is.setstate (std::ios_base::failbit); break; }
					
					SetLength (Duration {*optvalue});
				}
			
			}
			
			sKey.clear ();
			
			iState = 3;
		}
		else
		if (iState == 3)
		{
			Azzert (sKey.empty ());
			
			if (sText == ")")
				{ bSuccess = true; break; }
			else
			if (sText == ",")
				{ iState = 1; }
			else
				{ bSuccess = false; break; }
		}
		else
			Azzert (0);
	}
	
	if (! bSuccess)
		is.setstate (std::ios_base::failbit);
	
	return is;
}

std::ostream &operator<< (std::ostream &os, const Song &object)
{
	return object.Put (os);
}

std::istream &operator>> (std::istream &is,       Song &object)
{
	return object.Get (is);
}

bool operator== (const Song &x, const Song &y)
{
	return
		! std::strcmp (x.GetArtistNameQ ().data (), y.GetArtistNameQ ().data ()) &&
		! std::strcmp (x.GetSongNameQ   ().data (), y.GetSongNameQ   ().data ()) &&
		! std::strcmp (x.GetCodecNameQ  ().data (), y.GetCodecNameQ  ().data ()) &&
		!      numcmp (x.GetLength      ()        , y.GetLength      ()        );
}

bool operator!= (const Song &x, const Song &y)
{
	return ! (x == y);
}

}
