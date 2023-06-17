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
	std::shared_ptr <Worker> loc_spWorker (spWorker);
	class spWorker; // [2023-06-17] Please only use loc_spWorker.
	
	std::cout << "Worker::ThreadFn.\n" << std::flush;
}

ScopedWorkerThread::~ScopedWorkerThread ()
{
	std::cout << "Joinable ?\n" << std::flush;
	_thread.join ();
}

ScopedWorkerThread::ScopedWorkerThread (ScopedWorkerThread &&rhs) = default;

ScopedWorkerThread &ScopedWorkerThread::operator= (ScopedWorkerThread &&rhs) = default;

ScopedWorkerThread::ScopedWorkerThread (const std::shared_ptr <Worker> &spWorker):
	_spWorker (spWorker),
	_thread   ([spWorker] () { Worker::ThreadFn (spWorker); })
{}

}
