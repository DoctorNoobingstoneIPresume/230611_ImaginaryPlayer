#pragma once

#include "Worker.hpp"        // [2023-06-20] For `Worker::WorkItemRV`.
#include "_Fwd.hpp"
#include "noncopyable.hpp"

namespace ImaginaryPlayer
{

// [2023-06-20]
// WorkerImpl:
//   For some Worker types, the worker thread:
//     (1) uses a specific timeout when waiting for the condition variable;
//     (2) performs a specific function when the timeout has expired.
//   A Worker object can aggregate a WorkerImpl-derived object which specifies the exact behaviour.


class WorkerImpl:
	private noncopyable
{
 public:
	class Arg
	{
	 public:
		typedef Arg ThisType;
	
	 private:
		      TimePoint                 _tNow;
	
	 public:
		TimePoint  Now ()                const;
		ThisType  &Now (TimePoint value);
	
	 public:
		Arg ();
	};

 public:
	virtual ~WorkerImpl () = 0;

 private:
	virtual Duration Do_GetTimeToWait (const Arg &arg) = 0;
	virtual Worker::WorkItemRV Do_OnTimeout (const Arg &arg) = 0;

 public:
	Duration GetTimeToWait (const Arg &arg);
	Worker::WorkItemRV OnTimeout (const Arg &arg);
};

}
