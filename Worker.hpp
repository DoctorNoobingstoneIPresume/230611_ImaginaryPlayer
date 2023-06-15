#pragma once

#include <functional>
#include <memory>
#include <deque>
#include <mutex>
#include <condition_variable>

namespace ImaginaryPlayer
{

class WorkerImpl;

class Worker
{
 public:
	typedef
	      std::function <void ()>                   WorkItem;

	typedef
	      std::shared_ptr <WorkItem>                SPWorkItem;

 private:
	      std::deque <SPWorkItem>                   _contspWorkItems;
	      std::mutex                                _mtx;
	      std::condition_variable                   _cv;
	      std::unique_ptr <WorkerImpl>              _pImpl;

 public:
	static
	void
	ThreadFn
	(const std::shared_ptr <Worker> &spWorker);
	
	void
	AddWorkItem
	(const SPWorkItem &spWorkItem);
};

}
