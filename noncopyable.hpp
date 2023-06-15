#pragma once

// [2023-06-15] Inspired from `<boost/core/noncopyable.hpp>`, of course.

namespace ImaginaryPlayer
{

class noncopyable
{
 public:
	~noncopyable () = default;
	noncopyable (const noncopyable &rhs) = delete;
	noncopyable &operator= (const noncopyable &rhs) = delete;
	noncopyable () = default;
};

}
