#pragma once

#include "_Fwd.hpp"
#include "noncopyable.hpp"

namespace ImaginaryPlayer
{

class WorkerImpl:
	private noncopyable
{
 public:
	virtual ~WorkerImpl () = 0;

 private:
	virtual TimeRep Do_GetTimeToWait () const = 0;

 public:
	TimeRep GetTimeToWait () const;
};

}
