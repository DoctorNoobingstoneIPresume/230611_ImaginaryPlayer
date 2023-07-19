#include "Lexer.hpp"
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

lyb::optional <Token>
ExtractToken
(std::istream &is, Cursor *pCursor, bool bFileName)
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
			
			#if IMAGINARYPLAYER_Lexer_iDebug
			{
				std::ostringstream os;
				{
					os
						<< "iState " << std::setw (2) << iState << ", "
						<< std::hex << std::uppercase << std::setfill ('0')
						<< "c " << std::setw (2) << c_int << "h \'" << c << "\', "
						<< "is.rdstate () " << std::setw (2) << is.rdstate () << "h.\n";
					if (0)
						os
							<< "badbit "  << std::ios_base::badbit  << "h, "
							<< "failbit " << std::ios_base::failbit << "h, "
							<< "eofbit "  << std::ios_base::eofbit  << "h.\n";
				}
				
				std::cout << os.str ();
			}
			#endif
			
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
				{
					if (pCursor) token.SetFrom (*pCursor); token.AddText (c);
					
					if (bFileName)
						iState = 10;
					else
						break;
				}
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
				{
					if (bFileName)
						token.AddText (c);
					else
						{ is.putback (c); break; }
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
			
			if (pCursor)
				pCursor->Process (c);
			
			Azzert (c);
		}
	}
	
	#if IMAGINARYPLAYER_Lexer_iDebug
	{
		std::cout << "bSuccess " << bSuccess << ".\n";
		if (bSuccess)
			std::cout << token << "\n";
	}
	#endif
	
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
			
			#if IMAGINARYPLAYER_Lexer_iDebug
			{
				std::ostringstream os;
				{
					os
						<< "iState " << std::setw (2) << iState << ", "
						<< std::hex << std::uppercase << std::setfill ('0')
						<< "c " << std::setw (2) << c_int << "h \'" << c << "\', "
						<< "is.rdstate () " << std::setw (2) << is.rdstate () << "h.\n";
				}
				
				std::cout << os.str ();
			}
			#endif
			
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
				else
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
		
		#if IMAGINARYPLAYER_Lexer_iDebug
		{
			std::ostringstream os;
			{
				os
					<< "Final is.rdstate () "
					<< std::hex << std::uppercase << std::setfill ('0')
					<< is.rdstate () << ".\n";
			}
			
			std::cout << os.str ();
		}
		#endif
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

