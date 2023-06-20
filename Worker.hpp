#pragma once

#include "_Fwd.hpp"

#include "noncopyable.hpp"

#include <functional>
#include <memory>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>

namespace ImaginaryPlayer
{

class WorkerImpl;

class Worker
{
 public:
	// The value returned from a WorkItem might include the following bits:
	enum WorkItemRV:
		uint32_t
	{
		// Should the loop of the worker thread end ?
		Break_Mask                    = 1 << 0,
		Break_0                       = 0,
		Break_1                       = Break_Mask,
		
		All_Mask                      = (1 << 1) - 1
	};
	
	typedef
	      std::function <WorkItemRV ()>             WorkItem;

	typedef
	      std::shared_ptr <WorkItem>                SPWorkItem;

 private:
	      std::unique_ptr <WorkerImpl>              _pImpl;
	      std::deque <SPWorkItem>                   _contspWorkItems;
	      std::mutex                                _mtx;
	      std::condition_variable                   _cv;

 public:
	// [2023-06-17] Class mechanics:
	//   We have a `unique_ptr <IncompleteType>` member variable => we need a destructor which sees the complete type.
	//   We also have a `mutex` and a `condition_variable`       => no move operations.
	~Worker
	();
	
	explicit
	Worker
	(std::unique_ptr <WorkerImpl> pImpl = nullptr);
	
	static
	void
	ThreadFn
	(const std::shared_ptr <Worker> &spWorker);
	
	void
	AddWorkItem
	(const SPWorkItem &spWorkItem);
};

// ScopedWorkerThread:
//
// [2023-06-17]
//   Inspired by `boost::strict_scoped_thread`.
//   Unfortunately, `std::thread` lacks the non-intrusive interruption mechanism of `boost::thread`.
//   Therefore, we need intimate knowledge of how our worker thread is stopped.

class ScopedWorkerThread
{
 private:
	      std::shared_ptr <Worker>                  _spWorker;
	      std::thread                               _thread;

 public:
	~ScopedWorkerThread ();
	ScopedWorkerThread (ScopedWorkerThread &&rhs);
	ScopedWorkerThread &operator= (ScopedWorkerThread &&rhs);
	
	ScopedWorkerThread (const std::shared_ptr <Worker> &spWorker);
};

}
