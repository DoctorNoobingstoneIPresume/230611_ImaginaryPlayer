#pragma once

#include "_FwdConfig.hpp"
#include "string_view.hpp"

#include <utility>
#include <chrono>
#include <cstdint>


namespace ImaginaryPlayer
{


using std::uint8_t; using std::uint16_t; using std::uint32_t; using std::uint64_t; using std::uintmax_t; using std::uintptr_t;
using std:: int8_t; using std:: int16_t; using std:: int32_t; using std:: int64_t; using std:: intmax_t; using std:: intptr_t;


// Representationf and type of time units:

typedef int TimeRep;
typedef std::ratio <1, 1000> TimePeriod;
typedef std::chrono::duration <TimeRep, TimePeriod> Duration;
typedef std::chrono::time_point <std::chrono::steady_clock, Duration> TimePoint;
TimePoint Now    ();
Duration  Now_dt ();


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


// ScopeGuard (based on Andrei Alexandrescu's work) (also on Bjarne's 4th Edition, 13.3.1 - class template `Final_action`):

template <typename F>
class ScopeGuard
{
 private:
	      F                 _f;

 public:
	~ScopeGuard ()
	{
		_f ();
	}
	
	void swap (ScopeGuard &rhs) noexcept
	{
		using std::swap;
		swap (_f, rhs._f);
	}
	
	ScopeGuard (ScopeGuard &&rhs) noexcept:
		_f {std::move (rhs._f)}
	{}
	
	ScopeGuard &operator= (ScopeGuard &&rhs) noexcept
	{
		ScopeGuard tmp (std::move (rhs));
		swap (tmp);
		return *this;
	}
	
	explicit ScopeGuard (F &&f):
		_f {std::forward <F> (f)}
	{}
};

template <typename F>
ScopeGuard <F> MakeScopeGuard (F &&f)
{
	return ScopeGuard <F> {std::forward <F> (f)};
}


} // end namespace


// Azzertion macros:

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


// nelems:

// [2023-07-06] TODO:
//   This implementation of nelems prevents `a` from being a variable with user-defined `operator[]`
//   (by using `0 [a]` instead of `(a) [0]`) (and this is good),
//   but it still allows `a` to be a raw pointer
//   (and this is bad).
//   Only built-in arrays should be allowed.
#define nelems(a) ((sizeof (a) / sizeof (0 [a])))


// numcmp:

template <typename T>
int numcmp (T x, T y)
{
	if (x == y)
		return  0;
	else
	if (x < y)
		return -1;
	else
		return +1;
}


// cc_dt:

const int cc_dt {7};
