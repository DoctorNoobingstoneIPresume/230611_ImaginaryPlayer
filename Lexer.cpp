#include "Lexer.hpp"
#include "InputRange.hpp"
#include "InputRange_SV.hpp" // [2023-07-07] For `Tokenize_paren`.
#include "Cursor.hpp"

#ifndef IMAGINARYPLAYER_Lexer_iDebug
 #define IMAGINARYPLAYER_Lexer_iDebug 0
#endif

#if IMAGINARYPLAYER_Lexer_iDebug
 #include <iostream>
 #include <sstream>
 #include <iomanip>
#endif

namespace ImaginaryPlayer
{

lyb::optional <std::vector <Token>>
Tokenize
(InputRange <char> *pInputRange, const std::ctype <char> &ctype)
{
	std::vector <Token> contTokens;
	bool bSuccess {true};
	{
		unsigned    iRow   {1};
		unsigned    iCol   {1};
		unsigned    iState {0};
		Token       token;
		for (std::size_t iChar = 0; ; ++iChar)
		{
			const char c {pInputRange->Empty () ? static_cast <char> (0) : pInputRange->Front ()};
			
			#if IMAGINARYPLAYER_Lexer_iDebug
			{
				std::ostringstream os;
				{
					os
						<< "@" << std::setw (4) << iRow << ":" << std::setw (4) << iCol << ": "
						<< "iState " << std::setw (2) << iState << ", "
						<< "token " << token << ", "
						<< std::hex << std::uppercase << std::setfill ('0')
						<< "c " << std::setw (2) << static_cast <unsigned> (c) << "h.\n";
				}
				
				std::cout << os.str () << std::flush;
			}
			#endif
			
			const bool bTokenIsEmpty = token.GetTextQ ().empty ();
			if (! token.GetRow ()) Azzert (bTokenIsEmpty);
			if (! token.GetCol ()) Azzert (bTokenIsEmpty);
			
			if (! iState)
			{
				Azzert (bTokenIsEmpty);
				
				if (! c)
					break;
				else
				if (ctype.is (std::ctype_base::space, c))
					;
				else
				if (ctype.is (std::ctype_base::alnum, c))
					{ token.AddText (c).SetRow (iRow).SetCol (iCol); iState = 10; }
				else
				if (c == '\"')
					{ token.SetRow (iRow).SetCol (iCol); iState = 11; }
				else
					contTokens.push_back (Token {}.SetText (std::string (1, c)).SetRow (iRow).SetCol (iCol));
			}
			else
			if (iState == 10)
			{
				// [2023-07-09]
				//   The token might be empty in state 10, for example if it starts with two quotes characters.
				//Azzert (! bTokenIsEmpty);
				
				if (! c || ctype.is (std::ctype_base::space, c))
					{ contTokens.push_back (std::move (token)); token = Token {}; iState = 0; }
				else
				if (c == '\"')
					iState = 11;
				else
				if (ctype.is (std::ctype_base::alnum, c))
					token.AddText (c);
				else
				{
					contTokens.push_back (std::move (token)); token = Token {};
					contTokens.push_back (Token {}.AddText (c).SetRow (iRow).SetCol (iCol));
					iState = 0;
				}
			}
			else
			if (iState == 11)
			{
				if (! c)
					{ bSuccess = false; break; }
				else
				if (c == '\"')
					iState = 10;
				else
				if (c == '\\')
					iState = 12;
				else
					token.AddText (c);
			}
			else
			if (iState == 12)
			{
				if (! c)
					{ bSuccess = false; break; }
				else
					{ token.AddText (c); iState = 11; }
			}
			else
				Azzert (0);
			
			if (! c)
				break;
			else
			if (c == '\n')
				{ ++iRow; iCol = 1; }
			else
				// [2023-07-06] TODO: Special treatment of '\t' ?
				++iCol;
			
			pInputRange->PopFront ();
		}
	}
	
	if (bSuccess)
		return contTokens;
	else
		// [2023-07-06] TODO: Return or output `iRow` and `iCol` so the user can see where exactly lexing has failed ?
		return lyb::none;
}

lyb::optional <Token>
ExtractToken
(std::istream &is, Cursor *pCursor)
{
	Token token;
	bool bSuccess {true};
	{
		const std::ctype <char> &ctype {std::use_facet <std::ctype <char>> (is.getloc ())};
		
		unsigned iState = 0;
		while (is)
		{
			const auto c_int {is.get ()};
			const char c     {static_cast <char> (c_int == std::char_traits <char>::eof () ? 0 : c_int)};
			
			const bool bTokenIsEmpty {token.GetTextQ ().empty ()};
			
			if (! iState)
			{
				Azzert (bTokenIsEmpty);
				
				if (! c)
					{ bSuccess = false; break; }
				else
				if (ctype.is (std::ctype_base::space, c))
					;
				else
				if (ctype.is (std::ctype_base::alnum, c))
					{ if (pCursor) token.SetFrom (*pCursor); token.AddText (c); iState = 10; }
				else
				if (c == '\"')
					{ if (pCursor) token.SetFrom (*pCursor); iState = 11; }
				else
					{ if (pCursor) token.SetFrom (*pCursor); token.AddText (c); break; }
			}
			else
			if (iState == 10)
			{
				if (! c)
					break;
				else
				if (ctype.is (std::ctype_base::space, c))
					{ is.putback (c); break; }
				else
				if (ctype.is (std::ctype_base::alnum, c))
					token.AddText (c);
				else
				if (c == '\"')
					iState = 11;
				else
					{ is.putback (c); break; }
			}
			else
			if (iState == 11)
			{
				if (! c)
					{ bSuccess = false; break; }
				else
				if (c == '\"')
					iState = 10;
				else
				if (c == '\\')
					iState = 12;
				else
					token.AddText (c);
			}
			else
			if (iState == 12)
			{
				if (! c)
					{ bSuccess = false; break; }
				else
					{ token.AddText (c); iState = 11; }
			}
			else
				Azzert (0);
			
			if (pCursor)
				pCursor->Process (c);
			
			Azzert (c);
		}
	}
	
	if (bSuccess)
	{
		Azzert (! token.GetTextQ ().empty ());
		return token;
	}
	else
	{
		is.setstate (std::ios_base::failbit);
		return lyb::none;
	}
}

lyb::optional <std::vector <Token>>
Tokenize_paren
(std::istream &is, char cParenLeft, char cParenRite)
{
	std::string s;
	{
		if (! getline_paren (is, s, cParenLeft, cParenRite))
			return lyb::none;
	}
	
	InputRange_SV <char> inputrange (lyb::string_view {s});
	auto result = Tokenize (&inputrange, std::use_facet <std::ctype <char>> (is.getloc ()));
	if (! result)
		is.setstate (std::ios_base::failbit);
	
	return result;
}

lyb::optional <int64_t>
ExtractIntegral
(std::istream &is)
{
	int64_t rv {0};
	bool bSuccess {false};
	bool bNegative {false};
	{
		const std::ctype <char> &ctype {std::use_facet <std::ctype <char>> (is.getloc ())};
		
		unsigned iState = 0;
		while (is)
		{
			const auto c_int {is.get ()};
			const char c     {static_cast <char> (c_int == std::char_traits <char>::eof () ? 0 : c_int)};
			
			if (! iState)
			{
				if (! c)
					{ bSuccess = false; break; }
				else
				if (ctype.is (std::ctype_base::space, c))
					;
				else
				if (c == '-')
					{ bNegative = ! bNegative; }
				else
				if (c == '+')
					;
				if (c == '0')
					iState = 9;
				else
				if (c >= '1' && c <= '9')
					{ rv = c - '0'; iState = 10; }
				else
					{ bSuccess = false; break; }
			}
			else
			if (iState == 9)
			{
				if (! c)
					{ bSuccess = true; break; }
				else
				if (c == 'x' || c == 'X')
					iState = 16;
				else
				if (c >= '0' && c <= '9')
					{ rv = c - '1'; iState = 10; }
				else
					{ is.putback (c); bSuccess = false; break; }
			}
			else
			if (iState == 10)
			{
				if (! c)
					{ bSuccess = true; break; }
				else
				if (c >= '0' && c <= '9')
					{ rv = rv * 10 + c - '0'; }
				else
				if (ctype.is (std::ctype_base::space, c))
					{ is.putback (c); bSuccess = true; break; }
				else
					{ is.putback (c); bSuccess = false; break; }
			}
			else
			if (iState == 16)
			{
				if (! c)
					{ bSuccess = true; break; }
				else
				if (c >= '0' && c <= '9')
					{ rv = rv * 16 + c - '0'; }
				else
				if (c >= 'A' && c <= 'F')
					{ rv = rv * 16 + c - 'A' + 10; }
				else
				if (c >= 'a' && c <= 'f')
					{ rv = rv * 16 + c - 'a' + 10; }
				else
				if (ctype.is (std::ctype_base::space, c))
					{ is.putback (c); bSuccess = true; break; }
				else
					{ is.putback (c); bSuccess = false; break; }
			}
			else
				Azzert (0);
			
			Azzert (c);
		}
	}
	
	if (bSuccess)
		return bNegative ? - rv : rv;
	else
	{
		is.setstate (std::ios_base::failbit);
		return lyb::none;
	}
}

}

