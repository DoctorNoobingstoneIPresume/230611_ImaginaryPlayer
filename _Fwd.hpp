#pragma once

#include "_FwdConfig.hpp"
#include "string_view.hpp"

#include <chrono>
#include <cstdint>

namespace ImaginaryPlayer
{

using std::uint8_t; using std::uint16_t; using std::uint32_t; using std::uint64_t; using std::uintmax_t; using std::uintptr_t;
using std:: int8_t; using std:: int16_t; using std:: int32_t; using std:: int64_t; using std:: intmax_t; using std:: intptr_t;


// Representationf and type of time units:

typedef int TimeRep;
typedef std::chrono::milliseconds TimePeriod;


// Azzertions (always enabled assertions):
//
// [2023-06-15]
//   Azzertions are useful:
//     - to check pre-conditions, post-conditions, invariants etc.;
//     - for self-documentation;
//     - to improve debuggability -- the sooner after the bug we crash, the more useful the core file is.
//   Besides, the Programmer/Team has not considered these execution paths (and is not supposed to consider them),
//   so these execution paths (which we eliminate altogether via the azzertion-induced crash)
//   are (application-level) undefined behaviour anyway.

[[noreturn]]
void
AzzertionHasFailed_Msg
(
	      lyb::string_view  sFileName,
	      unsigned          iLine,
	      lyb::string_view  sFunctionName,
	      lyb::string_view  sMessage
);

[[noreturn]]
void
AzzertionHasFailed
(
	      lyb::string_view  sFileName,
	      unsigned          iLine,
	      lyb::string_view  sFunctionName
);

}

#define ImaginaryPlayer_Azzert_Msg(bCondition, sMessage) \
	do \
	{ \
		if (! (bCondition)) \
			ImaginaryPlayer::AzzertionHasFailed_Msg (__FILE__, __LINE__, __func__, sMessage); \
	} \
	while (0)

#define ImaginaryPlayer_Azzert(bCondition) \
	do \
	{ \
		if (! (bCondition)) \
			ImaginaryPlayer::AzzertionHasFailed     (__FILE__, __LINE__, __func__); \
	} \
	while (0)

#ifndef IMAGINARYPLAYER_NO_UNAFFIXED_AZZERT
	#define Azzert_Msg ImaginaryPlayer_Azzert_Msg
	#define Azzert     ImaginaryPlayer_Azzert
#endif
