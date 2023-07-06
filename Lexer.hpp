#pragma once

#include "Token.hpp"

#include "optional.hpp"

#include <vector>
#include <locale>


namespace ImaginaryPlayer
{

template <typename Char>
class InputRange;

lyb::optional <std::vector <Token>>
Tokenize
(InputRange <char> *pInputRange, const std::ctype <char> &ctype = std::use_facet <std::ctype <char>> (std::locale {}));

}
