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
	virtual ~WorkerImpl () = 0;

 private:
	virtual TimeRep Do_GetTimeToWait () = 0;
	virtual Worker::WorkItemRV Do_OnTimeout () = 0;

 public:
	TimeRep GetTimeToWait ();
	Worker::WorkItemRV OnTimeout ();
};

}
