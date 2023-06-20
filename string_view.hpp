#pragma once

// [2023-06-11] On this particular computer I have gcc-5.4 with some support for C++17, but without <string_view>.

#include "_FwdConfig.hpp"

#if IMAGINARYPLAYER_STRING_VIEW_BOOST
 #include <boost/utility/string_view.hpp>
#else
 #include <string_view>
#endif

#include <string>

namespace lyb
{

#if IMAGINARYPLAYER_STRING_VIEW_BOOST
 using boost::basic_string_view;
 using boost::      string_view;
#else
 using std  ::basic_string_view;
 using std  ::      string_view;
#endif

// [2023-06-20] This is only needed for converting `boost::string_view` to `std::string`.
std::string ViewToString (string_view sv);

}
