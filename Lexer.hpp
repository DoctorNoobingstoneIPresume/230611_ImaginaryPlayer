#pragma once

#include "Token.hpp"

#include "optional.hpp"
#include "string_view.hpp"

#include <vector>
#include <locale>
#include <string>
#include <iostream>

namespace ImaginaryPlayer
{

template <typename Char>
class InputRange;

lyb::optional <std::vector <Token>>
Tokenize
(InputRange <char> *pInputRange, const std::ctype <char> &ctype = std::use_facet <std::ctype <char>> (std::locale {}));

class Cursor;

lyb::optional <Token>
ExtractToken
(std::istream &is, Cursor *pCursor = nullptr);

// [2023-07-07]
//   `getline_paren (my_istream, my_string, '(', ')')` reads input starting with (possibly whitespace, then) '(' until ')'.
//   It actually expects the input to start with '(' (and otherwise sets `failbit`).
//   The implementation replicates (up to a point) the code in `Tokenize`.
//   This is an artifact of the fact that `InputRange` does not (currently ?) have a `put_back` facility
//   and it probably ought to be able to put back more than just one character
//   and we are trying to support `operator>>` for user-defined types based on tokens;
//   as a work-around for the complexities involved, we might require the input to be delimited with parentheses.

template <typename Char, class Traits, class Alloc>
std::basic_istream <Char, Traits> &
getline_paren
(std::basic_istream <Char, Traits> &is, std::basic_string <Char, Traits, Alloc> &sResult, Char cParenLeft = '(', Char cParenRite = ')')
{
	Azzert (cParenRite != cParenLeft);
	
	std::basic_string <Char, Traits, Alloc> s;
	bool                                    bSuccess {false};
	{
		// [2023-07-07] TODO:
		//   Could the following two lines be merged into `const auto &ctype = std::use_facet <...> (is.getloc ());` ?
		//   I think so, despite the danger of dangling reference, thanks to the specifics of lifetime of facets.
		const std::locale loc = is.getloc ();
		const std::ctype <Char> &ctype = std::use_facet <std::ctype <Char>> (loc);
		
		unsigned iState {0};
		unsigned iLevel {0};
		
		while (is)
		{
			if (is.eof ())
				break;
			
			const typename Traits::int_type c_int {is.get ()};
			const          Char             c     {static_cast <Char> (c_int == Traits::eof () ? 0 : c_int)};
			
			if (! iState)
			{
				if (! c)
					break;
				else
				if (c == cParenLeft)
					{ Azzert (! iLevel); ++iLevel; iState = 1; }
				else
				if (ctype.is (std::ctype_base::space, c))
					;
				else
					break;
			}
			else
			if (iState == 1)
			{
				if (! c)
					break;
				else
				if (c == cParenLeft)
					++iLevel;
				else
				if (c == cParenRite)
				{
					Azzert (iLevel);
					--iLevel;
					if (! iLevel) bSuccess = true;
				}
				else
				if (ctype.is (std::ctype_base::space, c))
					;
				else
				if (ctype.is (std::ctype_base::alnum, c))
					iState = 10;
				else
				if (c == '\"')
					iState = 11;
				else
					;
			}
			else
			if (iState == 10)
			{
				if (! c)
					break;
				else
				if (ctype.is (std::ctype_base::space, c))
					iState = 1;
				else
				if (c == '\"')
					iState = 11;
				else
				if (c == cParenLeft)
					{ ++iLevel; iState = 1; }
				else
				if (c == cParenRite)
				{
					Azzert (iLevel);
					--iLevel;
					if (! iLevel) bSuccess = true;
				}
				else
					;
			}
			else
			if (iState == 11)
			{
				if (! c)
					break;
				else
				if (c == '\"')
					iState = 10;
				else
				if (c == '\\')
					iState = 12;
				else
					;
			}
			else
			if (iState == 12)
			{
				if (! c)
					break;
				else
					iState = 11;
			}
			else
				Azzert (0);
			
			// [2023-07-07] TODO:
			//   All states have performed `if (! c) break;`. Could/should that be unified ?
			//   Currently: `if (! iState) { if (! c) break; ... } else if (iState == ...) { if (! c) break; ... } ...`.
			//   Proposal:  `if (! c) break; if (! iState) { ... } else if (iState == ...) { ... } ...`.
			Azzert (c);
			
			s.push_back (c);
			
			if (bSuccess)
			{
				Azzert (c == cParenRite);
				break;
			}
		}
	}
	
	if (bSuccess)
		sResult.swap (s);
	else
		is.setstate (std::ios_base::failbit);
	
	return is;
}

lyb::optional <std::vector <Token>>
Tokenize_paren
(std::istream &is, char cParenLeft = '(', char cParenRite = ')');

lyb::optional <int64_t>
to_int64_t
(InputRange <char> *pInputRange, const std::ctype <char> &ctype = std::use_facet <std::ctype <char>> (std::locale {}));

lyb::optional <int64_t>
ExtractIntegral
(std::istream &is);

}
