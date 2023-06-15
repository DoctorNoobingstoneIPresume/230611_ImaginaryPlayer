#pragma once

#include <chrono>
#include <cstdint>

namespace ImaginaryPlayer
{

using std::uint8_t; using std::uint16_t; using std::uint32_t; using std::uint64_t; using std::uintmax_t; using std::uintptr_t;
using std:: int8_t; using std:: int16_t; using std:: int32_t; using std:: int64_t; using std:: intmax_t; using std:: intptr_t;

// Representationf and type of time units:
typedef int TimeRep;
typedef std::chrono::milliseconds TimePeriod;

}
