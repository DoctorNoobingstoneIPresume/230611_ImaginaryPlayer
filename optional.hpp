#pragma once

// [2023-07-05] On this particular computer I have gcc-5.4 with some support for C++17, but without <optional>.

#include "_FwdConfig.hpp"

#if IMAGINARYPLAYER_OPTIONAL_BOOST
 #include <boost/optional.hpp>
#else
 #include <optional>
#endif

namespace lyb
{

#if IMAGINARYPLAYER_OPTIONAL_BOOST
 using boost::optional;
 using boost::none;
#else
 using std::optional;
 using std::none;
#endif

}
