#include "Worker.hpp"
#include "WorkerImpl.hpp"

#include <iostream>
#include <utility>

namespace ImaginaryPlayer
{

Worker::~Worker
()
= default;

//explicit
Worker::Worker
(std::unique_ptr <WorkerImpl> pImpl):
	_pImpl (std::move (pImpl))
{}

//static
void
Worker::ThreadFn
(const std::shared_ptr <Worker> &spWorker)
{
	// [2023-06-17]
	//   We keep the `Worker` alive and we no longer use the reference to the original `shared_ptr` (we shadow it)
	//   because that original `shared_ptr` might be modified/destroyed (on another thread, e.g. the spawner thread).
	const std::shared_ptr <Worker> loc_spWorker {spWorker};
	// [2023-06-21]
	//   Newer versions of g++ do not allow such shadowing.
	//class spWorker;
	Worker *const loc_pWorker {loc_spWorker.get ()};
	
	std::cout << "{ Worker::ThreadFn\n" << std::flush;
	
	for (;;)
	{
		// [2023-06-17] TODO: Remove the periodic wake-ups (in case of `dtWait` being < 0) ?
		const auto tNow = Now ();
		
		const auto arg = WorkerImpl::Arg {}.Now (tNow);
		
		const Duration dtWait {loc_pWorker->_pImpl ? loc_pWorker->_pImpl->GetTimeToWait (arg) : Duration {-1}};
		const bool     dtWait_b {dtWait >= Duration {0}};
		
		std::cv_status status;
		
		std::unique_lock <std::mutex> lock (loc_pWorker->_mtx);
		while (loc_pWorker->_contspWorkItems.empty ())
		{
			status = loc_pWorker->_cv.wait_until
			(
				lock,
				tNow + (dtWait_b ? dtWait : Duration {1000 * 60})
			);
			
			if (status == std::cv_status::timeout && dtWait_b)
				break;
		}
		
		// [2023-06-24]
		//decltype (loc_pWorker->_contspWorkItems) loc_contspWorkItems;
		//{
		//	loc_contspWorkItems.swap (loc_pWorker->_contspWorkItems);
		//}
		const auto loc_contspWorkItems = std::move (loc_pWorker->_contspWorkItems);
		
		lock.unlock ();
		
		bool bEndRequested {false};
		for (const auto &spWorkItem: loc_contspWorkItems)
		{
			if (! spWorkItem)
				{ bEndRequested = true; break; }
			
			const auto rv {(*spWorkItem) ()};
			Azzert ((rv & All_Mask) == rv);
			if ((rv & Break_Mask) != Break_0)
				{ bEndRequested = true; break; }
		}
		
		if (bEndRequested)
			break;
		
		if (status == std::cv_status::timeout && dtWait_b)
		{
			Azzert (loc_pWorker->_pImpl);
			const auto rv {loc_pWorker->_pImpl->OnTimeout (arg)};
			if ((rv & Break_Mask) != Break_0)
				break;
		}
	}
	
	std::cout << "} Worker::ThreadFn\n" << std::flush;
}

void
Worker::AddWorkItem
(const std::shared_ptr <WorkItem> &spWorkItem)
{
	std::unique_lock <std::mutex> lock (_mtx);
	_contspWorkItems.push_back (spWorkItem);
	lock.unlock ();
	_cv.notify_all ();
}

ScopedWorkerThread::~ScopedWorkerThread ()
{
	std::cout << "Joinable " << _thread.joinable () << ".\n" << std::flush;
	_spWorker->AddWorkItem (nullptr);
	_thread.join ();
}

ScopedWorkerThread::ScopedWorkerThread (ScopedWorkerThread &&rhs) noexcept = default;

ScopedWorkerThread &ScopedWorkerThread::operator= (ScopedWorkerThread &&rhs) noexcept = default;

ScopedWorkerThread::ScopedWorkerThread (const std::shared_ptr <Worker> &spWorker):
	_spWorker {spWorker},
	_thread   {[&spWorker] () { Worker::ThreadFn (spWorker); }}
{}

}
