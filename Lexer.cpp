#include "Lexer.hpp"
#include "InputRange.hpp"

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
				Azzert (! bTokenIsEmpty);
				
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

}
